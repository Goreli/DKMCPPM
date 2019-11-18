all: ch2d inp_file2d.csv

test: ch2d inp_file2d.csv
	./ch2d inp_file2d.csv

ch2d: \
	ch2d_main.cpp ch_alg2d.cpp ch_data_point2d.cpp \
	ch_alg2d.hpp ch_data_point2d.hpp
	clang++ -O3 -std=c++17 ch2d_main.cpp ch_alg2d.cpp ch_data_point2d.cpp -o ch2d

inp_file2d.csv:
	./gen-inp-file2d.sh 100 inp_file2d.csv

clean:
	rm -f ch2d
	rm -f inp_file2d.csv

release: ../../elf/release/ch2d	\
	../../elf/release/gen-inp-file2d.sh

../../elf/release/ch2d: ch2d
	mkdir -p ../../elf/release
	cp ch2d ../../elf/release

../../elf/release/gen-inp-file2d.sh: gen-inp-file2d.sh
	mkdir -p ../../elf/release
	cp gen-inp-file2d.sh ../../elf/release

data100:
	rm -f inp_file2d.csv
	./gen-inp-file2d.sh 100 inp_file2d.csv

data1K:
	rm -f inp_file2d.csv
	./gen-inp-file2d.sh 1000 inp_file2d.csv

data10K:
	rm -f inp_file2d.csv
	./gen-inp-file2d.sh 10000 inp_file2d.csv

data100K:
	rm -f inp_file2d.csv
	./gen-inp-file2d.sh 100000 inp_file2d.csv

data1M:
	rm -f inp_file2d.csv
	./gen-inp-file2d.sh 1000000 inp_file2d.csv

data10M:
	rm -f inp_file2d.csv
	./gen-inp-file2d.sh 10000000 inp_file2d.csv
