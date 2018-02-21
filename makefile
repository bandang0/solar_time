# solar_time makefile.
#
# make: compile solar_time
# clean: remove compiled executable
# install: move to /usr/local/bin (run as root)

all:	solar_time.c
	gcc -lm -o solar_time solar_time.c

clean:
	rm ./solar_time

install: 
	mv solar_time /usr/local/bin


