import json
import numpy as np
from scipy import stats
from django.http import HttpResponse
from . import sampling

lat_quantile=(0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 95, 98, 99, 99.5, 99.9, 100)


def query_sys_static(request):
    ip_addr = request.GET.get('ip_addr', '172.17.0.2')
    response = sampling.get_sys_static_info(ip_addr)
    return HttpResponse(json.dumps(response), content_type="application/json")

def query_sys_dynamic(request):
    ip_addr = request.GET.get('ip_addr', '172.17.0.2')
    sys_info = sampling.get_sys_dynamic_info(ip_addr)
    response = {
        'sys_cpu_percent': sys_info['cpu_percent'],
        'sys_mem_used': sys_info['mem_used'],
        'sys_mem_free': sys_info['mem_free'],
        'sys_net_sent': sys_info['net_sent'],
        'sys_net_recv': sys_info['net_recv'],
    } if sys_info else {}
    return HttpResponse(json.dumps(response), content_type="application/json")

def query_app_static(request):
    return HttpResponse("hello world!")

def query_app_dynamic(request):
    ip_addr = request.GET.get('ip_addr', '172.17.0.2')
    workload = request.GET.get('workload', 'default')
    sampling_size = int(request.GET.get('sampling_size', 1000))

    reqsTime = sampling.sampling(sampling_size)
    que_lat_prob = []
    svc_lat_prob = []
    tot_lat_prob = []
    for perc in lat_quantile:
        que_lat_prob.append([perc, stats.scoreatpercentile(reqsTime['que_tim'], perc)])
        svc_lat_prob.append([perc, stats.scoreatpercentile(reqsTime['svc_tim'], perc)])
        tot_lat_prob.append([perc, stats.scoreatpercentile(reqsTime['lat_tim'], perc)])
    response = {}
    response['fig_lat_prob'] = {
        'que_lat_prob': que_lat_prob,
        'svc_lat_prob': svc_lat_prob,
        'tot_lat_prob': tot_lat_prob,
    }
    response['fig_lat_intr'] = {
        'mean_lat': np.mean(reqsTime['lat_tim']),
        'min_lat':  min(reqsTime['lat_tim']),
        'max_lat':  max(reqsTime['lat_tim']),
        '90_perc': stats.scoreatpercentile(reqsTime['lat_tim'], 90),
        '99_perc': stats.scoreatpercentile(reqsTime['lat_tim'], 99),
        '99.9_perc': stats.scoreatpercentile(reqsTime['lat_tim'], 99.9),
    }
    response['qps'] = float(sampling_size) / reqsTime['tot_tim']
    response['fin_num'] = reqsTime['fin_num']
    response['tot_num'] = reqsTime['tot_num']
    return HttpResponse(json.dumps(response), content_type="application/json")
