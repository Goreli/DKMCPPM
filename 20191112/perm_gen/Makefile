all: str-perm-gen str-perm-gen-b file-entropy

str-perm-gen: \
	str_perm_gen_main.cpp \
	str_perm_gen.hpp \
	str_perm_gen.cpp \
	str_perm_gen_cli.hpp \
	str_perm_gen_cli.cpp \
	perm_gen_base.hpp \
	perm_gen_base.inl \
	cli_parser_base.hpp \
	cli_parser_base.cpp
	clang++ -O3 -std=c++17 str_perm_gen_main.cpp str_perm_gen.cpp str_perm_gen_cli.cpp cli_parser_base.cpp -o str-perm-gen

str-perm-gen-b: \
	str_perm_gen_b.cpp \
	perm_gen_base.hpp \
	perm_gen_base.inl \
	cli_parser_base.hpp \
	cli_parser_base.cpp
	clang++ -O3 -std=c++17 str_perm_gen_b.cpp cli_parser_base.cpp -o str-perm-gen-b

file-entropy: \
	file_entropy.cpp \
	cli_parser_base.hpp \
	cli_parser_base.cpp
	clang++ -O3 -std=c++17 file_entropy.cpp cli_parser_base.cpp -o file-entropy

clean:
	rm -f str-perm-gen
	rm -f str-perm-gen-b
	rm -f file-entropy
	rm -f hello_world*.txt
	rm -f permuted_greetings*.txt

test: str-perm-gen
	./str-perm-gen Car

benchmark: str-perm-gen-b
	./str-perm-gen-b "Hello World" -t 10

release: ../../elf/release/str-perm-gen	\
	../../elf/release/str-perm-gen-b \
	../../elf/release/file-entropy

../../elf/release/str-perm-gen: str-perm-gen
	mkdir -p ../../elf/release
	cp str-perm-gen ../../elf/release

../../elf/release/str-perm-gen-b: str-perm-gen-b
	mkdir -p ../../elf/release
	cp str-perm-gen-b ../../elf/release

../../elf/release/file-entropy: file-entropy
	mkdir -p ../../elf/release
	cp file-entropy ../../elf/release
