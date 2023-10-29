include ../Makefile.inc

LINUX_EXE = signalfd_sigval

GEN_EXE = ouch intquit nonreentrant t_kill t_sigaltstack
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

nonatomic_uint64 : nonatomic_uint64.c
	${CC} -m32 -o $@ $< ${CFLAGS} ${IMPL_LDLIBS}

${EXE} : ${TLPI_LIB}		# True as a rough approximation