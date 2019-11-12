#!/bin/bash

#shuf -i 0-100 -n 100 -o tmp1.txt
#shuf -i 0-100 -n 100 -o tmp2.txt
#paste -d , tmp1.txt tmp2.txt > 2d-convex-hull.csv

#: Title        : randomFloats
#: Date         : 2011-03-27
#: Author       : pharaoh
#: Version      : 1.0
#: Discription          : Generate a 4 float numbers with 5 decimal places and write them to a file and a variable


#awk -v "seed=$[(RANDOM & 32767) + 32768 * (RANDOM & 32767)]" \
#       'BEGIN { srand(seed); printf("%.5f\n", rand() * 15.0) }'

#for n in {1..416}
#do
#    echo "$(( ( $RANDOM % 4124 ) + 1 )),$(( ( $RANDOM % 4124 ) + 1 ))"
#done

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
