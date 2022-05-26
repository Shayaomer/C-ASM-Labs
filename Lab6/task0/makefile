all:  pipe

pipe: mypipeline.o LineParser.o
	gcc  -g -Wall -o pipe mypipeline.o LineParser.o 

LineParser.o: LineParser.c
	gcc  -g -Wall -c -o LineParser.o LineParser.c

mypipeline.o: mypipeline.c
	gcc  -g -Wall -c -o mypipeline.o  mypipeline.c


clean:
	rm -f *.o pipe
