CC=g++
CFLAGS=-g -std=c++0x -Wall

.SUFFIXES: .o .cpp .h

SRC_DIRS = ./ ./benchmarks/ ./concurrency_control/ ./storage/ ./system/
INCLUDE = -I. -I./benchmarks -I./concurrency_control -I./storage -I./system

CFLAGS += $(INCLUDE) -D NOGRAPHITE=1 -O3 -fPIC
LDFLAGS = -Wall -L. -L./libs -pthread -g -lrt -std=c++0x -O3 -ljemalloc
LDFLAGS += $(CFLAGS)
SOFLAGS += -shared -Wl,-soname,$@,-export-dynamic

CPPS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)*.cpp))
OBJS = $(CPPS:.cpp=.o)
OBJS_NOMAIN = $(filter-out ./main.o, $(OBJS))
DEPS = $(CPPS:.cpp=.d)

all: rundb libdb.so.1 libdb.a

rundb: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

libdb.a: $(OBJS_NOMAIN)
	ar rcs $@ $^

libdb.so.1: $(OBJS_NOMAIN)
	$(CC) -o $@ $^ $(LDFLAGS) $(SOFLAGS)

-include $(OBJS:%.o=%.d)

%.d: %.cpp
	$(CC) -MM -MT $*.o -MF $@ $(CFLAGS) $<

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f rundb libdb.so.1 libdb.a $(OBJS) $(DEPS)
