FROM ubuntu:16.04
LABEL maintainer="Colin Halseth"
LABEL version="1.0"
LABEL description="Creates a container which runs the qlib runtime."

RUN apt update
RUN apt install -y software-properties-common ca-certificates wget curl ssh
RUN apt install -y build-essential

RUN mkdir /usr/qlib
COPY . /usr/qlib
RUN cd /usr/qlib && make build
RUN mv /usr/qlib/bin/run.linux /usr/local/bin/qlib
RUN chmod 777 /usr/local/bin/qlib

ENTRYPOINT /bin/bash