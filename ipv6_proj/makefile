all: prog

bib:
	@echo 'creating out dir \n'
	mkdir -p out
	@echo 'compiling biblioteka_ipv6 \n'
	gcc -o out/biblioteka_ipv6.o -Wall -c -fPIC src/biblioteka_ipv6.c
	@echo 'compiling biblioteka_tcp \n'
	gcc -o out/biblioteka_tcp.o -Wall -c -fPIC src/biblioteka_tcp.c
	@echo 'creating lib dir \n'
	mkdir -p lib
	@echo 'creating shared library biblioteka_ipv6 \n'
	gcc out/biblioteka_ipv6.o -shared -o lib/biblioteka_ipv6.so
	@echo 'creating shared library biblioteka_tcp \n'
	gcc out/biblioteka_tcp.o -shared -o lib/biblioteka_tcp.so

prog: bib
	@echo 'compilig program \n'
	gcc -L"out" -o program -Wall src/ipv6_proj.c -ldl

clean:
	rm -rf out/ lib/ program

.PHONY: all clean bib
