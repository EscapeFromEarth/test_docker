# docker 基础阶段 7

当我们想要备份容器内的一个目录下的数据时，一般有以下方法：

1. 一开始就是用 `bind mount` 的方式，也就是把宿主机的一个绝对路径映射到容器里的一个路径。这种想要备份就最简单，直接把对应目录打包就行。
2. 采用的是 `volume` 的方式，毕竟这种方法也是有好处的，因为容器外就不能直接修改到容器内的内容，更安全。但是备份的时候就要采用更复杂的方式，具体如下。

首先举个例子，有一个容器的功能是专门去 `/data/test.txt` 文件下读内容来输出的，现在想备份这个文件，`/data/` 采用的是 `volume` 方式挂载。

首先直接运行 `docker run -it --name testBackup -v test-volume:/data mybaseimage:v1 /bin/bash`

然后向 `/data/test.txt` 里面输入一些内容。

```bash
XXX:/data# echo "This is some text" > test.txt
XXX:/data# cat test.txt
This is some text
```

再运行一个容器，这个容器专门从 `testBackup` 容器里面拿出 `test.txt` 到本地。方法是让这个容器采用 `bind mount` 的方式将本地一个文件夹映射到它里面的一个 `/backup` 文件夹；同时让它继承 `testBackup` 容器的 `volume` 设置（`--volumes-from testBackup`）；然后进行一些操作，然后删除此镜像（`--rm` 表示运行完就删除）。

```bash
docker run --rm --volumes-from testBackup -v XXX\myTest\stage7:/backup mybaseimage:v1 /bin/bash -c "cp /data/test.txt /backup/"
```

创建 `testBackup` 容器的副本，这个容器用的卷也是新的，保证它是没有 `/data/test.txt` 的。

```bash
docker run -it --name testBackup2 -v test-volume2:/data mybaseimage:v1 /bin/bash
```

然后同样运行一个容器，把 `test.txt` 文件弄到 `testBackup2` 的 `/data/` 上面去。

```bash
docker run --rm --volumes-from testBackup2 -v XXX\myTest\stage7:/backup mybaseimage:v1 /bin/bash -c "cp /backup/test.txt /data/"
```

最后在 `testBackup2` 中 `cat /data/test.txt` 便能得到备份内容。

以上就是本阶段的全部内容了。

但是你会发现其实没必要有 `test-volume` 这种方式，实际上直接的无挂载我也能用这种跑一次性容器的方式给它拿出来。所以其实是通用的。。只不过我看的例子刚好是需要用到 `volume` 的，因为他是数据库。总之我们自己知道核心部分是什么就行了。

还是很感谢[这位博主](https://www.bilibili.com/video/BV11L411g7U1/)的！！！`docker` 练习就到这里了 ~ ~
