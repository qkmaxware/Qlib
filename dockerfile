FROM ubuntu:16.04
LABEL maintainer="Colin Halseth"

RUN mkdir /usr/qlib
COPY . /usr/qlib
RUN make build
RUN export PATH=$PATH:/usr/qlib/bin
RUN mv /usr/qlib/bin/run.linux /usr/qlib/bin/qlib

CMD /bin/bash