#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import psutil
import redis
import threading
import time
import sys, getopt

ONLINE_APPS  = ('xapian', 'masstree', 'moses', 'img-dnn', 'shore', 'silo', 'specjbb', 'sphinx', 'redis')
OFFLINE_APPS = ('multix', 'sampling', 'logic', 'transform', 'set', 'graph', 'sort', 'statistic')
REDIS_HOST = "172.17.0.1"
REDIS_PORT = 6379
PASSWORD   = "xingwxingw"
# 本地IP
NET_NAME_LIST=("eth0", "docker0", )
# IP_ADDR = psutil.net_if_addrs()['eth0'][0].address if psutil.net_if_addrs()['eth0'] else "172.17.0.2"
bytes_recv = 0
bytes_sent = 0
timestamp  = 0

pool = redis.ConnectionPool(host=REDIS_HOST, password=PASSWORD, port=REDIS_PORT)
conn = redis.Redis(connection_pool=pool)


def get_sys_static_info():
    return {
        'cpu_count': psutil.cpu_count(logical=False),
        'mem_total': psutil.virtual_memory().total,
        'disk_total': psutil.disk_usage('/').total
    }


def get_sys_dynamic_info():
    global conn, bytes_recv, bytes_sent, timestamp
    if bytes_recv == 0 or bytes_sent == 0:
        net_info = psutil.net_io_counters()
        bytes_recv = net_info.bytes_recv
        bytes_sent = net_info.bytes_sent
        timestamp  = time.time()
        time.sleep(sampling_interval)
    net_info = psutil.net_io_counters()
    delta_recv = net_info.bytes_recv - bytes_recv
    delta_sent = net_info.bytes_sent - bytes_sent
    delta_time = time.time() - timestamp
    bytes_recv = net_info.bytes_recv
    bytes_sent = net_info.bytes_sent
    timestamp  = time.time()
    return {
        'timestamp': int(time.time() * 1000),
        'cpu_percent': psutil.cpu_percent(),
        'mem_used': psutil.virtual_memory().used,
        'mem_free': psutil.virtual_memory().free,
        # Mbps
        'net_recv': 1.0 * delta_recv / 1024 / 1024 / delta_time,
        # Mbps
        'net_sent': 1.0 * delta_sent / 1024 / 1024 / delta_time,
    }


def get_online_apps_info():
    pass

def get_offline_apps_info():
    pass

def set_sys_static_info():
    global conn
    static_info = get_sys_static_info()
    conn.set(IP_ADDR + "_static", static_info)

def set_sys_dynamic_info():
    global conn
    dynamic_info = get_sys_dynamic_info()
    conn.lpush(IP_ADDR + "_dynamic",dynamic_info)


def is_real_number(x):
    try:
        float(x)
        return True
    except (ValueError, TypeError):
       pass
    return False

def usage():
    print("Usage: ./system_tz.py [options]")
    print("Options are");
    print("\t-i(--ip) <hostname>    Server hostname (default 127.0.0.1)")
    print("\t-h(--host)             Display usage information(this message)")

if __name__=='__main__':
    net_addrs = psutil.net_if_addrs()
    for net_name in NET_NAME_LIST:
        if net_name in net_addrs.keys():
            IP_ADDR = net_addrs[net_name][0].address
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hi:", ["help", "ip"])
        for opt, arg in opts:
            if opt == '-h':
                usage()
                sys.exit()
            elif opt in('-i', '--ip'):
                IP_ADDR = arg
    except getopt.GetoptError:
        usage()
        sys.exit(2)

    print("IP_ADDR: %s" % IP_ADDR)
    #sys.exit()
    set_sys_static_info()
    while True:
        fin_flag = conn.get(IP_ADDR + "_fin_flag")
        if fin_flag == "True":
            break
        # 采样频率
        sampling_interval = conn.get(IP_ADDR + "_interval")
        #print(sampling_interval, type(sampling_interval))
        if not is_real_number(sampling_interval):
            sampling_interval = 1 
        else:
            sampling_interval = max(1.0, float(sampling_interval))
        set_sys_dynamic_info()
        time.sleep(sampling_interval * 0.95)
