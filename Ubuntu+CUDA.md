## Ubuntu Server 20.04 + Anaconda3 + CUDA11.2 + CUDNN8.1 + Tensorflow GPU 2.6.0

## Ubuntu Server 20.04 Installation
1. sudo apt update
2. sudo apt upgrade


## NVIDIA + CUDA + CUDNN
#### 屏蔽Ubuntu 显卡驱动
1. sudo vim /etc/modprobe.d/blacklist-nouveau.conf，添加：  
- blacklist nouveau
- options nouveau modeset=0
2. sudo update-initramfs -u
3. sudo reboot
#### 安装NVIDIA
4. Ubuntu查看显卡型号：lspci | grep -i nvidia
4. https://www.nvidia.com/Download/index.aspx?lang=en-us 下载对应的安装包
5. sudo apt install gcc g++ cmake
6. sudo bash NVIDIA-Linux-x86_64-460.73.01.run
#### 测试NVIDIA安装成功
7. nvidia-smi
#### 安装CUDA
11. https://developer.nvidia.com/cuda-toolkit-archive 下载对应的安装包
12. sudo bash ./cuda_11.2.1_460.32.03_linux.run
13. vim ~/.bashrc，加入
- export CUDA_HOME=/usr/local/cuda
- export PATH=$CUDA_HOME/bin/:$PATH
- export LD_LIBRARY_PATH=$CUDA_HOME/lib64/:$LD_LIBRARY_PATH
14. source ~/.bashrc
#### CUDA测试
14. nvcc -V
15. cd ~/NVIDIA_CUDA-11.2_Samples
16. make
17. cd bin/x86_64/linux/release
18. ./deviceQuery
#### CUDNN安装
19. https://developer.nvidia.com/rdp/cudnn-download 下载对应的安装包（需要注册登录）
20. tar -zxvf cudnn-11.2-linux-x64-v8.1.1.33.tgz
21. 将解压得到的cuda 文件夹下的内容拷贝到对应的$CUDA_HOME 下即可


## Anaconda3 Virtual Enviroment Installation 
#### Anaconda3（python3.7）
1. https://www.anaconda.com/products/individual#Downloads 官网下载anacoda安装包
1. sudo bash Anaconda3-2021.05-Linux-x86_64.sh
2. vim ~/.bashrc 加入 export PATH=$PATH:/usr/local/anaconda3/bin/
3. source ~/.bashrc
4. conda create -n tensorflow python=3.7
5. conda activate tensorflow

#### tensorflow-gpu-2.6.0-cp37 安装和测试
7. wget https://files.pythonhosted.org/packages/3a/d1/10b080c1925cf8f25775e90860d85fd758ab8404fdb546f88169acffb693/tensorflow_gpu-2.6.0-cp37-cp37m-manylinux2010_x86_64.whl ./
8. pip3 install tensorflow_gpu-2.6.0-cp37-cp37m-manylinux2010_x86_64.whl
9. python
10. import tensorflow as tf
11. tf.config.list_physical_devices('GPU')

#### pytorch 安装测试
12. 官网地址：https://pytorch.org/get-started/locally/
13. 

###opencv
pip install opencv-python


## 问题记录
1. nvidia-smi无法使用，有可能是系统自动升级内核导致，可以关掉系统升级功能，降低内核，参考：1. https://blog.csdn.net/u012114438/article/details/104425573  2. https://www.cnblogs.com/leeqizhi/p/11469309.html

2. GPU Linux命令：nvtop, gpustat, nvidia-smi   GPU Windows工具：查看进程管理器，nvidia-smi查看进程，kill可以杀掉进程。
3. tensorflow可以使用GPU，torch不行。
4. pytorch运行时出错：NVIDIA GeForce RTX 3060 with CUDA capability sm_86 is not compatible with the current PyTorch installation.
The current PyTorch install supports CUDA capabilities sm_37 sm_50 sm_60 sm_70。算力不匹配问题，
官网安装命令：pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113 ，运行有问题。
需要指定cuda版本安装解决：pip3 install torch==1.11.0+cu113 torchvision==0.12.0+cu113 torchaudio==0.11.0+cu113 -f https://download.pytorch.org/whl/cu113/torch_stable.html
