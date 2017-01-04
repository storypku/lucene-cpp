ECHO=echo
CC=g++

CXXFLAGS=-g -fPIC -Wall -Wextra -std=c++11
LDFLAGS=-L/usr/local/lib -lboost_system-mt -lboost_thread-mt -lboost_filesystem-mt -lboost_iostreams-mt

INCLUDES := -Iindex -Istore -Iutil -Iserver

SOURCES := $(wildcard util/*.cpp index/*.cpp store/*.cpp server/*.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: all clean
all: lucene
	@$(ECHO) "Make Lucene-cpp"

lucene: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

clean:
	$(RM) -f $(OBJECTS) lucene
