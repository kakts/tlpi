include ../Makefile.inc

LINUX_EXE =

GEN_EXE = simple_pipe pipe_sync pipe_ls_wc popen_glob fifo_seqnum_server fifo_seqnum_client
EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation