CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -O2
CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -Weffc++ -O2
CC=gcc
CXX=g++

ALL=client dead

all: $(ALL)

clean:
	rm -rf $(ALL) piper.tar.gz

test:
	echo "These three should time out properly!"
	python piper.py ./dead ./dead
	python piper.py ./dead ./client
	python piper.py ./client ./dead
	echo "This one should stop when client 2 stops!"
	python piper.py ./client ./client

zip:
	tar zcvf piper.tar.gz Makefile client.cc dead.cc piper.py
