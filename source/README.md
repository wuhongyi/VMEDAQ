<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 8月  5 12:10:59 2017 (+0800)
;; Last-Updated: 五 9月  1 10:41:49 2017 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 11
;; URL: http://wuhongyi.cn -->

# README

```text
V2718 FW CONET2 Compliant 	2.14_1.5 	April, 2014

v1190  

MADC32  0224

```


```bash
# A2818Drv-1.20-build20161118.tgz
#将该文件夹复制到 /opt 并安装在该位置
cp -r A2818Drv-1.20 /opt
cd /opt/A2818Drv-1.20
cp ./Makefile.2.6-3.x Makefile
make
#设置开机自动执行该脚本
#   emacs /etc/rc.d/rc.local
#添加 /bin/sh /opt/A2818Drv-1.20/a2818_load
```


babirl自动化安装方法

```bash
#会自动添加环境变量
autoinstallbabirl.sh
afterinstallbabirl.sh
```


V2718PCB板上DIP开关：Prog: 0 off, 1 off, 2 off, 3 on, 4 off, I/O NIM



运行babicon(安装后第一次需输入以下初始化)

```bash
seteflist 10 add localhost localhost
sethdlist 0 path /home/wuhongyi/data    #这里为数据存储路径
setclinfo 0 add localhost  #localhost为本机器
setclinfo 0 id  0

#如果设置给远程电脑
setclinfo 0 add [ip]   #[ip] 为接收端电脑IP 
setclinfo 0 id  0
```

将共享数据发送到Online电脑，需要做以下设置或者关闭防火墙

对Scientific Linux 6，终端ROOT权限下输入**setup**，选择**防火墙配置**，去掉**启用**。  
对cientific Linux 7，ROOT权限下终端输入以下信息关闭firewall  
```bash
systemctl stop firewalld.service #停止firewall
systemctl disable firewalld.service #禁止firewall开机启动
firewall-cmd --state #查看默认防火墙状态（关闭后显示notrunning，开启后显示running）
```

如果机器不联网，可以不需要开启以下iptables防火墙，反正不会被黑

```bash
#在 /etc/sysconfig/iptables 添加以下一行（不能放到最后一行，其中IP替换为发送DAQ电脑的IP）
-A INPUT -p udp -m state --state NEW -m udp --dport 17500:17510 -s 222.29.111.201 -j ACCEPT
```

之后在ROOT权限下执行以下命令

```bash
systemctl restart iptables.service #最后重启防火墙使配置生效
systemctl enable iptables.service #设置防火墙开机启动
```



<!-- README.md ends here -->
