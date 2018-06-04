docker run -it --name=latest --hostname=latest \
    -v /home/xingw/Mixed_Bench/Server/tailbench-v0.9:/root/tailbench-v0.9 \
    -v /home/xingw/Mixed_Bench/Server/tailbench-data:/root/tailbench-data \
    -v /home/xingw/Mixed_Bench/Server/tailbench-scratchData:/root/tailbench-scratchData \
    -v /home/xingw/Mixed_Bench/Client/result:/root/result \
    xingw/centos:latest /bin/bash
