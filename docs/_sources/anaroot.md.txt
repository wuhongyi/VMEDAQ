<!-- anaroot.md --- 
;; 
;; Description: 
;; Author: Hongyi Wu(吴鸿毅)
;; Email: wuhongyi@qq.com 
;; Created: 一 12月  3 11:01:30 2018 (+0800)
;; Last-Updated: 一 12月  3 11:01:56 2018 (+0800)
;;           By: Hongyi Wu(吴鸿毅)
;;     Update #: 1
;; URL: http://wuhongyi.cn -->

# anaroot

如果采用CBLT模式读取数据，则先修改*CBLT.hh*文件，不采用CBLT模式则不用修改。设置好之后，执行该目录下的自动编译、安装脚本 *autoPKU.sh* 即可

```bash
sh autoPKU.sh
```


修改 **CBLT.hh** 文件，其中设置应该与CBLT模式下的插件设置顺序一致。

当前CBLT chain支持v830、v7xx、v1190、v1290、madc五种类型的插件，如下所示：

```bash
#define v830m
#define v7xxm
#define v1190m
#define v1290m
#define madcm
```

**获取中如果没有哪一种类型插件，则需注释掉该类型的定义！！！**

以下*xxxn*为启用插件的数据顺序，从0开始编码，如果五种类型插件都有，则为以下设置：
```bash
#define v830n 0
#define v7xxn 1
#define v1190n 2
#define v1290n 3
#define madcn 4
```

如果只含有v7xx、madc两种类型的插件，则定义如下：

```bash
#define v7xxn 0
#define madcn 1
```

如果只含有v830、v7xx、madc三种类型的插件，则定义如下：

```bash
#define v830n 0
#define v7xxn 1
#define madcn 2
```

以下定义用来指定每种类型插件的个数

```bash
#define v830num
#define v7xxnum
#define v1190num
#define v1290num
#define madcnum
```

以下是v830的其它设置

```bash
#define v830chn 8 // 这里设置830开启路数
#define v830head 1 // 不要修改
#define v830geo 0 // 不要修改
```



<!-- anaroot.md ends here -->
