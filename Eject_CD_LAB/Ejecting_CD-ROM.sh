#!/bin/bash

echo "--Building--"
gcc -o Ejecting_CD-ROM.o Ejecting_CD-ROM.c

echo "--Unmounting CD-ROM--"
sudo umount /dev/sr0

echo "--Running Code--"
./Ejecting_CD-ROM.o