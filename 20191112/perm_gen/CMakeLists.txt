#https://cmake.org/documentation
#https://cliutils.gitlab.io/modern-cmake/chapters/basics.html

cmake_minimum_required(VERSION 3.8)

project("Generic Permutation Generator" LANGUAGES CXX)

add_executable(file-entropy file_entropy.cpp cli_parser_base.cpp)
target_compile_features(file-entropy PUBLIC cxx_std_17)

add_custom_target(entropy 
	COMMAND str-perm-gen "Hello World" -c 10 -r 2 -o hw.txt 
	COMMAND echo "Displaying 10 random permutations of Hello World:"
	COMMAND cat hw.txt
	COMMAND echo "Displaying entropy of the dataset..."
	COMMAND file-entropy hw.txt
	COMMAND rm -r hw.txt
	)

add_executable(str-perm-gen-b str_perm_gen_b.cpp cli_parser_base.cpp)
target_compile_features(str-perm-gen-b PUBLIC cxx_std_17)

add_custom_target(benchmark COMMAND str-perm-gen-b "Hello World" -t 10)

add_executable(str-perm-gen str_perm_gen_main.cpp str_perm_gen.cpp cli_parser_base.cpp str_perm_gen_cli.cpp)
target_compile_features(str-perm-gen PUBLIC cxx_std_17)

add_custom_target(permutations 
	COMMAND echo "Displaying 10 permutations of Hello World:"
	COMMAND str-perm-gen "Hello World" -c 10
	)
