CC=g++
CFLAGS=-std=c++11 -O3
INCLUDES=-I/usr/include/boost/
LIBS=-ltins -lboost_system

all:
	$(CC) $(CFLAGS) $(INCLUDES) node.cpp edge.cpp record.cpp util.cpp trie.cpp parse.cpp test.cpp -o server $(LIBS)
