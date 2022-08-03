#!/usr/bin/make -f
#
# Filename      Makefile
# Date          05/09/2021
# Author        Andrew Sen
# Email         ats190003@utdallas.edu
# Version       1.0
# Copyright     2021, All Rights Reserved
#
# Description:
#
#   Makefile for program 5


CXX      = g++
CPPFLAGS = -I/home/010/a/at/ats190003/include 
CXXFLAGS = -Wall -Werror
LDFLAGS  = -static -L/home/010/a/at/ats190003/lib
LDLIBS1  = -lrudeconfig -lcryptopp
LDLIBS2  = -lcryptopp

PROJECTNAME = assign6
EXEC1       = cs3377dirmond
EXEC2       = cs3377decrypt
TARNAME     = assignment6.tar.gz

SRCS1  = $(wildcard dirmond/*.cc)
OBJS1 := $(patsubst %.cc,%.o,$(SRCS1))
DEP1  := $(patsubst %.cc,%.d,$(SRCS1))

SRCS2  = $(wildcard decrypt/*.cc)
OBJS2 := $(patsubst %.cc,%.o,$(SRCS2))
DEP2  := $(patsubst %.cc,%.d,$(SRCS2))


.PHONY: all clean backup tarball


all: $(EXEC1) $(EXEC2)


clean:
	rm -f $(OBJS1) $(OBJS2) $(DEP1) $(DEP2) *~ dirmond/*~ decrypt/*~ $(EXEC1) $(EXEC2)


dirmond/%.d:dirmond/%.cc
	@echo Creating $@
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

decrypt/%.d:decrypt/%.cc
	@echo Creating $@
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


$(EXEC1): $(OBJS1)
	$(CXX) $(LDFLAGS) -o $(EXEC1) $(OBJS1) $(LDLIBS1)

$(EXEC2): $(OBJS2)
	$(CXX) $(LDFLAGS) -o $(EXEC2) $(OBJS2) $(LDLIBS2)


Makefile: $(SRCS1:.cc=.d) $(SRCS2:.cc=.d)


backup:	clean
	@mkdir -p ~/docs/backups; chmod 700 ~/docs/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/docs/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


tarball: all
	rm -f $(TARNAME) $(OBJS1) $(OBJS2) $(DEP1) $(DEP2) dirmond/*~ decrypt/*~; \
	tar cfvz $(TARNAME) Makefile dirmond decrypt $(EXEC1) $(EXEC2) cs3377dirmond.conf; \
	rm -f $(EXEC1) $(EXEC2)


-include $(SRCS1:.cc=.d)
-include $(SRCS2:.cc=.d)

