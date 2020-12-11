#!/bin/bash
set -x

# Create a 10Gb block device
device_name="dbeBlockDevice.img"
dd if=/dev/zero of=${device_name} bs=100M count=100
sudo losetup -fP $device_name

# Create ext4 filesystem and mount
mkfs.ext4 $device_name
mkdir targetfs
device_num=$(losetup -a | grep $(pwd)/$device_name | awk '{print $1}' | sed 's/.$//')
sudo mount -o loop $device_num  targetfs

# Give all permissions to everyone
sudo chmod a+rwx targetfs

# This is needed to allow docker to mount the loopback filesystem
sudo chown -R $USER targetfs/lost+found

