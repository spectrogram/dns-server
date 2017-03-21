#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <dirent.h>
#include <cstring>

#include "node.h"
#include "trie.h"
#include "record.h"

Trie t = Trie();

int scanFile(std::string path);
int compareStrings(std::string a, std::string b, int index);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " directory path" << std::endl;
		return 1;
	}

	DIR *pDir;
	struct dirent *entry;
	if (pDir = opendir(argv[1])) {
		while (entry = readdir(pDir)) {
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
				std::cout << entry->d_name << "\n";
		}
		closedir(pDir);
	}
	
	scanFile(argv[1]);

	//t.trimTrie(t.getRoot());
	std::cout << " ===================== SCANNING TRIE NOW! " << std::endl;
	t.scanTrie(t.getRoot());	

	return 0;
}