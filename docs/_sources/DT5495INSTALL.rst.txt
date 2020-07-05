.. DT5495INSTALL.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 日 7月  5 19:33:34 2020 (+0800)
.. Last-Updated: 日 7月  5 19:44:24 2020 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 1
.. URL: http://wuhongyi.cn 

##################################################
网页控制界面的安装
##################################################


============================================================
PLULib 驱动安装
============================================================

PLULib 依赖 gcc 5.0 及以上的版本。 

.. code:: bash

   tar -zxvf CAEN_PLULib-1.1.0-Build20190924.tgz
   cd CAEN_PLU-1.1
   sh install_x64
   cd ..
	  

============================================================
apache
============================================================

CentOS 7(Scientific Linux 7) 配置文件路径

.. code:: bash

   /etc/httpd/conf/httpd.conf


**selinux 关闭**
   
.. code:: bash

   getenforce # 查看selinux状态
   setenforce 0 # 临时关闭
   修改/etc/selinux/config 文件，将SELINUX=enforcing改为SELINUX=disabled #永久关闭


**修改 /etc/httpd/conf/httpd.conf**

.. code:: bash

   #
   # "/var/www/cgi-bin" should be changed to whatever your ScriptAliased
   # CGI directory exists, if you have that configured.
   #
   <Directory "/var/www/cgi-bin">
       AllowOverride None
       Options ExecCGI
       Require all granted
   </Directory>


.. code:: bash

    #
    # AddHandler allows you to map certain file extensions to "handlers":
    # actions unrelated to filetype. These can be either built into the server
    # or added with the Action directive (see below)
    #
    # To use CGI scripts outside of ScriptAliased directories:
    # (You will also need to add "ExecCGI" to the "Options" directive.)
    #
    AddHandler cgi-script .cgi .py .sh	  
	  








将文件夹 *cgi-bin* 内文件复制到 */var/www/cgi-bin*，将文件夹 *html* 内文件复制到 */var/www/html*。


**linux 下 apache启动、停止、重启命令**

.. code:: bash

   service httpd start  #启动
   service httpd restart #重新启动
   service httpd stop #停止服务

.. 
.. DT5495INSTALL.rst ends here
