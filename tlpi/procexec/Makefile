include ../Makefile.inc

LINUX_EXE =

GEN_EXE = child_status t_fork fork_file_sharing footprint t_vfork fork_whos_on_first fork_sig_sync exit_handlers fork_stdio_buf \
			multi_wait make_zombie multi_SIGCHLD envargs t_execve t_execlp \
			t_execle t_execl closeonexec t_system acct_on t_clone
EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation