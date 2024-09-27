include ../Makefile.inc

LINUX_EXE = 
GEN_EXE = us_xfr_sv us_xfr_cl ud_ucase_sv ud_ucase_cl i6d_ucase_sv i6d_ucase_cl
EXE = ${GEN_EXE} ${LINUX_EXE}


CFLAGS = ${IMPL_CFLAGS} ${IMPL_THREAD_FLAGS}
LDLIBS = ${IMPL_LDLIBS} ${IMPL_THREAD_FLAGS}


i6d_ucase_sv.o i6d_ucase_cl.o : i6d_ucase.h 

id_echo_cl.o id_echo_sv.o : id_echo.h 

is_seqnum_sv.o is_seqnum_cl.o : is_seqnum.h 

is_seqnum_v2_sv.o is_seqnum_v2_cl.o : is_seqnum_v2.h 

scm_cred_recv.o scm_cred_send.o : scm_cred.h

scm_rights_recv.o scm_rights_send.o : scm_rights.h

us_xfr_sv.o us_xfr_cl.o : us_xfr.h 

us_xfr_v2_sv.o us_xfr_v2_cl.o : us_xfr_v2.h 

ud_ucase_sv.o ud_ucase_cl.o : ud_ucase.h 
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