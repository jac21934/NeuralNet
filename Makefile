#!/usr/bin/make -f

TARGET		= net

CXX			= g++
RM			= rm -f

CPPFLAGS	= -MD -Wall -Werror -std=c++11
CXXFLAGS	= -O3
LDFLAGS		=
LDLIBS		=

SRCFILES = network_output.cpp network_init.cpp network_input.cpp main.cpp network.cpp
SRCDIR = src/
SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

condor: CXX = condor_compile g++
condor: LDLIBS += dl-support.o
condor: dl-support.o $(TARGET)

debug: CPPFLAGS += -DDEBUG
debug: CXXFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

dl-support.o:
	$(AR) -x /lib64/libc.a dl-support.o

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

.PHONY: all clean debug

-include $(DEPS)