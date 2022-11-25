#!/bin/bash
export num=3
echo $num > var
name="test_cse330"
home=$(whoami)
useradd $name
passwd -d $name
uid=$(id -u $name)
make clean
make

echo "---------------------- Test Case: $1 -----------------------------"
sudo dmesg -c > /dev/null
if [[ $1 -eq 1 ]]; then
    echo "Test script will insert the test kernel module"
    insmod proc_filesys.ko uuid=$uid &&
    echo "echo "whoami" > /proc/cse330/identity"
    echo "whoami" > /proc/cse330/identity
    sleep 2
    echo "Data received from the Kernel space"
    cat /proc/cse330/identity
    echo "------------------------------------------------------------------"
    echo "Test script will remove the test kernel module"
    rmmod proc_filesys
else
    echo "Test script will insert the test kernel module"
    insmod proc_filesys.ko uuid=$uid &&
    echo "Test script will start $num processes for the user $uid"
    su $name -c ./process_gen/process_generator &
    sleep 5
    pids=$(ps -u $uid | awk '{print $1}')
    pidsList=($pids)
    pidsLen=${#pidsList[@]}
    for ((j=1; j<${pidsLen}; j++)); do
        echo "Testing for PID:${pidsList[$j]}"
        echo ${pidsList[$j]}> /proc/cse330/statistics
        sleep 2
        cat /proc/cse330/statistics
        echo "------------------------------------------------------------------"
    done
    echo "Test script will remove the test kernel module"
    rmmod proc_filesys
    echo "------------------------------------------------------------------"
fi
pkill -u $uid; rm -rf var;
