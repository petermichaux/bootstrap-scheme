.PHONY: clean

scheme: scheme.c
	cc -Wall -ansi -o scheme scheme.c

clean:
	rm scheme
