<!-- README.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 六 8月  5 12:10:59 2017 (+0800)
;; Last-Updated: 六 8月  5 12:15:21 2017 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
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


<!-- README.md ends here -->
