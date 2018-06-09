FROM centos

MAINTAINER "Xingwang Xiong" "xingw.xiong@gmail.com"

WORKDIR /root

ADD apache-ant-1.9.9-bin.tar.gz /opt
ADD jdk-8u171-linux-x64.tar.gz /opt

# please ensure you have run `cp ~/.ssh/id_rsa.pub authorized_keys`
COPY authorized_keys /root/.ssh/authorized_keys

COPY system_tz.py /root/

RUN yum -y update \
    && yum install -y epel-release python gcc gcc-c++ automake autoconf libtool make zlib-devel libuuid-devel e2fsprogs-devel \
                    redis openssl-devel openssh-server passwd python-devel net-tools opencv opencv-devel \ 
                    bison swig boost-devel readline-devel libdb-cxx-devel numactl-devel libaio-devel git wget \ 
    && yum install -y python-pip  libgtop2-devel jemalloc-devel bc expect \
    && yum clean all \
    && pip install --upgrade pip \
    && pip install redis psutil numpy scipy \
    && echo "123456" | passwd --stdin root \
    && mkdir -p /var/run/sshd  /root/.ssh \
    && chmod 700 -R /root/.ssh \
    && ssh-keygen -q -t rsa -f /etc/ssh/ssh_host_rsa_key -N '' \
    && ssh-keygen -q -t ecdsa -f /etc/ssh/ssh_host_ecdsa_key -N '' \
    && ssh-keygen -q -t ed25519 -f /etc/ssh/ssh_host_ed25519_key -N '' \
    && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
    && sed -i 's/session\s*required\s*pam_loginuid.so/session optional pam_loginuid.so/g' /etc/pam.d/sshd \
    && git clone https://github.com/gperftools/gperftools && cd gperftools && ./autogen.sh && ./configure && make && make install && cd .. && rm -rf gperftools \
    && ln -sf ./apache-ant-1.9.9 /opt/ant \
    && ln -sf ./jdk1.8.0_171 /opt/jdk \
    && echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> /root/.bashrc \
    && echo "export JAVA_HOME=/opt/jdk" >> /root/.bashrc \
    && echo "export JRE_HOME=$JAVA_HOME/jre" >> /root/.bashrc \
    && echo "export ANT_HOME=/opt/ant" >> /root/.bashrc \
    && echo "export CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib" >> /root/.bashrc \
    && echo "export PATH=$JAVA_HOME/bin:$ANT_HOME/bin:/usr/local/bin/:$PATH" >> /root/.bashrc \
    && source /root/.bashrc

ENV LD_LIBRARY_PATH  $LD_LIBRARY_PATH:/usr/local/lib
ENV JAVA_HOME /opt/jdk
ENV JRE_HOME $JAVA_HOME/jre
ENV ANT_HOME /opt/ant
ENV CLASSPATH .:$JAVA_HOME/lib:$JRE_HOME/lib
ENV PATH $JAVA_HOME/bin:$ANT_HOME/bin:/usr/local/bin/:$PATH


EXPOSE 22
EXPOSE 10000
EXPOSE 10001
EXPOSE 10002
EXPOSE 10003
EXPOSE 10004
EXPOSE 10005
EXPOSE 10006
EXPOSE 10007

CMD ["/usr/sbin/sshd", "-D"]
