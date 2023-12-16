// addrspace.h 
//	Data structures to keep track of executing user programs 
//	(address spaces).
//
//	For now, we don't keep any information about address spaces.
//	The user level CPU state is saved and restored in the thread
//	executing the user program (see thread.h).
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include <string.h>

#define UserStackSize 1024 	// increase this as necessary!

enum swap_method_t {FIFO, LRU};

class AddrSpace {
    static std::queue<uint32_t> FreeFrameList;
    static std::queue<uint32_t> FreeSectorList;
  public:
    static swap_method_t SwapMethod;
    static inline void PushFreeSector(uint32_t num){ FreeSectorList.push(num); }
    static uint32_t PopFreeSector(void){ 
        if (FreeSectorList.size() == 0) { throw std::exception(); }
        int tmp = FreeSectorList.front();
        FreeSectorList.pop();
        return tmp;
    }

    static inline void PushFreeFrame(uint32_t num){ FreeFrameList.push(num); }
    static uint32_t PopFreeFrame(void){ 
        try {
            if (FreeFrameList.size() == 0) { 
                throw std::exception();
            }
            int tmp = FreeFrameList.front();
            FreeFrameList.pop();
            return tmp;
        } catch(...){
            throw std::exception();
        }
    }
    AddrSpace();			// Create an address space.
    ~AddrSpace();			// De-allocate an address space

    static bool usedPhyPage[NumPhysPages];

    void Execute(char *fileName);	// Run the the program
					// stored in the file "executable"

    void SaveState();			// Save/restore address space-specific
    void RestoreState();		// info on a context switch 

  private:
    TranslationEntry *pageTable;	// Assume linear page table translation for now!
    uint32_t numPages;  // Number of pages in the virtual address space
    uint32_t numSectors;// Number of sector in the virtual address space

    bool Load(char *fileName);		// Load the program into memory
					// return false if not found

    void InitRegisters();		// Initialize user-level CPU registers,
					// before jumping to user code

};

#endif // ADDRSPACE_H
