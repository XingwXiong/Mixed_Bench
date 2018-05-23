
APP=redis-server
BASE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

DIRS="7000 7001 7002 7003 7004 7005"

${BASE}/clean.sh

for dir in $DIRS
do
    cd ${BASE}/${dir}
    ${APP} ${BASE}/${dir}/redis.conf &
done

${BASE}/redis-trib.rb create --replicas 1 127.0.0.1:7000 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7005
