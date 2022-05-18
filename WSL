Win10使用Linux子系统(WSL2)进行深度学习训练: https://blog.csdn.net/Dteam_f/article/details/118390709
https://docs.nvidia.cn/cuda/wsl-user-guide/index.html

【wsl】window子系统ubantu安装cuda 显卡3060
https://blog.csdn.net/ALL_BYA/article/details/123980257


问题解决：
/usr/lib/wsl/lib/libcuda.so.1 is not a symbolic link
原因，/usr/lib/wsl/lib/目录下都是文件而不是链接
又因为该目录只读，因此只能将在其他目录操作，具体步骤如下

cd /usr/lib/wsl
sudo mkdir lib2
sudo ln -s lib/* lib2
之后将文件/etc/ld.so.conf.d/ld.wsl.conf中的 /usr/lib/wsl/lib 改为 /usr/lib/wsl/lib2
