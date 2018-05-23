
graph500:
	make -C graph500-2.1.4
my_make_edgelist: my_make_edgelist.c ./graph500-2.1.4/xalloc.c ./graph500-2.1.4/generator/utils.c
	gcc -g -std=c99 -I./graph500-2.1.4 my_make_edgelist.c ./graph500-2.1.4/xalloc.c ./graph500-2.1.4/generator/utils.c -lm -lrt -o my_make_edgelist

.PHONY : all
all: my_make_edgelist graph500

.PHONY : clean
clean:
	rm -f my_make_edgelist
	-make -C graph500-2.1.4 clean
