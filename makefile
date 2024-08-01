CC = clang++
CFLAGS = -Wall -std=c++20

INCLUDES = -Iserver/ -I/usr/local/include/drogon/
LIBS = -L/usr/local/lib/ -ldrogon -lsqlite3 -ltrantor -ljsoncpp

SOURCES = $(wildcard server/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ObjectServer

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LIBS) -o $@

server/%.o: server/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

