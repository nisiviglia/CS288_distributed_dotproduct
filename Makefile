
FILES = hw10_dot.c
OBJS = hw10_dot.o
CC = mpicc 
CFLAGS = -g 
INCLUDE = 
LIBS = 

hw5:${OBJ}
	${CC} ${FILES} ${CFLAGS} ${INCLUDES} -o ${OBJS} ${LIBS}

run:${OBJ}
	mpirun -np 4 ./${OBJS} 1000000

clean:
	-rm -f *.o

# mpicc hw10_dot.c -o hw10_dot.o
# mpirun -np 2 ./hw10_dot.o
