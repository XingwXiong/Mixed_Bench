FROM centos

MAINTAINER "Xingwang Xiong" "xingw.xiong@gmail.com"

WORKDIR /root

RUN yum install -y  epel-release python gcc gcc-c++ automake autoconf libtool make zlib-devel libuuid-devel e2fsprogs-devel \
                    redis openssl-devel openssh-server python-devel net-tools opencv \ 
                    bison swig boost-devel readline-devel libgtop2-devel jemalloc-devel libdb-cxx-devel git wget \ 
    && yum install -y python-pip \
    && yum clean all \
    && pip install --upgrade pip \
    && pip install redis psutil numpy scipy \
    && mkdir -p /var/run/sshd  /root/.ssh \
    && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
    && sed -i 's/session\s*required\s*pam_loginuid.so/session optional pam_loginuid.so/g' /etc/pam.d/sshd \
    && git clone https://github.com/gperftools/gperftools && cd gperftools && ./autogen.sh && ./configure && make && make install && cd .. && rm -rf gperftools

# please ensure you have run `cp ~/.ssh/id_rsa.pub authorized_keys`
COPY authorized_keys /root/.ssh/authorized_keys

COPY system_tz.py /root/

ADD apache-ant-1.9.9-bin.tar.gz /opt
ADD jdk-8u171-linux-x64.tar.gz /opt
RUN ln -sf ./apache-ant-1.9.9 /opt/ant \
    && ln -sf ./jdk1.8.0_171 /opt/jdk

# RUN echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> /root/.bashrc && source /root/.bashrc
ENV LD_LIBRARY_PATH  $LD_LIBRARY_PATH:/usr/local/lib
ENV JAVA_HOME /opt/jdk
ENV JRE_HOME $JAVA_HOME/jre
ENV ANT_HOME /opt/ant
ENV CLASSPATH .:$JAVA_HOME/lib:$JRE_HOME/lib
ENV PATH $JAVA_HOME/bin:$ANT_HOME/bin:/usr/local/bin/:$PATH


EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
