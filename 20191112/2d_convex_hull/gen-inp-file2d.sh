#!/bin/bash

create_random_data_file () {
awk -v n=$1 -v seed="$RANDOM" 'BEGIN { srand(seed); for (i=0; i<n; ++i) printf("%.4f\n", 2*(rand()-0.5)) }' > $2
}

if [ -z "$1" ]
    then
    NUM_POINTS=100
else
    NUM_POINTS=$1
fi

if [ -z "$2" ]
    then
    FILE_NAME=inp-file.csv
else
    FILE_NAME=$2
fi


create_random_data_file $NUM_POINTS tmp1.txt
create_random_data_file $NUM_POINTS tmp2.txt
paste -d , tmp1.txt tmp2.txt > $FILE_NAME
rm tmp1.txt tmp2.txt
