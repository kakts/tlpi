include ../Makefile.inc

GEN_EXE = t_setsid catch_SIGHUP disc_SIGHUP job_mon handling_SIGTSTP orphaned_pgrp_SIGHUP

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation