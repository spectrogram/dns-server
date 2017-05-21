#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <dirent.h>
#include <cstring>
#include <tins/tins.h>
#include <iostream>
#include <typeinfo>
#include <exception>
#include <boost/asio.hpp>

#include "node.h"
#include "trie.h"
#include "record.h"
#include "trie_exceptions.h"

using boost::asio::ip::tcp;

Trie t = Trie();

int scanFile(std::string path);
Tins::PacketSender sender;


void server() {
	try {
		boost::asio::io_service io_service;

		tcp::acceptor acceptor(io_service, tcp::endpoint(boost::asio::ip::address_v4::from_string("192.168.1.32"), 53));

		for (;;) {
			tcp::socket socket(io_service);
			tcp::no_delay option(true);
			acceptor.accept(socket);
			socket.get_option(option);

			uint8_t charbuf[1200];
			int readbytes = socket.receive(boost::asio::buffer(charbuf, 1200));

			Tins::DNS dns(charbuf + 2, readbytes - 2);

			if (dns.type() == Tins::DNS::QUERY) {
				std::string queryName;
				std::vector<Record> results;
				for (const auto& query : dns.queries()) {
					queryName = query.dname();
					std::transform(queryName.begin(), queryName.end(), queryName.begin(), ::toupper);

					try {
						results = t.lookup(queryName, query.query_type(), t.getRoot(), 0);
					} catch (nxdomainException &e) {
						// send a response with the NXDOMAIN rcode

						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						dns.rcode(3);

						Tins::PDU::serialization_type s = dns.serialize();
						std::vector<uint8_t> data;

						// uint16_t to uint8_t code kindly provided by James Bott
						uint8_t bHi = (unsigned char)(s.size() / 256);
						uint8_t bLo = (unsigned char)(s.size() % 256);

						data.push_back(bHi);
						data.push_back(bLo);

						data.insert(std::end(data), std::begin(s), std::end(s));

						boost::system::error_code ignored_error;

						boost::asio::write(socket, boost::asio::buffer(data), boost::asio::transfer_all(), ignored_error);
					} catch (std::exception &e) {
						e.what();
					}

					// no results, so return empty answer
					// rcode = 0 (NOERROR) with 0 answers
					if (results.size() == 0) {
						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						dns.rcode(0);
						Tins::PDU::serialization_type s = dns.serialize();

						std::vector<uint8_t> data;

						// uint16_t to uint8_t code kindly provided by James Bott
						uint8_t bHi = (unsigned char)(s.size() / 256);
						uint8_t bLo = (unsigned char)(s.size() % 256);

						data.push_back(bHi);
						data.push_back(bLo);

						data.insert(std::end(data), std::begin(s), std::end(s));

						boost::system::error_code ignored_error;

						boost::asio::write(socket, boost::asio::buffer(data), boost::asio::transfer_all(), ignored_error);
					}

					for (std::vector<Record>::iterator it = results.begin(); it != results.end(); it++) {
						if (query.query_type() == Tins::DNS::MX) {
							Tins::DNS::resource r = Tins::DNS::resource(
								(*it).getName(),
								(*it).getContent(),
								Tins::DNS::MX,
								query.query_class(),
								(*it).getTtl(),
								(*it).getPriority()
							);
							dns.add_answer(r);
						} else if (query.query_type() == Tins::DNS::SOA) {
							// create an SOA record first to get the data out of it
							Tins::DNS::soa_record s = Tins::DNS::soa_record(
								(*it).getContent(),
								(*it).getMail(),
								(*it).getSerial(),
								(*it).getRefresh(),
								(*it).getRetry(),
								(*it).getExpire(),
								(*it).getMinTtl()
							);
							Tins::DNS::resource r = Tins::DNS::resource(
								(*it).getName(),
								(*it).getContent(),
								Tins::DNS::SOA,
								query.query_class(),
								(*it).getTtl()
							);
							r.data(s);
							std::cout << r.data() << std::endl;
							dns.add_answer(r);
						} else {
							Tins::DNS::resource r = Tins::DNS::resource(
								(*it).getName(),
								(*it).getContent(),
								query.query_type(),
								query.query_class(),
								(*it).getTtl()
							);
							dns.add_answer(r);
						}
					}

					if (dns.answers_count() > 0) {
						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						Tins::PDU::serialization_type s = dns.serialize();
						std::vector<uint8_t> data;

						// uint16_t to uint8_t code kindly provided by James Bott
						uint8_t bHi = (unsigned char)(s.size() / 256);
						uint8_t bLo = (unsigned char)(s.size() % 256);

						data.push_back(bHi);
						data.push_back(bLo);

						data.insert(std::end(data), std::begin(s), std::end(s));

						boost::system::error_code ignored_error;

						boost::asio::write(socket, boost::asio::buffer(data), boost::asio::transfer_all(), ignored_error);
					}
				}
			}

		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <directory path> <interface> " << std::endl;
		return 1;
	}

	DIR *pDir;
	struct dirent *entry;
	std::string fullpath;

	if (pDir = opendir(argv[1])) {
		while (entry = readdir(pDir)) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				fullpath = std::string(argv[1]) + std::string(entry->d_name);
				std::cout << fullpath << "\n";
				scanFile(fullpath);
		}
		closedir(pDir);
	}

	// t.scanTrie(t.getRoot());
	t.trimTrie(t.getRoot());

	server();

	return 0;
}
