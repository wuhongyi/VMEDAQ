.. DAQConfig.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 一 12月  3 11:03:17 2018 (+0800)
.. Last-Updated: 四 5月 14 16:07:39 2020 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 12
.. URL: http://wuhongyi.cn 

=================================
DAQConfig
=================================

首先修改 **StartDAQ.sh/StopDAQ.sh** 两个文件，将文件内的 *wuhongyi* 替换成当前 LINUX 的用户名。然后修改 bbcaenvme 文件夹下 babies、init 文件夹内文件。

修改文件之前，我们需要先理解硬件地址与 GEO 编号。每个插件模块侧面都有四个拨盘，每个拨盘代表一个 16 进制位，例如，当四个拨盘从左到右分别为 1，2，3，4 时，表示其硬件地址为 0x1234。控制器与模块的通讯依靠该硬件地址来寻址，每个控制器下的模块地址应该具有唯一性。同时，我们可以通过软件对每个模块设置一个 GEO 编号，该模块输出数据中都会带有该 GEO 标记，方便我们对数据进行解码。GEO 编号范围为 0-31。

这里我们对硬件地址设置进行如下约定（当然以下约定不是强制要求，用户可以任意修改），

- v7xx 模块硬件地址从 0x1000 开始，然后 0x1001，有多少个模块就依次往后设置。
- MADC32 硬件地址从 0x2000 开始，然后 0x2001，有多少个模块就依次往后设置。
- v1x90 硬件地址从 0x4000 开始，然后 0x4001，有多少个模块就依次往后设置。  
- v830 模块硬件地址从 0x5000 开始，然后 0x5001，有多少个模块就依次往后设置。

这里我们对 GEO 地址设置进行如下约定（当然以下约定不是强制要求，用户可以任意修改），

- 一般 v830 使用 1-2 个模块而已，因此 GEO 编号 30/31 我们预留给 v830。第一个 v830 的 GEO 为 30，第二个 v830 的 GEO 为 31。
- 实验中一般 v7xx 和 MADC32 模块使用较多，如果每种模块均不超过 10 个的话，我们默认 0-9 预留给 v7xx，10-19 预留给 MADC32。如果某种模块超过 10 个话，那么 v7xx 和 MADC32 的 GEO 按照 0-19 的编号依次往下进行分配。
- 实验中 v1x90 模块的使用数量一般不会超过 5 个，这里我们为 v1x90 预留 20-24，编号从 20 开始依次往后进行分配。 
- 剩余的 GEO 编号 25-29 进行机动使用。

**整个 DAQ 的程序配置中，需要在文件 babies/bbmodules.h 中进行硬件地址设置。然后还需对文件夹 init 内的文件进行硬件地址和 GEO 的设置。**

  
---------------------------------
babies/bbmodules.h
---------------------------------

修改 ADCADDR、MADCADDR、V1x90ADDR、SCAADDR 使之与硬件地址匹配（可以多余设置，不可少设置）。其它不要修改。

这里我们按照之前的约定，V7xx 硬件地址从 0x1000 开始编号，用 ADC[x]ADDR 来表示不同模块。MADC32 硬件地址从 0x2000 开始编号，用 MADC[x]ADDR 来表示不同模块。V1x90 硬件地址从 0x4000 开始编号，用 V1x90ADDR[x] 来表示不同模块。 V830 硬件地址从 0x5000 开始编号，用 SCAADDR[x] 来表示不同模块。（其中 [x] 代表不同的数字）

**如果您依照我们的约定来设置，则不需要修改本文件。**

如果您使用控制器V1718，则需要注释以下代码。否则开启以下代码

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
   
  #define V2718   //如果使用 V1718 则注释本行
   
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



---------------------------------
babies/start.c
---------------------------------

**根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型 busy 代码。其它不要修改。**


**busy 模式**

如果您使用软件 busy 模式时，则开启以下代码行，如果您使用硬件 busy 模式时，则注释掉以下行代码。

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 软件busy
  v2718_init_ioport(4,0,0);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



**V830**

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 V830 插件
  v830_clear_all(SCAADDR0);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

用户需要修改以上代码段，如果您不使用 V830 模块，则注释掉以上区域的代码。

如果您使用一个 V830 模块，则添加代码：

.. code:: cpp

   v830_clear_all(SCAADDR0);

如果您使用两个 V830 模块，则添加代码：

.. code:: cpp

   v830_clear_all(SCAADDR0);   
   v830_clear_all(SCAADDR1);


**V7xx**

.. code:: cpp
	  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 V7xx 插件
  // 每个插件单独设置
  v7xx_rst_counter(ADC0ADDR);
  v7xx_rst_counter(ADC1ADDR);
  v7xx_rst_counter(ADC2ADDR);
  // v7xx_rst_counter(ADC3ADDR);
  // v7xx_rst_counter(ADC4ADDR);
  // v7xx_rst_counter(ADC5ADDR);
  // v7xx_rst_counter(ADC6ADDR);
  // v7xx_rst_counter(ADC7ADDR);
  
  v7xx_clear(ADC0ADDR);
  v7xx_clear(ADC1ADDR);
  v7xx_clear(ADC2ADDR);
  // v7xx_clear(ADC3ADDR);
  // v7xx_clear(ADC4ADDR);
  // v7xx_clear(ADC5ADDR);
  // v7xx_clear(ADC6ADDR);
  // v7xx_clear(ADC7ADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


用户需要修改以上代码段，如果您不使用 V7xx 模块，则注释掉以上区域的代码。

如果您使用一个 V7xx 模块，则添加代码：

.. code:: cpp

   v7xx_rst_counter(ADC0ADDR);
   v7xx_clear(ADC0ADDR);

如果您使用两个 V7xx 模块，则添加代码：

.. code:: cpp

  v7xx_rst_counter(ADC0ADDR);
  v7xx_rst_counter(ADC1ADDR);
  v7xx_clear(ADC0ADDR);
  v7xx_clear(ADC1ADDR);

使用更多 V7xx 则依次类推。


**V1x90**

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 V1190/V1290 插件
  // 每个插件单独clear
  // v1190_clear(V1x90ADDR0);
  // v1290_clear(V1x90ADDR1);

  v1190_clear(V1x90ADDR0);
  v1190_clear(V1x90ADDR1);
  // v1290_clear(V1x90ADDR0);
  // v1290_clear(V1x90ADDR1);
  
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

用户需要修改以上代码段，如果您不使用 V1x90 模块，则注释掉以上区域的代码。

如果您只使用一个 V1190 模块，则添加代码：

.. code:: cpp

   v1190_clear(V1x90ADDR0);

如果您只使用两个 V1190 模块，则添加代码：

.. code:: cpp

  v1190_clear(V1x90ADDR0);
  v1190_clear(V1x90ADDR1);

如果您使用一个 V1190，一个 V1290，则添加代码：

.. code:: cpp

  v1190_clear(V1x90ADDR0);
  v1290_clear(V1x90ADDR1);  
	  
更多模块使用的组合，请以此类推。


  
**MADC32**

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 MADC32 插件
  madc32_mclear(MSTMDCADDR);
  madc32_mirq_level(MSTMDCADDR,0);
  madc32_mreset_ctra_counters(MSTMDCADDR);
  madc32_mfifo_reset(MSTMDCADDR);
  madc32_mstart_acq(MSTMDCADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

用户需要修改以上代码段，如果您不使用 MADC32 模块，则注释掉以上区域的代码。如果您使用了 MADC32 模块，不管使用了多少个模块，只需要开启以上代码即可对所有的模块完成初始化。

**busy 模式**

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  // 硬件busy / 软件busy中的多机箱同步
  // v2718_clear_ioport(3);
  
  // 软件busy
  v2718_pulse_ioport(4);
  
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
如果您是软件 busy 模式，则开启代码：

.. code:: cpp

   v2718_pulse_ioport(4);

如果您是硬件 busy 模式或者软件 busy 模式下的多机箱同步方案下，则需要开启代码：
  
.. code:: cpp

   v2718_clear_ioport(3);

   

---------------------------------
babies/evt.c
---------------------------------

**根据文件内提示设置。其它不要修改。**

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  // 软件BUSY模式下6036->0x1不需要以下清除，6036->0x3需要以下清除，6036->0x0需要以下清除
  // 硬件BUSY模式下只能采用6036->0x3，需要以下清除
  
  // 有 MADC32 插件  
  madc32_mclear(MSTMDCADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
用户需要修改以上代码段，如果您不使用 MADC32 模块，则注释掉以上区域的代码。如果您使用了 MADC32 模块，不管使用了多少个模块，只需要开启以上代码即可对所有的模块完成清除。

当然，在软件 busy 模式下，对每个模块的寄存器进行相应的寄存器配置，可以不用以上清除指令自动进行清除，此时每个事件能够节约 20 us 左右的时间，该方案建议对 DAQ 比较熟悉的用户使用。


---------------------------------
babies/clear.c
---------------------------------

**根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型 busy 代码。其它不要修改。**

如果您使用软件 busy 模式时，则开启以下代码行，如果您使用硬件 busy 模式时，则注释掉以下行代码。

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 软件busy
  v2718_pulse_ioport(4);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	  
---------------------------------
babies/stop.c
---------------------------------

**根据文件内提示设置，有该类型插件则开启对应代码，开启对应类型 busy 代码。其它不要修改。**

**MADC32**

.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 有 MADC32 插件
  madc32_mstop_acq(MSTMDCADDR);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

用户需要修改以上代码段，如果您不使用 MADC32 模块，则注释掉以上区域的代码。如果您使用了 MADC32 模块，不管使用了多少个模块，只需要开启以上代码即可对所有的模块发送结束采集指令。


**busy 模式**

如果您是硬件 busy 模式或者软件 busy 模式下的多机箱同步方案下，则需要开启代码，否则注释掉以下代码：
  
.. code:: cpp

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // 硬件busy / 软件busy多机箱同步
  // v2718_set_ioport(3);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


---------------------------------
cmdvme/cmdvme.c
---------------------------------

如果使用制器器 V1718，则需要修改文件中以下代码。将 *V2718* 改为 *V1718* 即可。

.. code:: cpp

    enum board bd = V2718;
	  
  
---------------------------------
init/daqinitrc.sh
---------------------------------

修改该文件内对应脚本，使之与获取插件对应，用来初始化插件。

**重点是修改 cblt.hh 文件，对启用的插件设置CBLT ADDR 为 0xbb，其中 MADC 还得设置 MCST ADDR 为 0xdd。还得设置每一个插件在 CBLT 中的顺序，first、mid、last。至少得两个插件才能组成CBLT.**

init/daqinitrc.sh 文件包含以下内容：

.. code:: bash

   #!/bin/sh 
    
   /bin/sh ./v830.sh
   /bin/sh ./v7xx_all.sh
   # /bin/sh ./v7xx_thres.sh
   /bin/sh ./v1190_0.sh
   /bin/sh ./v1190_1.sh
   # /bin/sh ./v1290.sh
   /bin/sh ./madcall.sh
   # /bin/sh ./madc_thres.sh
   /bin/sh ./cblt.sh

如果您使用了 V830 则开启以下代码，否则注释掉以下代码：

.. code:: bash
	  
   /bin/sh ./v830.sh


**待补充**


..
.. DAQConfig.rst ends here 
