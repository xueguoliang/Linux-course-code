
# a.out 目标
# a.o b.o c.o 依赖
# gcc a.o b.o c.o 命令
#
# 命令被执行的条件是：依赖比目标更加新（依赖文件的修改时间，更加接近现在）


target := b.out
allobjs := a.o b.o c.o d.o

#${target}: ${allobjs}
#	gcc ${allobjs} -o ${target}

${target}: ${allobjs}
	gcc -o $@ $^

# $@表示了目标
# $^表示所有依赖
# $<表示第一个依赖

# 依赖可以是迭代的
#a.o: a.c
#	gcc -c a.c

#b.o: b.c
#	gcc -c b.c

#c.o: c.c
#	gcc -c c.c

#d.o: d.c
#	gcc -c d.c
