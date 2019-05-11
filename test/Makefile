srcs:=$(wildcard ../src/*.c)
srcs+=graph_test.c
objs:=$(patsubst %.c, %.o, $(srcs))
std:=-std=c99

graph:$(objs)
	gcc $(objs) $(std) -lm -o graph

$(filter %.o, $(objs)):%.o:%.c
	gcc -c $(std) $< -o $@

%.d:%.c
	@set -e;rm -f $@; \
	gcc -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(srcs:.c=.d)

.PYTHON: clean
clean:
	-rm graph $(objs) $(srcs:.c=.d)
