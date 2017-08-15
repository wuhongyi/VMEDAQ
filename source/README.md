<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 8月  5 12:10:59 2017 (+0800)
;; Last-Updated: 二 8月 15 21:18:24 2017 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 5
;; URL: http://wuhongyi.cn -->

# README

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
autoinstallbabirl.sh
afterinstallbabirl.sh
```


V2718PCB板上DIP开关：Prog: 0 off, 1 off, 2 off, 3 on, 4 off, I/O NIM



运行babicon(安装后第一次需输入以下初始化)

```bash
seteflist 10 add localhost localhost
sethdlist 0 path /home/wuhongyi/data    #这里为数据存储路径
setclinfo 0 add localhost
setclinfo 0 id  0
```


<!-- README.md ends here -->
