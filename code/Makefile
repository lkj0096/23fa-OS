# Copyright (c) 1992,1995 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

MAKE = make
LPR = lpr

all: threads userprog filesys network bin test

.PHONY: threads userprog filesys network bin test

threads: 
	$(MAKE) -C threads depend nachos
userprog:
	$(MAKE) -C userprog depend nachos
filesys:
	$(MAKE) -C filesys depend nachos
network:
	$(MAKE) -C network depend nachos
bin:	
	$(MAKE) -C bin all
test:	
	$(MAKE) -C test all

# don't delete executables in "test" in case there is no cross-compiler
clean:
	/bin/csh -c "rm -f */{core,nachos,DISK,*.o,swtch.s} test/{*.coff} bin/{coff2flat,coff2noff,disasm} "
	$(MAKE) -C test clean

# delete executables in "test"
distclean: clean
	find test -type f ! -name "script" ! -name "*.c" ! -name "*.s" ! -name "Makefile" -exec rm -f {} \;
print:
	/bin/csh -c "$(LPR) Makefile* */Makefile"
	/bin/csh -c "$(LPR) threads/*.h threads/*.cc threads/*.s"
	/bin/csh -c "$(LPR) userprog/*.h userprog/*.cc" 
	/bin/csh -c "$(LPR) filesys/*.h filesys/*.cc
	/bin/csh -c "$(LPR) network/*.h network/*.cc 
	/bin/csh -c "$(LPR) lib/*.h lib/*.cc"
	/bin/csh -c "$(LPR) machine/*.h machine/*.cc
	/bin/csh -c "$(LPR) bin/noff.h bin/coff.h bin/coff2noff.c"
	/bin/csh -c "$(LPR) test/*.h test/*.c test/*.s"

