# Ubuntu Server 20.04 + Anaconda3 + CUDA11.2 + CUDNN8.1 + Tensorflow GPU 2.6.0

## Ubuntu Server 20.04 Installation
### sudo apt upgrade
### sudo apt update

## Anaconda3 Virtual Enviroment Installation
### 
### sudo bash Anaconda3-2021.05-Linux-x86_64.sh
### vim ~/.bashrc
### export PATH=$PATH:/usr/local/anaconda3/bin/
### conda create -n tensorflow python=3.7
### conda activate tensorflow
### pip3 install tensorflow_gpu-2.6.0-cp37-cp37m-manylinux2010_x86_64.whl

## NVIDIA安装
### sudo apt install gcc
### sudo apt install cmake
### sudo bash NVIDIA-Linux-x86_64-460.73.01.run
