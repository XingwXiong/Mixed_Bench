#!/usr/bin/python

import sys
import os
import numpy as np
from scipy import stats

class Lat(object):
    def __init__(self, fileName):
        f = open(fileName, 'rb')
        a = np.fromfile(f, dtype=np.uint64)
        self.reqTimes = a.reshape((int(a.shape[0]/3), 3))
        f.close()

    def parseQueueTimes(self):
        return self.reqTimes[:, 0]

    def parseSvcTimes(self):
        return self.reqTimes[:, 1]

    def parseSojournTimes(self):
        return self.reqTimes[:, 2]

if __name__ == '__main__':
    def getLatPct(latsFile,user_p_num):
        assert os.path.exists(latsFile)
        latsObj = Lat(latsFile)

        qTimes = [l/1e6 for l in latsObj.parseQueueTimes()]
        svcTimes = [l/1e6 for l in latsObj.parseSvcTimes()]
        sjrnTimes = [l/1e6 for l in latsObj.parseSojournTimes()]
        all_time=float(sum(sjrnTimes))
        avg_sjrnTimes=all_time / len(sjrnTimes)
        throughput_rate=len(sjrnTimes)/all_time
        f = open('lats.txt','w')

        f.write('%12s | %12s | %12s\n\n' \
                % ('QueueTimes', 'ServiceTimes', 'SojournTimes'))

        for (q, svc, sjrn) in zip(qTimes, svcTimes, sjrnTimes):
            f.write("%12s | %12s | %12s\n" \
                    % ('%.3f' % q, '%.3f' % svc, '%.3f' % sjrn))
        f.close()
        p95 = stats.scoreatpercentile(sjrnTimes, 95)
        p_num = stats.scoreatpercentile(sjrnTimes, int(user_p_num))
        maxLat = max(sjrnTimes)
        minLat = min(sjrnTimes)
        lllen=len(sjrnTimes) 
        print("len %d"  % (int(lllen)))
        print("user_p_num %d th latency %.3f ms | avg latency %.3f ms | throughput rate : %.3f" \
                % (int(user_p_num),p_num, avg_sjrnTimes,throughput_rate))

    latsFile = sys.argv[1]
    user_num = sys.argv[2]
    getLatPct(latsFile,user_num)
        
