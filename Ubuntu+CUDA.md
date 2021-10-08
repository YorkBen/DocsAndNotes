# Ubuntu Server 20.04 + Anaconda3 + CUDA11.2 + CUDNN8.1 + Tensorflow GPU 2.6.0

## Ubuntu Server 20.04 Installation
1. sudo apt upgrade
2. sudo apt update


## NVIDIA + CUDA + CUDNN
#### 安装NVIDIA
1. https://www.nvidia.com/Download/index.aspx?lang=en-us 下载对应的安装包
2. sudo apt install gcc
3. sudo apt install cmake
4. sudo bash NVIDIA-Linux-x86_64-460.73.01.run
#### 屏蔽Ubuntu 显卡驱动
6. sudo vim /etc/modprobe.d/blacklist-nouveau.conf，添加：  
- blacklist nouveau
- options nouveau modeset=0
8. sudo update-initramfs -u
9. sudo reboot
#### 测试NVIDIA安装成功
9. nvidia-smi
#### 安装CUDA
11. https://developer.nvidia.com/cuda-toolkit-archive 下载对应的安装包
12. sudo bash ./cuda_11.2.1_460.32.03_linux.run
13. vim ~/.bashrc，加入
- export CUDA_HOME=/usr/local/cuda
- export PATH=$CUDA_HOME/bin/:$PATH
- export LD_LIBRARY_PATH=$CUDA_HOME/lib64/:$LD_LIBRARY_PATH
#### CUDA测试
14. nvcc -V
15. cd ~/NVIDIA_CUDA-11.2_Samples
16. make
17. cd bin/x86_64/linux/release
18. ./deviceQuery
#### CUDNN安装

## Anaconda3 Virtual Enviroment Installation 
#### Anaconda3（python3.7）
1. sudo bash Anaconda3-2021.05-Linux-x86_64.sh
2. vim ~/.bashrc
3. export PATH=$PATH:/usr/local/anaconda3/bin/
4. conda create -n tensorflow python=3.7
5. conda activate tensorflow

#### tensorflow-gpu-2.6.0-cp37 安装和测试
7. wget https://files.pythonhosted.org/packages/3a/d1/10b080c1925cf8f25775e90860d85fd758ab8404fdb546f88169acffb693/tensorflow_gpu-2.6.0-cp37-cp37m-manylinux2010_x86_64.whl ./
8. pip3 install tensorflow_gpu-2.6.0-cp37-cp37m-manylinux2010_x86_64.whl
9. python
10. import tensorflow as tf
11. tf.config.list_physical_devices('GPU')

