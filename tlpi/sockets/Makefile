# デフォルトターゲットをallに設定
.DEFAULT_GOAL := all

include ../Makefile.inc

GEN_EXE = i6d_ucase_sv i6d_ucase_cl \
	is_seqnum_sv is_seqnum_cl \
	ud_ucase_sv ud_ucase_cl \
	us_xfr_cl us_xfr_sv

LINUX_EXE = list_host_addresses \
	us_abstract_bind

EXE = ${GEN_EXE} ${LINUX_EXE}

CFLAGS = ${IMPL_CFLAGS} ${IMPL_THREAD_FLAGS}
LDLIBS = ${IMPL_LDLIBS} ${IMPL_THREAD_FLAGS}

# 例: read_line.cをコンパイル対象に追加
OBJS = read_line.o inet_sockets.o

.PHONY: all clean

is_seqnum_cl: is_seqnum_cl.o read_line.o
	${CC} -o $@ is_seqnum_cl.o read_line.o ${CFLAGS} ${LDLIBS}

# is_seqnum_svのビルドルール追加
is_seqnum_sv: is_seqnum_sv.o read_line.o
	${CC} -o $@ is_seqnum_sv.o read_line.o ${CFLAGS} ${LDLIBS}

i6d_ucase_sv.o i6d_ucase_cl.o : i6d_ucase.h

read_line.o: read_line.c read_line.h
	${CC} ${CFLAGS} -c read_line.c

inet_sockets.o: inet_sockets.c inet_sockets.h
	${CC} ${CFLAGS} -c inet_sockets.c

is_seqnum_cl.o : is_seqnum.h is_seqnum.h read_line.h
	${CC} ${CFLAGS} -c is_seqnum_cl.c

is_seqnum_sv.o: is_seqnum_sv.c is_seqnum.h read_line.h
	${CC} ${CFLAGS} -c is_seqnum_sv.c

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