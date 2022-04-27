.. httponline.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 一 12月  3 11:10:13 2018 (+0800)
.. Last-Updated: 二 1月  4 11:17:42 2022 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 3
.. URL: http://wuhongyi.cn 

=================================
httponline
=================================

基于网页的在线监视。

仅仅需要修改插件定义即可，无需修改其它代码。

修改文件 **UserDefine.hh** ，按照提示修改即可。 

.. code-block:: cpp

   // V7XX
   #define V7XX_CRATE //没有该插件则注释本行
   #define V7XX_CRATE_NUM 3  //插件数
   const int V7XX_CRATE_GEO[V7XX_CRATE_NUM] = {0,1,2};
   #define V7XX_HIST_BIN 4200
   #define V7XX_HIST_MIN 0
   #define V7XX_HIST_MAX 4200
    
   // MADC
   #define MADC_CRATE //没有该插件则注释本行
   #define MADC_CRATE_NUM 2  //插件数
   const int MADC_CRATE_GEO[MADC_CRATE_NUM] = {10,11};
   #define MADC_HIST_BIN 2000
   #define MADC_HIST_MIN 0
   #define MADC_HIST_MAX 8000
    
   // V1190
   #define V1190_CRATE //没有该插件则注释本行
   #define V1190_CRATE_NUM 1  //插件数
   const int V1190_CRATE_GEO[V1190_CRATE_NUM] = {20};
   #define V1190_HIST_BIN 5000
   #define V1190_HIST_MIN 0
   #define V1190_HIST_MAX 50000
    
   // V1290
   #define V1290_CRATE //没有该插件则注释本行
   #define V1290_CRATE_NUM 1  //插件数
   const int V1290_CRATE_GEO[V1290_CRATE_NUM] = {21};
   #define V1290_HIST_BIN 5000
   #define V1290_HIST_MIN 0
   #define V1290_HIST_MAX 500000


修改文件 **UserDefine.hh** 之后，按照以下指令进行编译


.. code-block:: bash

   make clean
   make
   # 编译成功会在目录中生成可执行文件 online

   # 开启 http online 后台进程
   ./online
   # 默认端口 8888，可通过 ip:8888 访问
   # 访问网页需要用户名/密码，其中 用户名 admin 密码 admin，拥有最高权限；用户名 guest 没有密码。仅可以查看
   
..
.. httponline.rst ends here 
