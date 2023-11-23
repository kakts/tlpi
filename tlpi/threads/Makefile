include ../Makefile.inc

LINUX_EXE =

GEN_EXE = simple_thread simple_thread_detach \
			detached_attr too_simple_thread thread_incr pthread_incr_mutex prod_no_condvar prod_condvar
EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation