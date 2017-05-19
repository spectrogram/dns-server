#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <dirent.h>
#include <cstring>
#include <tins/tins.h>
#include <iostream>

#include "node.h"
#include "trie.h"
#include "record.h"

Trie t = Trie();

int scanFile(std::string path);
Tins::PacketSender sender;


bool callback(const Tins::PDU& pdu) {
	std::string queryName;
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
			if (query.query_type() == Tins::DNS::A) {
				queryName = query.dname();
				std::transform(queryName.begin(), queryName.end(), queryName.begin(), ::toupper);
				std::vector<Record> results = t.lookup(queryName, Tins::DNS::A, t.getRoot(), 0);
				for (std::vector<Record>::iterator it = results.begin(); it != results.end(); it++) {
					std::cout << (*it).getName() << " " << (*it).getContent() << " " << (*it).getType() << std::endl;
					dns.add_answer(
						Tins::DNS::resource(
							(*it).getName(),
							(*it).getContent(),
							Tins::DNS::A,
							query.query_class(),
							(*it).getTtl()
						)
					);
				}
			}
		}

		if (dns.answers_count() > 0) {
			dns.type(Tins::DNS::RESPONSE);
			dns.recursion_available(0);
			auto pkt = Tins::EthernetII(eth.src_addr(), eth.dst_addr()) /
				Tins::IP(ip.src_addr(), ip.dst_addr()) /
				Tins::UDP(udp.sport(), udp.dport()) /
				dns;
			sender.send(pkt);
			std::cout << "I sent it!" << std::endl;
		}
	}
	return true;
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

	// listen on port 53 in promiscuous mode
	Tins::SnifferConfiguration config;
	config.set_promisc_mode(true);
	config.set_immediate_mode(true);
	config.set_filter("udp and dst port 53");
	Tins::Sniffer sniffer(argv[2], config);
	sender.default_interface(argv[2]);

	std::cout << "Starting sniffing loop" << std::endl;
	sniffer.sniff_loop(callback);


	//t.trimTrie(t.getRoot());
	//std::cout << " ===================== SCANNING TRIE NOW! " << std::endl;
	//t.scanTrie(t.getRoot());

	return 0;
}
