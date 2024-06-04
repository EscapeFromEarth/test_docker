# 借本地镜像一用即可
FROM mytest:v1

ENV DEBIAN_FRONTEND = noninteractive

RUN apt-get update && apt-get install -y build-essential cmake wget

WORKDIR /tmp

# --strip-components=1 表示去掉解压后的那个顶层的文件夹
# ls -la /tmp/protobuf 展示下是否正常解压
RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v3.19.4/protobuf-cpp-3.19.4.tar.gz && \
	mkdir -p /tmp/protobuf && \
	tar -xzvf protobuf-cpp-3.19.4.tar.gz -C /tmp/protobuf --strip-components=1

# 原本用了个 cd 的，结果报错了。。。查了下发现只能用这个 WORKDIR
# chmod +x configure 确保该文件有执行权限（一开始没加导致失败了）
WORKDIR /tmp/protobuf
RUN chmod +x configure && \
	./configure -prefix=/usr && \
	make -j4  && \
	make install

WORKDIR /tmp
RUN rm -rf *

WORKDIR /app
RUN rm -rf *
COPY . .

RUN protoc ./**/*.proto --cpp_out=./ && \
	cmake -S . -B build && cmake --build build

CMD ./build/test