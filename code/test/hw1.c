#include "syscall.h"
main() {
    int i = 0;
    int a = 0, myop1 = 2023, myop2 = 112;
    
    for(i = 1; i < 2; i++) {
        a = Add(i, i-1);
        PrintInt(a);
        a = Sub(i, i-1);
        PrintInt(a);
        a = Mul(i, i-1);
        PrintInt(a);
        a = Div(i, i/2);
        PrintInt(a);
        a = Mod(i, i/2);
        PrintInt(a);
    }

    a = Add(myop1, myop2);
    PrintInt(a);
    a = Sub(myop1, myop2);
    PrintInt(a);
    a = Mul(myop1, myop2);
    PrintInt(a);
    a = Div(myop1, myop2);
    PrintInt(a);
    a = Mod(myop1, myop2);
    PrintInt(a);
    
    a = Print("ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz\n");
    PrintInt(a);

    a = Print("Hello NachOS2023!\n");
    PrintInt(a);
    a = Print("Good Morning!\n");
    PrintInt(a);
}