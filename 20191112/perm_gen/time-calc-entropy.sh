#!/bin/bash

now=$(date +"%T:%N")
echo "Current time : $now"

echo "./calc-entropy  -i ../../../../LargeFile.bin"
./calc-entropy  -i ../../../../LargeFile.bin

now=$(date +"%T:%N")
echo "Current time : $now"

echo "./calc-entropy2  -i ../../../../LargeFile.bin"
./calc-entropy2  -i ../../../../LargeFile.bin

now=$(date +"%T:%N")
echo "Current time : $now"

