# docker 基础阶段 6

学习将本地创建的镜像推送到 `docker hub` 并使用。

首先创建一个账号，用户名如 `escapefromearth`，然后选择 `create a repository`，填写仓库名 `test`。

然后为要推送的本地镜像添加一个 `tag`，就叫做 `escapefromearth/test:v1`，并推送。

```bash
docker tag mytest:v3 escapefromearth/test:v1
docker push escapefromearth/test:v1
```

随后把本地的 `mytest:v3` 和 `escapefromearth/test:v1` 都删掉，然后再运行一下 `escapefromearth/test:v1` 看看效果。

```
docker run -dp 8080:13352 --name testMyFarEndImage escapefromearth/test:v1
```

在本地跑一下 `client` 容器，发现此服务器容器没问题。注意把 `client` 配置的 `ip` 改成宿主机 `ip`，访问端口为上面暴露的 `8080`。

以后那个 `stage5` 里面用到的镜像 `mytest:v3` 只需要改成 `escapefromearth/test:v1` 就能在任何地方使用了。