BASE_ROOT=/root/
TAIL_ROOT=${BASE_ROOT}/tailbench-v0.9/

RESULT_ROOT=${BASE_ROOT}/result/

# Set this to point to the top level of the TailBench data directory
DATA_ROOT=${BASE_ROOT}/tailbench-data/
# Set this to point to the top level installation directory of the Java
# Development Kit. Only needed for Specjbb
JDK_PATH=/opt/jdk

# This location is used by applications to store scratch data during execution.
SCRATCH_DIR=${BASE_ROOT}/tailbench-scratchData


REDIS_IP="172.17.0.1"
REDIS_PORT=6379
REDIS_AUTH="xingwxingw"
if [[ -z "${RESULT_DIR}" ]]; then RESULT_DIR=/root/result/; fi
