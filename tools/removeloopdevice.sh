#!/bin/bash
set -x

sudo umount targetfs
device_name="dbeBlockDevice.img"
device_num=$(losetup -a | grep $(pwd)/$device_name | awk '{print $1}' | sed 's/.$//')
sudo losetup -d  $device_num
rm -rf targetfs
rm -rf $device_name