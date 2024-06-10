FROM mybaseimage:v1
WORKDIR /app
COPY . .
RUN apt-get install -y net-tools && g++ -o server server.cpp