## chat-room

基于socket实现的简易聊天室,支持单聊和群聊。

## 基础使用

**服务端运行**

```
./run_server.sh
```

**客户端运行**

```
./run_client.sh
```

**查看在线人数**

```
#1
```

**查看在线用户**

```
#2
```

**单聊**

```
@<name>:<content> // 不以@开头默认群聊
```

**日志保存**

```conf
MY_NAME=test
LOG_FILE=chat.log
```

运行客户端程序,当前目录会生成 `name_chat.log` 文件

## 项目截图

服务端

![](https://cdn.jsdelivr.net/gh/nosleepy/picture@master/img/socket_server.png)

客户端1

![](https://cdn.jsdelivr.net/gh/nosleepy/picture@master/img/socket_client1.png)

客户端2

![](https://cdn.jsdelivr.net/gh/nosleepy/picture@master/img/socket_client2.png)
