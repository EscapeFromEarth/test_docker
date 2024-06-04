# docker 基础阶段 2

> 这次相比上次多了个依赖 protobuf 的动态库，所以需要先在里面安装 protobuf，验证了一下自己的想法，就是把那些安装的命令填上去。
>
> 同时还练习了一下 cmake 子库。

### 运行效果

```bash
1
1
1
1
age: 1
toy {
  color: 1
  desc: "1"
}

18
2
5 asd
3 sss
age: 18
toy {
  color: 5
  desc: "asd"
}
toy {
  color: 3
  desc: "sss"
}
```