// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which) {
	int	type = kernel->machine->ReadRegister(2);
	int	val, valR, valL, my_val = 0, strlen = 0;

    switch (which) {
	case SyscallException:
	    switch(type) {
		case SC_Halt:
		    DEBUG(dbgAddr, "Shutdown, initiated by user program.\n");
   		    kernel->interrupt->Halt();
		    break;
		case SC_PrintInt:
			val=kernel->machine->ReadRegister(4);
			cout << "Print integer:" <<val << endl;
			return;
/*		case SC_Exec:
			DEBUG(dbgAddr, "Exec\n");
			val = kernel->machine->ReadRegister(4);
			kernel->StringCopy(tmpStr, retVal, 1024);
			cout << "Exec: " << val << endl;
			val = kernel->Exec(val);
			kernel->machine->WriteRegister(2, val);
			return;
*/
		case SC_Example:
			val=kernel->machine->ReadRegister(4);
			cout << "EX Value:" <<val << endl;
			return;

		case SC_Sleep:
			val=kernel->machine->ReadRegister(4);
			cout << "Sleep Time:" << val << "ms" << endl;
			kernel->alarm->WaitUntil(val);
			return;

		case SC_Add:
			valR=kernel->machine->ReadRegister(4);
			valL=kernel->machine->ReadRegister(5);
			// cout << "Call Add:" << valR << "+" << valL << "=" << valR + valL << endl;
			kernel->machine->WriteRegister(2, valR + valL);
			return;

		case SC_Sub:
			valR=kernel->machine->ReadRegister(4);
			valL=kernel->machine->ReadRegister(5);
			// cout << "Call Sub:" << valR << "-" << valL << "=" << valR - valL << endl;
			kernel->machine->WriteRegister(2, valR - valL);
			return;

		case SC_Mul:
			valR=kernel->machine->ReadRegister(4);
			valL=kernel->machine->ReadRegister(5);
			// cout << "Call Mul:" << valR << "*" << valL << "=" << valR * valL << endl;
			kernel->machine->WriteRegister(2, valR * valL);
			return;

		case SC_Div:
			valR=kernel->machine->ReadRegister(4);
			valL=kernel->machine->ReadRegister(5);
			if(valL == 0){
				cout << "Error: Divide by zero" << endl;
				kernel->machine->WriteRegister(2, 11015037);
				return;
			}
			// cout << "Call Div:" << valR << "/" << valL << "=" << valR / valL << endl;
			kernel->machine->WriteRegister(2, valR / valL);
			return;

		case SC_Mod:
			valR=kernel->machine->ReadRegister(4);
			valL=kernel->machine->ReadRegister(5);
			if(valL == 0){
				cout << "Error: Mod by zero" << endl;
				kernel->machine->WriteRegister(2, 11015037);
				return;
			}
			// cout << "Call Mod:" << valR << "%" << valL << "=" << valR % valL << endl;
			kernel->machine->WriteRegister(2, valR % valL);
			return;

		case SC_PtrStr:
			strlen = 0;
			val=kernel->machine->ReadRegister(4);
			kernel->machine->ReadMem(val++, 1, &my_val);
			cout << "[B11015037_Print]";
			while(my_val != 0 && val != (int)nullptr){
				strlen++;
				cout << ( ('A'+11 == my_val || 'a'+11 == my_val) ? '*' : (char)my_val);
				kernel->machine->ReadMem(val++, 1, &my_val);
			}
			kernel->machine->WriteRegister(2, strlen);
			return;

		case SC_Exit:
			DEBUG(dbgAddr, "Program exit\n");
			val=kernel->machine->ReadRegister(4);
			cout << "return value:" << val << endl;
			kernel->currentThread->Finish();
			break;
		default:
		    cerr << "Unexpected system call " << type << "\n";
 		    break;
	    }
	    break;
	case PageFaultException:
		cerr << "Page Fault" << "\n";
	    break;
	case AddressErrorException:
		cerr << "address error" << "\n";
	    break;
	case BusErrorException:
		cerr << "addr translation error" << "\n";
	    break;
	default:
	    cerr << "Unexpected user mode exception " << which << "\n";
	    break;
    }
    ASSERTNOTREACHED();
}
