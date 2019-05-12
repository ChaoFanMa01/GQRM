srcs:=$(wildcard ../src/*.c)
objs:=$(patsubst %.c, %.o, $(srcs))
exe_srcs:=$(wildcard *.c)
exe_objs:=$(patsubst %.c, %.o, $(exe_srcs))
exe:=$(basename $(exe_srcs))
std:=-std=c99

all:$(exe)

.PYTHON: all clean

$(exe):$(exe_objs) $(objs)
	gcc $(patsubst %.c, %.o, $(filter $@.c, $(exe_srcs))) $(objs) $(std) -lm -o $@

$(filter %.o, $(objs)):%.o:%.c
	gcc -c $(std) $< -o $@

%.d:%.c
	@set -e;rm -f $@; \
	gcc -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(srcs:.c=.d) $(exe_srcs:.c=.d)

clean:
	-rm $(objs) $(srcs:.c=.d) $(exe) $(exe_srcs:.c=.d) $(exe_objs)
