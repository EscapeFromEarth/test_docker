# docker 基础阶段 3

> 主要就是用到了本地目录挂载到镜像目录。

在**创建容器的时候**(注意不是创建镜像的时候)使用 `-v ${宿主机绝对路径}:${镜像路径}`，将来要修改到代码的时候只需要修改宿主机目录的代码，然后重新运行一下容器即可生效。

本阶段的任务就是在这个目录写代码，然后挂载到 `stage1` 创建的容器的 `/tmp` 上，看看效果。注意这里不能直接挂载到 `/app` 上，否则 `docker run` 的时候就是直接执行那个 `CMD` 的命令，而此时项目并没有编译，所以启动失败。正确操作如下：

首先在宿主机上面运行以下指令(这个 `-it` 单纯为了出来的时候不要在那里死循环输出。。其实更合理的基础镜像应该是没有编译和运行项目的)

```bash
# 注意到镜像名称之后的部分为容器启动命令的一部分，所以不能乱放。
docker run -it --name testMount -v D:\XXX\myTest\stage3:/tmp mytest:v1
```

在镜像里面运行以下指令

```bash
cd /tmp
cmake -S . -B build
cmake --build build
./build/test
```

就能看到如下输出了

```bash
# ./build/test
Hello World #0
Hello World #1
Hello World #2
Hello World #3
Hello World #4
Hello World #5
Hello World #6
...
```

同时会发现在 `stage3` 文件夹下会多出一个 `build` 文件夹，当然里面的 `test` 在宿主机肯定是运行不了的，因为宿主机是 `win` 而里面用了 `linux` 的系统调用。