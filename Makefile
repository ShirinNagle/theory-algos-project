# Example of a comment in Makefile

# How to declare a variable in Makefile and assign executable files
EXECS = 512SHA optsarg sha256 sha512

#identifier for the Makefile
512SHA: 512SHA.c
	cc -o 512SHA 512SHA.c

#sha512sum outputs the hash of the input.txt con command line
test: 512SHA input.txt tests.sh
	./tests.sh
	
# cleans the directory of executable files by running on command line
# make clean
clean:
	rm -f $(EXECS)

optsarg: optargs.c
	cc -o optargs optargs.c
	