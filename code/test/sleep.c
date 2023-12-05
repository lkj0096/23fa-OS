#include "syscall.h"
main() {
    int i = 0;
    for(i = 1; i < 5; i++){
        PrintInt(i);
        Sleep(1000000 * i);
    }
}