FROM ubuntu:20.04
WORKDIR /app
COPY . .
RUN apt-get update && apt-get install -y build-essential && g++ -o client client.cpp