#!/bin/bash

now=$(date +"%T")
echo "Current time : $now"

./str-perm-gen "Hello World" > hello_world.txt

now=$(date +"%T")
echo "Current time : $now"
