.. Firmware.md --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 一 12月  3 10:26:33 2018 (+0800)
.. Last-Updated: 六 12月 28 21:43:42 2019 (+0800)
..	     By: Hongyi Wu(吴鸿毅)
..     Update #: 13
.. URL: http://wuhongyi.cn 

=================================
固件要求
=================================

**注意**

- 请确保所使用的所有插件固件版本与以下一致。
- 我们尽可能及时更新保证采用较新/最新的固件。
- 由于新版本软件/固件我们需要经过大量评估测试，用户请不要随意升级我们未推荐的版本。


---------------------------------
当前固件版本
---------------------------------

.. code:: text

   V1718   2.14
   V2718 FW CONET2 Compliant 	2.14_1.5    
    
   A2818  新版的CONET2 1.0  旧版的CONET1 0.8
   A3818   0.5
   v1190   1.1
   MADC32  0224



---------------------------------
查看固件版本
---------------------------------

V1718/V2718/A2818/A3818 查看固件版本采用 CAENUpgraderGUI 程序，V1718/V2718/A2818/A3818/V1x90 升级固件版本同样采用 CAENUpgraderGUI 程序。即在终端中执行

.. code:: bash
	  
   CAENUpgraderGUI


升级固件时候，Browse 选择固件之后会弹出一个警告窗口，提示你 “You have chosen to use a raw binary file”，点击确认，然后点击右下角的 Upgrade。等待升级结束，将会有一个窗口提示你重启。


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
V1718
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

如下图，查看 V1718 的固件版本，点击 *Get Fw Rel* 按钮。

.. image:: /_static/img/V1718GetVersion.png

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：

.. image:: /_static/img/V1718UpdateVersion.png


^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
V2718
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

V2718 上固件包括主板 V2718 及子板上的 A2719。

如下图，查看 V2718 主板的固件版本，点击 *Get Fw Rel* 按钮。

.. image:: /_static/img/V2718GetVersion.png

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。


升级界面如下图所示：

.. image:: /_static/img/V2718UpdateVersion.png


如下图，查看子板 A2719 的固件版本，点击 *Get Fw Rel* 按钮。

.. image:: /_static/img/A2719GetVersion.png

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：

.. image:: /_static/img/A2719UpdateVersion.png

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
A2818
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

如下图，查看 A2818 的固件版本，点击 *Get Fw Rel* 按钮。

.. image:: /_static/img/A2818GetVersion.png

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：

.. image:: /_static/img/A2818UpdateVersion.png

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
A3818
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

如下图，查看 A3818 的固件版本，点击 *Get Fw Rel* 按钮。

.. image:: /_static/img/A3818GetVersion.png

如果该固件版本不是 **当前固件版本** 所列版本，则升级固件。

升级界面如下图所示：

.. image:: /_static/img/A3818UpdateVersion.png

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
V1x90
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- V1190/V1290
	- Firmware Revision Register(Base Address + 0x1026, read only, D16) 
	- This register contains the firmware revision number coded on 8 bit. 

**待补充**

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
MADC32
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- madc32
	- 0x600E firmware_revision 
	
**待补充**


..
.. Firmware.md ends here 
