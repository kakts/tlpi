FROM ubuntu
RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y gcc gdb libcap-dev make
RUN apt-get install -y vim
COPY tlpi /tlpi
RUN cd /tlpi && make