include ../Makefile.inc

GEN_EXE = ouch intquit t_kill nonreentrant

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

# crypt.hを利用する際は、-lcrypt(ここではLINUX_LIBCRYPT)を指定する必要がある
nonreentrant : nonreentrant.o
	${CC} -o $@ nonreentrant.o ${CFLAGS} ${LDLIBS} ${LINUX_LIBCRYPT}

${EXE} : ${TLPI_LIB}		# True as a rough approximation