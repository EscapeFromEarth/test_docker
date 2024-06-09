# docker 基础阶段 4

这次主要是实现容器之间通信。所以弄了一个最简单的服务器和客户端，让两者之间通信。

首先构建虚拟网络 `docker network create test-net`。

然后分别在运行服务器和客户端容器的时候指令中添加一个 `--network test-net`。

首先当然是先构建服务器镜像，然后运行（其中的 `-p 8000:13352` 是用来暴露 `8000` 端口给宿主机使用的，这里只是单测试容器间通信，其实是不用的，只不过是我自己想测一下是不是宿主机也可以用）

```bash
# 运行容器指令
docker run -it -p 8000:13352 --name server --network test-net mytest:v3 /bin/bash
# 进入容器之后查看下 ip 地址，出现两个 inet 值，其中非 127.0.0.1 的就是
ifconfig
# 启动服务器
./server
```

然后创建客户端镜像，再运行容器

```bash
# 创建并运行容器
docker run -it --name client --network test-net mytest:v4  
# 访问服务器
./client ${上面 ifconfig 获取到的 ip 地址} 13352
```

则会看到以下的日志

```log
# 客户端
root@19349b593e21:/app# ./client 172.18.0.3 13352
MSG: create socket succ. socket fd 3
MSG: socket connect succ. ip 172.18.0.3, port 13352        
MSG: send msg succ. msg hello
MSG: recv server msg succ. msg len 5, msg HELLO

# 服务端
MSG: socket accept succ. connect fd 15
MSG: recv client msg succ. connect fd 15, msg len 5, msg hello
MSG: send msg succ. connect fd 3, msg len 5, msg HELLO
```

以上就是容器间通信的例子了。

此外实测在宿主机上面运行客户端也能访问到服务，不过命令稍微不一样，是

```bash
./client ${本机 ip 地址} 8000
```