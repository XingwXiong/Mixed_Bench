# Docker Configuration

1. `yum install -y docker-io` or `apt-get install -y docker.io`
2. `docker build -t xingw/centos:v1 .`


## redis data structure
| field | read | write | len |
|:-----:|:-----|:------|:----|
|IP_ADDRS|sadd |smembers|scard|
|<IP_ADDR>_interval|set|get|--|
|<IP_ADDR>_static|set|get|--|
|<IP_ADDR>_dynamic|lpush|rpop|llen|
