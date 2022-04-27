.. r2root.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 一 12月  3 11:04:50 2018 (+0800)
.. Last-Updated: 二 1月  4 11:12:58 2022 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 5
.. URL: http://wuhongyi.cn 

=================================
r2root
=================================

仅仅需要修改插件定义即可，无需修改其它代码。

修改文件 **UserDefine.hh** ，按照提示修改即可。 

.. code-block:: cpp
		
   #define ROOTFILEPATH "/home/wuhongyi/data/rootfile/"
   #define RAWFILEPATH "/home/wuhongyi/data/"
   #define RAWFILENAME "data"

   // RAWFILEPATH 原始数据路径
   // RAWFILENAME 原始数据文件名
   // ROOTFILEPATH 生成的 ROOT 文件的存放位置

   
.. code-block:: cpp

   // scaler
   #define v830_r2root //没有该插件则注释本行
   #define v830num 1
   const int v830geo[v830num] = {30};//元素个数必须与 v830num 一致
    
    
   // adc
   #define v785_r2root //没有该插件则注释本行
   #define v785num 1
   const int v785geo[v785num] = {0};//元素个数必须与 v785num 一致
    
    
   // qdc
   #define v792_r2root //没有该插件则注释本行
   #define v792num 1
   const int v792geo[v792num] = {1};//元素个数必须与 v792num 一致
    
    
   // tdc
   #define v775_r2root //没有该插件则注释本行
   #define v775num 1
   const int v775geo[v775num] = {2};//元素个数必须与 v775num 一致
    
    
   // gdc
   #define v1x90_r2root //没有该插件则注释本行
   #define v1x90num 2
   #define v1x90multi 5 //gdc 数组第三维度
   #define v1x90hitmax 3000
   const int v1x90geo[v1x90num] = {20,21};//元素个数必须与 v1x90num 一致
    
    
   // madc
   #define madc32_r2root //没有该插件则注释本行
   #define madc32num 2
   const int madc32geo[madc32num] = {10,11};//元素个数必须与 madc32num 一致



修改文件 **UserDefine.hh** 之后，按照以下指令进行编译


.. code-block:: bash

   make clean
   make
   # 编译成功会在目录中生成可执行文件 convert

   # 数据转换指令
   ./convert [run number]
   # 其中 [run number] 为要转换的文件编号，例如转换编号为 652 的文件命令如下
   ./convert 652


   
..
.. r2root.rst ends here 
