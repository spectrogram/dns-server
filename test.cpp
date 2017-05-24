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
#include <chrono>
#include <ctime>

#include "node.h"
#include "trie.h"
#include "record.h"
#include "trie_exceptions.h"
#include "server.h"

using boost::asio::ip::tcp;

Trie t = Trie();

int scanFile(std::string path);
int scanZoneFile(std::string path);

Tins::PacketSender sender;


void server() {
	try {
		boost::asio::io_service io_service;

		tcp::acceptor acceptor(io_service, tcp::endpoint(boost::asio::ip::tcp::v4(), 53));

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

					// having a domain name with less than 3 characters in it is impossible, so return NXDOMAIN
					if (queryName.length() < 3) {
						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						dns.rcode(3);
					}

					// this program does not support SRV records - return NotImp
					if (query.query_type() == Tins::DNS::SRV) {
						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						dns.rcode(4);
					}

					try {
						results = t.lookup(queryName, query.query_type(), t.getRoot(), 0);
					} catch (nxdomainException &e) {
						// send a response with the NXDOMAIN rcode
						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						dns.rcode(3);
					} catch (std::exception &e) {
						e.what();
					}

					// no results, so return empty answer with SOA record if it exists
					// rcode = 0 (NOERROR) with 0 answers
					if (results.size() == 0) {
						results = t.lookup(queryName, Tins::DNS::SOA, t.getRoot(), 0);

						for (std::vector<Record>::iterator it = results.begin(); it != results.end(); it++) {
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
							dns.add_authority(r);
							// dns.authority_count();
						}

						dns.type(Tins::DNS::RESPONSE);
						dns.recursion_available(0);
						dns.rcode(0);
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
					}

					Tins::PDU::serialization_type s = dns.serialize();
					std::vector<uint8_t> data;

					uint8_t bLo = s.size() & 0xff;
					uint8_t bHi = (s.size() >> 8);

					data.push_back(bHi);
					data.push_back(bLo);

					data.insert(std::end(data), std::begin(s), std::end(s));

					boost::system::error_code ignored_error;

					boost::asio::write(socket, boost::asio::buffer(data), boost::asio::transfer_all(), ignored_error);

				}
			}

		}
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

int main(int argc, char *argv[]) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <zone/text> <path>" << std::endl;
		return 1;
	}

	// get the time now before processing starts
	start = std::chrono::system_clock::now();

	if (std::string(argv[1]) == "text") {
		DIR *pDir;
		struct dirent *entry;
		std::string fullpath;
		std::string dir(argv[2]);

		if (pDir = opendir(argv[2])) {
			while (entry = readdir(pDir)) {
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
					fullpath = dir + std::string(entry->d_name);
				}
				scanFile(fullpath);
			}
			closedir(pDir);
		}
	} else if (std::string(argv[1]) == "zone") {
		scanZoneFile(std::string(argv[2]));
	} else {
		std::cerr << "Usage: " << argv[0] << " <zone/text> <path>" << std::endl;
		return 1;
	}

	// get time now after processing
	end = std::chrono::system_clock::now();
	
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "Finished processing at " << std::ctime(&end_time) << "\nElapsed time: " << elapsed_seconds.count() << "s\n";
	
	while (1) {
		try {
			boost::asio::io_service io_service;
			tcp_server server(io_service);
			io_service.run();
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}


	return 0;
}
