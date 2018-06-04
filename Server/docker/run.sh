# cat ~/.ssh/id_rsa.pub >> ./authorized_keys

 docker run -d \
    -v /home/xingw/Mixed_Bench/Server/tailbench-v0.9:/root/tailbench-v0.9 \
    -v /home/xingw/Mixed_Bench/Server/tailbench-data:/root/tailbench-data \
    -v /home/xingw/Mixed_Bench/Server/tailbench-scratchData:/root/tailbench-scratchData \
    -v /home/xingw/Mixed_Bench/Client/result:/root/result \
    -v /home/xingw/Mixed_Bench/Server/dwarf-set:/root/dwarf-set \
    --publish-all=true \
    xingw/centos:latest
    #| xargs -it docker exec {} ifconfig
    #--name=latest --hostname=latest \
    #-p 10000:10000 -p 10001:10001 -p 10002:10002 -p 10003:10003 \
    #-p 10004:10004 -p 10005:10005 -p 10006:10006 -p 10007:10007 \
