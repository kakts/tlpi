include ../Makefile.inc

LINUX_EXE = mmcat
GEN_EXE = 
EXE = ${GEN_EXE} ${LINUX_EXE}


CFLAGS = ${IMPL_CFLAGS} ${IMPL_THREAD_FLAGS}
LDLIBS = ${IMPL_LDLIBS} ${IMPL_THREAD_FLAGS}

# strerror_test: strerror_test.o strerror.o
# 	${CC} -o $@ strerror_test.o strerror.o \
# 		${CFLAGS} ${LDLIBS}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation