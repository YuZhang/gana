# makefile 
#          -- for `fastrace'
# Author: zhangyu @pact518.hit.edu.cn
# Date: 2004-4-13

SHELL := /bin/sh

SRCPATH := .
INCPATH := .
SOURCE := $(wildcard $(SRCPATH)/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SOURCE))

EXECUTABLE := gana 
PACKAGE := gana
VERSION := 0.1.0
CC := g++
#CFLAGS := -DDEBUG -Wall -g -lGTL -I$(INCPATH)
CFLAGS := -DDEBUG -Wall -O3 -lGTL -I$(INCPATH)
#CPPFLAGS := -DDEBUG -Wall -g -I$(INCPATH)
CPPFLAGS := -DDEBUG -Wall -O3 -I$(INCPATH)

.PHONY : all objs clean rebuild install

all : $(EXECUTABLE)

objs : $(OBJS)

clean :
	rm -f $(OBJS)
	rm -f $(EXECUTABLE)

rebuild : clean all

# To install things in the right place
install: $(EXECUTABLE) 
	cp -f $(EXECUTABLE) $(BINPATH)
	cp -f $(MANUAL) $(MANPATH)

$(EXECUTABLE) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

