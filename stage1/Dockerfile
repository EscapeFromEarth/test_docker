# 下面的命令会按顺序执行

# 使用官方的 Ubuntu 镜像，FROM 指定基础镜像，我下面的命令在
# 这个镜像里面运行，最终创建出我自己的镜像，应该是这么理解。
FROM ubuntu:20.04

# 设置非交互模式，以避免在安装过程中出现提示，比如有些要选时区的，但是用户又互动不了。。
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表并安装构建工具和 cmake，build-essential 会安装 gcc、g++ 和 make，但是不会安装 cmake
RUN apt-get update && apt-get install -y build-essential cmake

# 设置（镜像里面的）工作目录
WORKDIR /app

# 复制项目文件到工作目录，第一个参数是宿主机的工作目录，第二个参数为镜像里面的目录。
COPY . .

# 创建构建目录并构建项目
# `-S .` 指定当前目录（针对宿主机的）为源目录
# `-B build` 指定 build 目录为构建产出存储的目录
# `cmake --build build` 就是构建的意思了
RUN cmake -S . -B build && cmake --build build

# 设置启动命令
# dockerfile 鼓励 json 格式的命令，也就是 `CMD ["executable","param1","param2", ...]`
# 类型的，比如 CMD ["./build/test"]，我下面就不管了，特殊情况是可以直接写的
CMD ./build/test