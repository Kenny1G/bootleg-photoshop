# Victoria Subritzky Katz vsubrit1
# Kenneth Oseleononmen koseleo1 
# Makefile
# 601.220, Spring 2020
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g


#demo: demo_ppm.o ppm_io.o
#	$(CC) demo_ppm.o ppm_io.o -o demo

#demo_ppm.o: demo_ppm.c ppm_io.h
#	$(CC) $(CFLAGS) -c demo_ppm.c

project: project.o blur.o imageManip.o ppm_io.o error.o 
	$(CC) project.o blur.o imageManip.o ppm_io.o error.o -o project -lm

project.o: project.c project.h blur.h imageManip.h ppm_io.h error.h
	$(CC) $(CFLAGS) -c project.c

project_test: project_test.o blur.o imageManip.o ppm_io.o error.o
	$(CC) project_test.o blur.o imageManip.o ppm_io.o error.o -o project_test -lm

project_test.o: project_test.c project_test.h blur.h imageManip.h ppm_io.h project.h error.h
	$(CC) $(CFLAGS) -c project_test.c

blur.o: blur.c blur.h error.h
	$(CC) $(CFLAGS) -c blur.c

imageManip.o: imageManip.c imageManip.h error.h
	$(CC) $(CFLAGS) -c imageManip.c

ppm_io.o: ppm_io.c ppm_io.h error.h
	$(CC) $(CFLAGS) -c ppm_io.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

clean:
	rm -f *.o demo project project_test test/*
