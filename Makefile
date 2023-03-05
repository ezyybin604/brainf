build:
	touch bf
	rm bf
	clang -O2 -o bf intptr.c

install:
	cp bf /usr/local/bin