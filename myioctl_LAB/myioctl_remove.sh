#!/bin/bash

echo "---removing node---"
sudo rm -rf /dev/myioctl 

echo "---removing module---"
sudo rmmod myioctl_driver
echo "------"
sudo dmesg -T | tail

echo "---Checking is file wrote ---"
ls -la /home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt
cat /home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt

echo "---done---"