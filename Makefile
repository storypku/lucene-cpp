ECHO=echo
CXX=g++

CXXFLAGS=-g -fPIC -Wall -Wextra -std=c++11
LDFLAGS=-L/usr/local/lib -lboost_system-mt -lboost_thread-mt -lboost_filesystem-mt -lboost_iostreams-mt

INCLUDES := -Iindex -Iutil -Istore -Iserver

SOURCES := $(wildcard util/*.cpp index/*.cpp store/*.cpp store/*.cpp server/*.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

C_SOUCES := $(wildcard util/md5/*.c)
C_OBJECTS := $(patsubst %.c, %.o, $(C_SOUCES))

.PHONY: all clean
all: lucene
	@$(ECHO) "Make Lucene-cpp"

lucene: $(OBJECTS) $(C_OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

clean:
	$(RM) -f $(OBJECTS) $(C_OBJECTS) lucene
