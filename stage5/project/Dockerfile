FROM mybaseimage:v1
WORKDIR /app
# 注意到挂载的只有代码，之前用 RUN 编译的结果就没了，所以在 CMD 编译才有用
# 有 sleep infinity 就能待机了。
CMD g++ -o client client.cpp && sleep infinity