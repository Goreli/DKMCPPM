#!/bin/bash

now=$(date +"%T")
echo "Current time : $now"

echo "./str-perm-gen \"Hello World\" > hello_world1.txt"
./str-perm-gen "Hello World" > hello_world1.txt

now=$(date +"%T")
echo "Current time : $now"

echo "./str-perm-gen \"Hello World\" -o hello_world2.txt"
./str-perm-gen "Hello World" -o hello_world2.txt

now=$(date +"%T")
echo "Current time : $now"

