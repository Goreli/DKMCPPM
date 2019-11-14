all: str-perm-gen str-perm-gen-b

str-perm-gen: \
	str_perm_gen.cpp \
	perm_gen_base.cpp \
	perm_gen_base.hpp
	clang++ -O3 -std=c++17 str_perm_gen.cpp perm_gen_base.cpp -o str-perm-gen

str-perm-gen-b: \
	str_perm_gen_b.cpp \
	perm_gen_base.cpp \
	perm_gen_base.hpp
	clang++ -O3 -std=c++17 str_perm_gen_b.cpp perm_gen_base.cpp -o str-perm-gen-b

clean:
	rm -f str-perm-gen str-perm-gen-b
