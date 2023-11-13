include ../Makefile.inc

LINUX_EXE = signalfd_sigval

GEN_EXE = child_status t_fork fork_file_sharing footprint t_vfork fork_whos_on_first fork_sig_sync exit_handlers fork_stdio_buf \
			multi_wait
EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation