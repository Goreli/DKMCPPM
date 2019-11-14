all: str-perm-gen str-perm-gen-b

str-perm-gen: \
	str_perm_gen.cpp \
	perm_gen_base.hpp \
	perm_gen_base.inl
	clang++ -O3 -std=c++17 str_perm_gen.cpp -o str-perm-gen

str-perm-gen-b: \
	str_perm_gen_b.cpp \
	perm_gen_base.hpp \
	perm_gen_base.inl
	clang++ -O3 -std=c++17 str_perm_gen_b.cpp -o str-perm-gen-b

clean:
	rm -f str-perm-gen str-perm-gen-b

test: str-perm-gen
	./str-perm-gen Car

benchmark: str-perm-gen-b
	./str-perm-gen-b "Hello World" 10
