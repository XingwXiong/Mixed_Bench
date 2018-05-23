#!/bin/bash
port=8080
sudo lsof -i:${port} | tail -n +2 | head -n 1 | awk '{system("sudo kill -9 " $2)}'
python ./manage.py runserver 0.0.0.0:${port} > run.log 2>&1 &
sleep 1
cnt=`sudo lsof -i:${port} | tail -n +2 | wc -l`
if [[ ${cnt} -ne 0 ]]; then
    echo "django started successfully!"
else
    echo "django failed to start!"
fi

sleep 1
#sudo lsof -i:6379 | tail -n +2 | awk '{system("sudo kill -9 " $2)}'
sudo lsof -i:6379 | tail -n +2 | head -n 1 |awk '{system("sudo kill -9 " $2)}'
redis-server ./redis.conf
cnt=`sudo lsof -i:6379 | tail -n +2 | wc -l`
if [[ ${cnt} -ne 0 ]]; then
    echo "redis-server started successfully!"
else
    echo "redis-server failed to start!"
fi
