FROM ubuntu:20.04
WORKDIR /app
COPY . .
RUN apt-get update && apt-get install -y net-tools build-essential && g++ -o server server.cpp