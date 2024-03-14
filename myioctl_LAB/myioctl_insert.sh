#!/bin/bash

echo "---Building myioctl Module---"
make

echo "---checking myioctl_cnt exist---"
if [ -s "/home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt" ]; then
    echo "myioctl_cnt exist"
else
    touch "/home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt"
    echo "myioctl_cnt not exist, created"
    echo "0" > "/home/lkj/OS_HW4/myioctl_LAB/myioctl_cnt"
fi

echo "---inserting module---"
sudo insmod myioctl_driver.ko
echo "------"
sudo dmesg -T | tail

echo "---making node---"
sudo mknod /dev/myioctl c $(cat /proc/devices | grep "myioctl" | cut -d' ' -f1) 0
sudo chmod 666 /dev/myioctl
ls -lah /dev/myioctl

echo "---done---"