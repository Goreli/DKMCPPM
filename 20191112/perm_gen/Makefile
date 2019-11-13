all: str-perm-gen

run: str-perm-gen
	./str-perm-gen $(arg)

str-perm-gen: \
	str_perm_gen_main.cpp \
	perm_gen.cpp \
	perm_gen.hpp
	clang++ -O3 -std=c++17 str_perm_gen_main.cpp perm_gen.cpp -o str-perm-gen

clean:
	rm -f str-perm-gen
