import random
import redis

REDIS_HOST = "172.17.0.1"
REDIS_PORT = 6379
PASSWORD   = "xingwxingw"

pool = redis.ConnectionPool(host=REDIS_HOST, password=PASSWORD, port=REDIS_PORT)
conn = redis.Redis(connection_pool=pool)

fin_num = 0
tot_num = 400000

def sampling(size=1000):
    global fin_num
    fin_num = fin_num + random.randint(1200, 1400)
    if fin_num >= tot_num:
        fin_num = random.randint(1200, 1400) 
    reqs = {
        "que_tim": [],
        "svc_tim": [],
        "lat_tim": [],
        'tot_tim': random.randint(5, 50),
        'fin_num': fin_num,
        'tot_num': tot_num,
    }
    for i in range(size):
        que_lat = random.randint(80, 200)
        svc_lat = random.randint(300, 500)
        tot_lat = que_lat + svc_lat
        reqs['que_tim'].append(que_lat)
        reqs['svc_tim'].append(svc_lat)
        reqs['lat_tim'].append(tot_lat)
    return reqs

def get_sys_static_info(ip_addr):
    global conn
    r = conn.get(ip_addr + "_static")
    return eval(r) if r else {}

def get_sys_dynamic_info(ip_addr):
    global conn
    r = conn.lindex(ip_addr + "_dynamic", 0)
    return eval(r) if r else {}

def get_app_static_info(ip_addr, app_name):
    pass

def get_app_dynamic_info(ip_addr, app_name):
    pass

