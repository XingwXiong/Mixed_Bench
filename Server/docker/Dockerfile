FROM centos

MAINTAINER "Xingwang Xiong" "xingw.xiong@gmail.com"

#yum install -y epel-release python python-pip gcc redis openssh-server python-devel \
RUN yum provides '*/applydeltarpm' \
    && yum install -y epel-release deltarpm python gcc redis openssh-server python-devel \
    && yum install -y python-pip \
    && yum clean all \
    && pip install --upgrade pip \
    && pip install redis psutil \
    && mkdir -p /var/run/sshd  /root/.ssh \
    && sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
    && sed -i 's/session\s*required\s*pam_loginuid.so/session optional pam_loginuid.so/g' /etc/pam.d/sshd

# please ensure you have run `cp ~/.ssh/id_rsa.pub authorized_keys`
COPY authorized_keys /root/.ssh/authorized_keys

COPY system_tz.py /root

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
