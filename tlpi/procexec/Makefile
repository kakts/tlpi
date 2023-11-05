include ../Makefile.inc

LINUX_EXE = signalfd_sigval

GEN_EXE = t_fork fork_file_sharing footprint t_vfork
EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation