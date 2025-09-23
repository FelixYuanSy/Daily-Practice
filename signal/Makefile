.PHONY:all
all: signal mykill
signal:testsig.cc
	g++ -o $@ $^ -std=c++11
mykill:mykill.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f signal mykill