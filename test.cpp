#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <dirent.h>
#include <cstring>
#include <tins/tins.h>
#include <iostream>
#include <typeinfo>
#include <exception>

#include "node.h"
#include "trie.h"
#include "record.h"
#include "trie_exceptions.h"

Trie t = Trie();

int scanFile(std::string path);
Tins::PacketSender sender;


bool callback(const Tins::PDU& pdu) {
	std::string queryName;
	std::vector<Record> results;

	// The packet probably looks like this:
	//
	// EthernetII / IP / UDP / RawPDU
	//
	// So we retrieve each layer, and construct a 
	// DNS PDU from the RawPDU layer contents.
	Tins::EthernetII eth = pdu.rfind_pdu<Tins::EthernetII>();
	Tins::IP ip = eth.rfind_pdu<Tins::IP>();
	Tins::UDP udp = ip.rfind_pdu<Tins::UDP>();
	Tins::DNS dns = udp.rfind_pdu<Tins::RawPDU>().to<Tins::DNS>();

	if (dns.type() == Tins::DNS::QUERY) {
		for (const auto& query : dns.queries()) {
			queryName = query.dname();
			std::transform(queryName.begin(), queryName.end(), queryName.begin(), ::toupper);

			try {
				results = t.lookup(queryName, query.query_type(), t.getRoot(), 0);
			} catch (nxdomainException &e) {
				// send a response with the NXDOMAIN rcode
				std::cout << "DOMAIN DOESN'T EXIST!" << std::endl;

				dns.type(Tins::DNS::RESPONSE);
				dns.recursion_available(0);
				dns.rcode(3);
				auto pkt = Tins::EthernetII(eth.src_addr(), eth.dst_addr()) /
					Tins::IP(ip.src_addr(), ip.dst_addr()) /
					Tins::UDP(udp.sport(), udp.dport()) /
					dns;
				sender.send(pkt);

			} catch (std::exception &e) {
				e.what();
			}

			// no results, so return empty answer
			// rcode = 0 (NOERROR) with 0 answers
			if (results.size() == 0) {
				dns.type(Tins::DNS::RESPONSE);
				dns.recursion_available(0);
				dns.rcode(0);
				auto pkt = Tins::EthernetII(eth.src_addr(), eth.dst_addr()) /
					Tins::IP(ip.src_addr(), ip.dst_addr()) /
					Tins::UDP(udp.sport(), udp.dport()) /
					dns;
				sender.send(pkt);
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
				Tins::PDU::serialization_type s = dns.serialize();

				for (std::vector<uint8_t>::iterator it = s.begin(); it != s.end(); it++) {
					std::cout << (int)(*it) << std::endl;
				}

				auto pkt = Tins::EthernetII(eth.src_addr(), eth.dst_addr()) /
					Tins::IP(ip.src_addr(), ip.dst_addr()) /
					Tins::UDP(udp.sport(), udp.dport()) /
					dns;
				sender.send(pkt);
				std::cout << "I sent it!" << std::endl;
			}

			return true;
		}
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

	std::cout << "Finished populating trie" << std::endl;
	// t.scanTrie(t.getRoot());

	// listen on port 53 in promiscuous mode
	Tins::SnifferConfiguration config;
	config.set_immediate_mode(true);
	config.set_filter("udp and dst port 53");
	Tins::Sniffer sniffer(argv[2], config);
	sender.default_interface(argv[2]);
	sender.open_l3_socket(Tins::PacketSender::SocketType::IP_UDP_SOCKET);

	std::cout << "Starting sniffing loop" << std::endl;
	sniffer.sniff_loop(callback);

	//t.trimTrie(t.getRoot());
	//std::cout << " ===================== SCANNING TRIE NOW! " << std::endl;

	return 0;
}
