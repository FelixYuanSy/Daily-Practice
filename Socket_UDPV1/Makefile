.PHONY:all
all:udpclient udpserver

udpclient: Udpclient.cc
	g++ -o $@ $^ -std=c++17
udpserver: UdpServer.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f udpclient udpserver