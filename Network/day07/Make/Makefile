
target := b.out
srcs := $(shell ls *.c)
objs := $(srcs:.c=.o)
deps := $(srcs:.c=.d)  
# a.d b.d c.d d.d e.d

all: ${target}

${target}: $(objs)
	$(CC) -o $@ $^ 

-include ${deps}

%.d: %.c
	gcc -MM $< > $@

clean: 
	rm ${target} *.o *.d -rf 

test:
	@echo source file is: $(srcs)
	@echo object file is: $(objs)
