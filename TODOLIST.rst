.. TODOLIST.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 六 12月 28 21:05:15 2019 (+0800)
.. Last-Updated: 二 9月  7 16:38:09 2021 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 4
.. URL: http://wuhongyi.cn 

##################################################
TODO
##################################################

- 测试新版本软件 CAENUpgrader-1.6.6.tgz
- 测试 MADC 最新固件 0230
- 测试 Vx718 最新固件 2.17



支持ubuntu

babirl/babicon/Makefile

.. code-block:: bash
		
   LOADLIBES = ../lib/libbabirl.a -lreadline -lncurses -lexpat
   LDFLAGS += 



DAQConfig/bbcaenvme/babiescmdme.c

.. code-block:: cpp
		
   typedef struct {
     enum mode md;
     const char mdchar[4];
   }stchkmode;





dmesg | grep a2818
[   28.319411] a2818: loading out-of-tree module taints kernel.
[   28.319436] a2818: module verification failed: signature and/or required key missing - tainting kernel
[   28.319581] a2818: CAEN A2818 CONET controller driver v1.20s
[   28.319582] a2818:   Copyright 2004, CAEN SpA
[   28.319766] a2818: found A2818 adapter at iomem 0xf7100000 irq 0, PLX at 0xf7200000
[   28.331186] a2818:   CAEN A2818 Loaded.
[   28.332049] a2818:   CAEN A2818: 1 device(s) found.


/usr/src/linux-headers-5.4.0-81-generic/arch/x86/Makefile
LINE 222-233 注释掉

ifdef CONFIG_RETPOLINE
  KBUILD_CFLAGS += $(RETPOLINE_CFLAGS)
  # Additionally, avoid generating expensive indirect jumps which
  # are subject to retpolines for small number of switch cases.
  # clang turns off jump table generation by default when under
  # retpoline builds, however, gcc does not for x86. This has
  # only been fixed starting from gcc stable version 8.4.0 and
  # onwards, but not for older ones. See gcc bug #86952.
  ifndef CONFIG_CC_IS_CLANG
    KBUILD_CFLAGS += $(call cc-option,-fno-jump-tables)
  endif
endif





   
  
.. 
.. TODOLIST.rst ends here
