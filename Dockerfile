FROM ubuntu:latest
MAINTAINER Upendra Chitnis (upendrachitnis@gmail.com)
ENV WIT_HOME /app
ENV PLATFORM linux
COPY . /app
WORKDIR /app/mcl/linux/
RUN apt-get update && apt-get install sudo && apt-get install -y build-essential
RUN make exe_type=released all 
RUN ./unitTest
