/** $lic$
 * Copyright (C) 2016-2017 by Massachusetts Institute of Technology
 *
 * This file is part of TailBench.
 *
 * If you use this software in your research, we request that you reference the
 * TaiBench paper ("TailBench: A Benchmark Suite and Evaluation Methodology for
 * Latency-Critical Applications", Kasture and Sanchez, IISWC-2016) as the
 * source in any publications that use this software, and that you send us a
 * citation of your work.
 *
 * TailBench is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 */

#include "client.h"
#include "helpers.h"
#include "tbench_client.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
extern std::string name;
/*******************************************************************************
 * Client
 *******************************************************************************/

Client::Client(int _nthreads) {
    status = INIT;

    nthreads = _nthreads;
    pthread_mutex_init(&lock, nullptr);
    pthread_barrier_init(&barrier, nullptr, nthreads);
    
    minSleepNs = getOpt("TBENCH_MINSLEEPNS", 0);
    seed = getOpt("TBENCH_RANDSEED", 0);
    lambda = getOpt<double>("TBENCH_QPS", 1000.0) * 1e-9;

    dist = nullptr; // Will get initialized in startReq()

    startedReqs = 0;
    
    server_ip = getOpt<std::string>("TBENCH_SERVER", "");
    app_name = getOpt<std::string>("APP_NAME", "");

    redis_ip = getOpt<std::string>("REDIS_IP", "172.17.0.1");
    redis_port = getOpt<int>("REDIS_PORT", 6379);
    redis_auth = getOpt<std::string>("REDIS_AUTH", "xingwxingw");
    redis_ctx = redisConnect(redis_ip.c_str(), redis_port); 
    if (redis_ctx == NULL || redis_ctx->err) {
        if (redis_ctx) {
            fprintf(stderr, "Error: %s\n", redis_ctx->errstr);
        } else {
            printf("Can't allocate redis context\n");
            redisFree(redis_ctx);
            redis_ctx = NULL;
        }
    } else {
        freeReplyObject(redis_reply);
        fprintf(stdout, "redis connection success!\n");
        redis_reply = (redisReply* ) redisCommand(redis_ctx, "auth %s", redis_auth.c_str());
        if(redis_reply->type == REDIS_REPLY_ERROR) {
            fprintf(stderr, "Error: Redis Authentication failure\n");
            freeReplyObject(redis_reply);
        } else fprintf(stdout, "Authentication success!\n");
    }
    tBenchClientInit();
}

Request* Client::startReq() {
    if (status == INIT) {
        pthread_barrier_wait(&barrier); // Wait for all threads to start up

        pthread_mutex_lock(&lock);

        if (!dist) {
            uint64_t curNs = getCurNs();
            dist = new ExpDist(lambda, seed, curNs);

            status = WARMUP;

            pthread_barrier_destroy(&barrier);
            pthread_barrier_init(&barrier, nullptr, nthreads);
        }

        pthread_mutex_unlock(&lock);

        pthread_barrier_wait(&barrier);
    }

    pthread_mutex_lock(&lock);

    Request* req = new Request();
    size_t len = tBenchClientGenReq(&req->data);
    req->len = len;

    req->id = startedReqs++;
    req->genNs = dist->nextArrivalNs();
    inFlightReqs[req->id] = req;

    pthread_mutex_unlock(&lock);

    uint64_t curNs = getCurNs();

    if (curNs < req->genNs) {
        sleepUntil(std::max(req->genNs, curNs + minSleepNs));
    }

    return req;
}

void Client::finiReq(Response* resp) {
    static char val_buf[2000];
    pthread_mutex_lock(&lock);

    auto it = inFlightReqs.find(resp->id);
    assert(it != inFlightReqs.end());
    Request* req = it->second;

    if (status == ROI) {
        uint64_t curNs = getCurNs();

        assert(curNs > req->genNs);

        uint64_t sjrn = curNs - req->genNs;
        assert(sjrn >= resp->svcNs);
        uint64_t qtime = sjrn - resp->svcNs;

        queueTimes.push_back(qtime);
        svcTimes.push_back(resp->svcNs);
        sjrnTimes.push_back(sjrn);
        std::cout << queueTimes.size() << std::endl;
        if(redis_ctx != NULL) {
            redis_key = server_ip + "_" + app_name + "_latency";
            sprintf(val_buf, "(%d,%d,%d)", qtime, resp->svcNs, sjrn);
            printf("%s\n", val_buf);
            redis_reply = (redisReply* ) redisCommand(redis_ctx, "lpush %s %s", redis_key.c_str(), val_buf);
            freeReplyObject(redis_reply);
        }
    }

    delete req;
    inFlightReqs.erase(it);
    pthread_mutex_unlock(&lock);
}

void Client::_startRoi() {
    assert(status == WARMUP);
    status = ROI;

    queueTimes.clear();
    svcTimes.clear();
    sjrnTimes.clear();
}

void Client::startRoi() {
    pthread_mutex_lock(&lock);
    _startRoi();
    pthread_mutex_unlock(&lock);
}

void Client::dumpStats(std::string name) {
    std::string dir=getOpt<std::string>("RESULT_DIR","/home/fanfanda/client/result/");
    //std::string app_name=getOpt<std::string>("APP_NAME","");
    int num=getOpt<int>("NUM",0);
    std::ostringstream stream;
    stream<<num;
    std::ofstream out(dir+app_name+"_"+stream.str()+"_lats.bin", std::ios::out | std::ios::binary);
    int reqs = sjrnTimes.size();

    for (int r = 0; r < reqs; ++r) {
        out.write(reinterpret_cast<const char*>(&queueTimes[r]), 
                    sizeof(queueTimes[r]));
        out.write(reinterpret_cast<const char*>(&svcTimes[r]), 
                    sizeof(svcTimes[r]));
        out.write(reinterpret_cast<const char*>(&sjrnTimes[r]), 
                    sizeof(sjrnTimes[r]));
    }
    out.close();
    if(redis_ctx != NULL) {
        redisFree(redis_ctx);
        //freeReplyObject(redis_reply);
    }
}

/*******************************************************************************
 * Networked Client
 *******************************************************************************/
NetworkedClient::NetworkedClient(int nthreads, std::string serverip, 
        int serverport) : Client(nthreads)
{
    pthread_mutex_init(&sendLock, nullptr);
    pthread_mutex_init(&recvLock, nullptr);

    // Get address info
    int status;
    struct addrinfo hints;
    struct addrinfo* servInfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    std::stringstream portstr;
    portstr << serverport;
    
    const char* serverStr = serverip.size() ? serverip.c_str() : nullptr;

    if ((status = getaddrinfo(serverStr, portstr.str().c_str(), &hints, 
                    &servInfo)) != 0) {
        std::cerr << "getaddrinfo() failed: " << gai_strerror(status) \
            << std::endl;
        exit(-1);
    }

    serverFd = socket(servInfo->ai_family, servInfo->ai_socktype, \
            servInfo->ai_protocol);
    if (serverFd == -1) {
        std::cerr << "socket() failed: " << strerror(errno) << std::endl;
        exit(-1);
    }

    if (connect(serverFd, servInfo->ai_addr, servInfo->ai_addrlen) == -1) {
        std::cerr << "connect() failed: " << strerror(errno) << std::endl;
        exit(-1);
    }

    int nodelay = 1;
    if (setsockopt(serverFd, IPPROTO_TCP, TCP_NODELAY, 
                reinterpret_cast<char*>(&nodelay), sizeof(nodelay)) == -1) {
        std::cerr << "setsockopt(TCP_NODELAY) failed: " << strerror(errno) \
            << std::endl;
        exit(-1);
    }
}

bool NetworkedClient::send(Request* req) {
    pthread_mutex_lock(&sendLock);

    int len = sizeof(Request) - MAX_REQ_BYTES + req->len;
    int sent = sendfull(serverFd, reinterpret_cast<const char*>(req), len, 0);
    if (sent != len) {
        error = strerror(errno);
    }

    pthread_mutex_unlock(&sendLock);

    return (sent == len);
}

bool NetworkedClient::recv(Response* resp) {
    pthread_mutex_lock(&recvLock);

    int len = sizeof(Response) - MAX_RESP_BYTES; // Read request header first
    int recvd = recvfull(serverFd, reinterpret_cast<char*>(resp), len, 0);
    if (recvd != len) {
        error = strerror(errno);
        std::cout << "recvd:" << recvd << std::endl;
        std::cout << "len  :" << len << std::endl;
        std::cout<<"gougoug"<<std::endl;
	    return false;
    }

    if (resp->type == RESPONSE) {
        recvd = recvfull(serverFd, reinterpret_cast<char*>(&resp->data), \
                resp->len, 0);

        if (static_cast<size_t>(recvd) != resp->len) {
            error = strerror(errno);
            return false;
        }
    }

    pthread_mutex_unlock(&recvLock);

    return true;
}

