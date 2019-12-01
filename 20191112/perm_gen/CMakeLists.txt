#https://cmake.org/documentation
#https://cliutils.gitlab.io/modern-cmake/chapters/basics.html

cmake_minimum_required(VERSION 3.8)

project("Generic Permutation Generator" LANGUAGES CXX)

add_executable(calc-entropy calc_entropy_main.cpp calc_entropy_cli.cpp cli_parser_base.cpp)
target_compile_features(calc-entropy PUBLIC cxx_std_17)

add_custom_target(entropy 
	COMMAND str-perm-gen -is "Hello World" -c 10 -rpa 2 -o hw.txt 
	COMMAND echo "Displaying 10 random permutations of Hello World:"
	COMMAND cat hw.txt
	COMMAND echo "Displaying entropy of the dataset..."
	COMMAND calc-entropy -i hw.txt -ft t
	COMMAND rm -r hw.txt
	)

add_executable(str-perm-gen str_perm_gen_main.cpp str_perm_gen.cpp cli_parser_base.cpp str_perm_gen_cli.cpp)
target_compile_features(str-perm-gen PUBLIC cxx_std_17)

add_custom_target(test_run 
	COMMAND echo "Displaying 10 permutations of Hello World:"
	COMMAND str-perm-gen -is "Hello World" -c 10
	)
