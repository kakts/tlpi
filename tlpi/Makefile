# Makefile to build all programs in all subdirectories
#
# DIRS is a list of all subdirectories containing makefiles
# (The library directory is first so that the library gets built first)
#

DIRS = 	lib \
		time \
		inotify \
		signals \
		timers \
		procexec \
		threads \
		daemons \
		mmap \
		pgsjc \
		pipes \
		procpri \
		procres \
		svmsg \
		sockets

# The "namespaces" directory is deliberately excluded from the above
# list because much of the code requires a fairly recent kernel and
# userspace to build. Nevertheless, there is a Makefile in that directory.

BUILD_DIRS = ${DIRS}


# Dummy targets for building and clobbering everything in all subdirectories

all: 	
	echo ${BUILD_DIRS}
	@ for dir in ${BUILD_DIRS}; do (cd $${dir}; ${MAKE}) ; done

allgen: 
	@ for dir in ${BUILD_DIRS}; do (cd $${dir}; ${MAKE} allgen) ; done

clean: 
	@ for dir in ${BUILD_DIRS}; do (cd $${dir}; ${MAKE} clean) ; done

.PHONY prune:
prune:
	docker image prune
	docker container prune