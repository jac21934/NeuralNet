#!/usr/bin/make -f

TARGET		= net

CXX			= g++
RM			= rm -f

CPPFLAGS	= -MD -Wall -Werror -std=c++11
CXXFLAGS	= -g -O3
LDFLAGS		=
LDLIBS		=

SRCFILES = network_output.cpp network_init.cpp network_input.cpp main.cpp random_lib.cpp network.cpp
SRCDIR = src/
SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

.PHONY: all clean

-include $(DEPS)