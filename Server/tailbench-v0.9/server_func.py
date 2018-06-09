#!/usr/bin/env python

import os, sys

TAIL_ROOT="/root/tailbench-v0.9"

server_list=("cd %s/xapian; ./run_xapian_server.sh  " % TAIL_ROOT,
    "cd %s/img-dnn; ./run_img-dnn_server.sh " % TAIL_ROOT,
    "cd %s/masstree; ./run_masstree_server.sh " % TAIL_ROOT,
    "cd %s/shore; ./run_shore_server.sh " % TAIL_ROOT,
    "cd %s/sphinx; ./run_sphinx_server.sh " % TAIL_ROOT,
    "cd %s/silo; ./run_silo_server.sh " % TAIL_ROOT,
    "cd %s/specjbb; ./run_specjbb_server.sh " % TAIL_ROOT,
    "cd %s/moses; ./run_moses_server.sh " % TAIL_ROOT)


if __name__=="__main__":
    for item in sys.argv[1:]:
        print server_list[int(item)]
        os.system(server_list[int(item)])
	os.system("echo '=================='")
    print "=================="
