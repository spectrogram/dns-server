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

	std::cout << "Finished processing at " << std::ctime(&end_time) << "Elapsed time: " << elapsed_seconds.count() << "s\n";
	
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
