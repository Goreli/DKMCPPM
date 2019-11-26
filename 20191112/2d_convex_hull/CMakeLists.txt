#https://cmake.org/documentation
#https://cliutils.gitlab.io/modern-cmake/chapters/basics.html

cmake_minimum_required(VERSION 3.8)

project("2D Convex Hull" LANGUAGES CXX)

add_executable(ch2d ch2d_main.cpp ch_alg2d.cpp ch_data_point2d.cpp)
target_compile_features(ch2d PUBLIC cxx_std_17)

add_custom_command(OUTPUT inp_file2d.csv 
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 100 inp_file2d.csv
	)
add_custom_target(test_run 
	COMMAND ch2d inp_file2d.csv
	DEPENDS inp_file2d.csv
	)

add_custom_target(data100
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 100 inp_file2d.csv
)
add_custom_target(data1K
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 1000 inp_file2d.csv
)
add_custom_target(data10K
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 10000 inp_file2d.csv
)
add_custom_target(data100K
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 100000 inp_file2d.csv
)
add_custom_target(data1M
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 1000000 inp_file2d.csv
)
add_custom_target(data10M
	COMMAND rm -f inp_file2d.csv
	COMMAND ../gen-inp-file2d.sh 10000000 inp_file2d.csv
)
