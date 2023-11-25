include ../Makefile.inc

LINUX_EXE =

GEN_EXE = simple_thread simple_thread_detach \
			detached_attr too_simple_thread thread_incr pthread_incr_mutex prod_no_condvar prod_condvar \
			thread_multijoin thread_cancel thread_cleanup
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