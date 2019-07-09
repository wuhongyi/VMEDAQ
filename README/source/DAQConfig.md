<!-- DAQConfig.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 12月  3 11:03:17 2018 (+0800)
;; Last-Updated: 一 12月  3 11:03:53 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 2
;; URL: http://wuhongyi.cn -->

# DAQConfig

修改bbcaenvme文件夹下babies、init文件夹内文件

## babies/bbmodules.h

修改ADCADDR、MADCADDR、V1190ADDR、SCAADDR使之与硬件地址匹配（可以多余设置，不可少设置）。其它不要修改。

## babies/start.c

根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型busy代码。其它不要修改。

## babies/evt.c

根据文件内提示设置。其它不要修改。

## babies/clear.c

根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型busy代码。其它不要修改。

## babies/stop.c

根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型busy代码。其它不要修改。

## init/daqinitrc.sh

修改该文件内对应脚本，使之与获取插件对应，用来初始化插件。

**重点是修改 cblt.hh 文件，对启用的插件设置CBLT ADDR 为0xbb，其中MADC还得设置MCST ADDR为0xdd。还得设置每一个插件在CBLT中的顺序，first、mid、last。至少得两个插件才能组成CBLT**





<!-- DAQConfig.md ends here -->
