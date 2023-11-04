include ../Makefile.inc

GEN_EXE = demo_timerfd ptmr_sigev_signal ptmr_sigev_thread real_timer timed_read t_nanosleep

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

ptmr_sigev_signal: ptmr_sigev_signal.o
	${CC} -o $@ ptmr_sigev_signal.o ${CFLAGS} ${LDLIBS} ${LINUX_LIBRT}

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation