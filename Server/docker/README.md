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


---

**Tips:**

1. File Server/docker/jdk-8u171-linux-x64.tar.gz is 182.05 MB; this exceeds GitHub's file size limit of 100.00 MB;
so maybe you should download this file yourself. :smile:
2. If you need to login to container without password, you should replace the file `authorized_keys`.
    - ssh-keygen -t rsa
    - cp ~/.ssh/id_dsa.pub authorized_keys
    - ssh -o StrictHostKeyChecking=no 172.17.0.XX
