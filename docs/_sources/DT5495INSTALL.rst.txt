.. DT5495INSTALL.rst --- 
.. 
.. Description: 
.. Author: Hongyi Wu(吴鸿毅)
.. Email: wuhongyi@qq.com 
.. Created: 日 7月  5 19:33:34 2020 (+0800)
.. Last-Updated: 四 7月 29 21:51:43 2021 (+0800)
..           By: Hongyi Wu(吴鸿毅)
..     Update #: 7
.. URL: http://wuhongyi.cn 

##################################################
网页控制界面的安装
##################################################


============================================================
PLULib 驱动安装
============================================================

**PLULib 依赖 gcc 5.0 及以上的版本。CAEN 给我们提供了驱动源代码，因此使用我们的程序包则不需要安装以下 PLULib 驱动。**

.. code:: bash

   ### 使用 吴鸿毅 提供的网页控制程序，请不要安装以下驱动！！！！！！
   tar -zxvf CAEN_PLULib-1.1.0-Build20190924.tgz
   cd CAEN_PLU-1.1
   sh install_x64
   cd ..
	  

============================================================
CentOS 7
============================================================

----------------------------------------------------------------------
apache
----------------------------------------------------------------------


**CentOS 7(Scientific Linux 7) 配置文件路径**

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


----------------------------------------------------------------------
apache 无法启动
----------------------------------------------------------------------

检查log文件 **/var/log/httpd/error_log**


如果有以下内容，则说明由于SSL证书过期导致无法正常启动

.. code:: bash

  [Mon Feb 15 18:44:20.458085 2021] [:error] [pid 1482] SSL Library Error: -8181 Certificate has expired
  [Mon Feb 15 18:44:20.458106 2021] [:error] [pid 1482] Unable to verify certificate 'Server-Cert'. Add "NSSEnforceValidCerts off" to nss.conf so the server can start until the problem can be resolved.

也可通过以下命令来检查证书是否过期

.. code:: bash

   certutil -d /etc/httpd/alias -L -n Server-Cert
   
简单的处理方法是先设置禁止检查证书，待更新证书后再取消此设置，操作方法：在 **/etc/httpd/conf.d/nss.conf** 中加入 **NSSEnforceValidCerts off** 此行设置
   


----------------------------------------------------------------------
firewalld
----------------------------------------------------------------------


.. code:: bash
	  
   # 取消firewalld的锁定
   systemctl unmask firewalld 
   ## 使用systemctl start firewalld命令开启防火墙的时候，却开不成功，出现Failed to start firewalld.service: Unit is masked的错误，是firewalld服务被锁定了


.. code:: bash
	  
   # 查看firewalld状态
   systemctl status firewalld


.. code:: bash
	  
   # 开启防火墙
   systemctl start firewalld
   ##没有任何提示即开启成功


.. code:: bash
	  
   # 关闭防火墙
   systemctl stop firewalld


.. code:: bash
	  
   # 重启防火墙
   firewall-cmd --reload



.. code:: bash
	  
   # 查询TCP/UDP的80端口占用情况
   firewall-cmd --query-port=80/tcp
   firewall-cmd --query-port=80/udp
   ## 如果返回结果为“no”，则表示该端口尚未开放


.. code:: bash
	  
   # 永久开放TCP/UDP的端口
   firewall-cmd --permanent --zone=public --add-port=3306/tcp
   firewall-cmd --permanent --zone=public --add-port=80/tcp
   firewall-cmd --permanent --zone=public --add-port=80/udp
    
   ## 提示FirewallD is not running，则防火墙没开启



.. code:: bash
	  
   # 启动 Apache
   systemctl start httpd
    
   # 重启http服务
   systemctl restart httpd
    
   ##查询状态
   systemctl status httpd
    
   ##  默认监听80端口 /var/www/html


============================================================
Ubuntu 20.04
============================================================


**Ubuntu 20.04 配置文件路径**

等待补充。。。
    



   

.. 
.. DT5495INSTALL.rst ends here
