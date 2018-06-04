
Applications
============

TailBench includes eight latency-critical applications, each with its own
subdirectory:

 - img-dnn      : Image recognition
	- yum install -y opencv opencv-devel
 - masstree     : Key-value store
	- git clone https://github.com/gperftools/gperftools
	- make && make install
 - moses        : Statistical machine translation
	- yum install -y bison swig boost-devel 
 - shore        : OLTP database (optimized for disk/ssd)
	- yum install -y readline-devel libgtop2-devel  
 - silo         : OLTP database (in-memory)
	- yum install -y jemalloc-devel libdb-cxx-devel openssl-devel numactl-devel libaio-devel
 - specjbb      : Java middleware
 - sphinx       : Speech recognition
 - xapian       : Online search

Please see the TailBench paper ("TailBench: A Benchmark Suite and Evaluation
Methodology for Latency-Critical Applications", Kasture and Sanchez, IISWC-2016)
for further details on these applications.


Harness
=======

The TailBench harness controls application execution (e.g., implementing warmup
periods, generating request traffic during measurement periods), and measures
request latencies (both service and queuing components). The harness can be set
up in one of three configurations:

 - Networked    : Client and application run on different machines, communicate
                  over TCP/IP
 - Loopback     : Client and application run on the same machine, communicate
                  over TCP/IP
 - Integrated   : Client and applicaion are integrated into a single process and
                  communicate over shared memory

See the TailBench paper for more details on these configurations.

Each TailBench application has two components: a server comoponent that
processes requests, and a client component that generates requests. Both
components interact with the harness via a simple C API. Further information on
the API can be found in the header files harness/tbench_server.h (for the server
component) and harness/tbench_client.h (for the client component). 

Application and client execution is controlled via environment variables. Some
of these are common for all three configurations, while others are specific to
some configurations. We describe the environment variables in each of these
categories below. The quantities in parentheses indicate whehter the environment
variable is used for the application or the client, and if it is only used for
some configurations (e.g. networked + loopback).  Additionally, some application
clients use additional environment variables for configuration. These are
described in the README files within application directories where applicable.

** Environment Variables **

TBENCH_WARMUPREQS (application): Length of the warmup period in # requests. No
latency measurements are performed during this period.

TBENCH_MAXREQS (application): The total number of requests to be executed during
the measurement period (the region of interest). This count *does not* include
warmup requests.

TBENCH_MINSLEEPNS (client): The mininum length of time, in ns, for which the
client sleeps in the kernel upon encountering an idle period (i.e., when no
requests are submitted).

TBENCH_QPS (client): The average request rate (queries per second) during the
measurement period. The harness generates interarrival times using an
exponential distribution.

TBENCH_RANDSEED (client): Seed for the random number generator that generates
interarrival times.

TBENCH_CLIENT_THREADS (client, networked + loopback): The number of client
threads generating requests. The total request rate is still controlled by
TBENCH_QPS; this parameter is useful if a single client thread is overwhelmed
and is not able to meet the desired QPS.

TBENCH_SERVER (client, networked + loopback): The URL or IP address of the
server. Defaults to localhost.

TBENCH_SERVER_PORT (client, networked + loopback): The TCP/IP port used by the
server. Defaults to 8080.

** OUTPUT **

At the end of the run, each liblat client publishes a lats.bin file, which
includes a <service time, end-to-end time> tuple for each request submitted by
the client. Note that the tuples are not guaranteed to be in the order the
requests were submitted, and therefore cannot be used to generate a time series
for request latencies. The lats.bin file contains binary data, and can be parsed
using the utilities/parselats.py script.

Building and running
====================
Please see BUILD-INSTRUCTIONS for instructions on how to build and execute
TailBench applications.

Note on SPECjbb
===============
Since SPECjbb is not freely available, we do not include it in the TailBench
distribution. Instead, the specjbb directory contains a patch file
(tailbench.patch) that can be applied to a pristine copy of SPECjbb2005 to
obtain the version used in the TailBench paper.

MISC NOTES
==========

We recommend running latency-sensitive applicationwith real time priority (using
chrt, for instance), to avoid interference from background daemons etc. It is
also advisable to disable deep sleep states, and limit the system to shallow
sleep states like C1/C1E.


---

**TIPs:**
The followings are the bugs I found in [initialial version of Tailbench](http://tailbench.csail.mit.edu/tailbench-v0.9.tgz).

- `xapian_networked_server: tbench_server_networked.cpp:252: virtual void NetworkedServer::sendResp(int, const void*, size_t): Assertion 'sent == totalLen' failed.`:
	this is because `server` still send response to `client` even after the TCP connection between `server` and `client` has been closed. So we can fix this bug by adding a variable `fin_flag` so that the server can perceive the status of the TCP connection.
