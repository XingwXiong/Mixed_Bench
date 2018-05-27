# 面向云计算数据中心的混合负载发生器在线监控平台

## 依赖情况：

Web-Server
1. `python` + `django` + `numpy` + `scipy`
    - yum install -y python
    - pip install django numpy scipy
2. `redis` + `python redis`
    - yum install -y redis
    - pip install redis

docker 
1. yum -y install docker-io
2. cd ../Server/docker
3. docker built -t xingw/centos:v1 .

## 在线监控平台的大致功能如下（部分功能还未完成）：

1. 启动负载，并设置负载发生器的请求参数；
2. 实时显示在线型负载的`延迟-概率分布图`、`延迟-时序分布图`、`吞吐-时序分布图`；
3. 实时显示系统资源（cpu、mem、net、disk）等资源的利用情况:
    - 监控平台启动redis服务；
    - 被监控的服务器定时向监控平台redis写入资源利用情况；
4. 请求全部结束后，生成实验报告。

