all: 2d-convex-hull inp-file.csv

2d-convex-hull: \
	ch2d-main.cpp ch2d-alg.cpp ch2d-data-point.cpp \
	ch2d-alg.hpp ch2d-data-point.hpp
	clang++ -O3 -std=c++17 ch2d-main.cpp ch2d-alg.cpp ch2d-data-point.cpp -o 2d-convex-hull

inp-file.csv:
	./gen-inp-file.sh 100 inp-file.csv

data100:
	rm -f inp-file.csv
	./gen-inp-file.sh 100 inp-file.csv

data1K:
	rm -f inp-file.csv
	./gen-inp-file.sh 1000 inp-file.csv

data10K:
	rm -f inp-file.csv
	./gen-inp-file.sh 10000 inp-file.csv

data100K:
	rm -f inp-file.csv
	./gen-inp-file.sh 100000 inp-file.csv

data1M:
	rm -f inp-file.csv
	./gen-inp-file.sh 1000000 inp-file.csv

clean:
	rm -f 2d-convex-hull
	rm -f inp-file.csv

run:
	./2d-convex-hull
