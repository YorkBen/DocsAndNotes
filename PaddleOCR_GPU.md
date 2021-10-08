## PaddleOCR Installation Steps

## 安装docker && docker-nvidia
https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#docker

## 安装paddle-paddle container
1. cd /home/Projects
2. mkdir paddle
3. cd paddle
4. sudo nvidia-docker run --name ppocr -v $PWD:/paddle --shm-size=128G --network=host -it paddlepaddle/paddle:2.1.3-cuda11.2-cudnn8-gcc82 /bin/bash
5. #sudo nvidia-docker run --name paddle -it -v $PWD:/paddle registry.baidubce.com/paddlepaddle/paddle:2.1.3-gpu-cuda11.2-cudnn8 /bin/bash
6. sudo docker container exec -it ppocr /bin/bash（再次进入时执行）

## 克隆PaddleOCR repo代码 && 安装第三方库
7. git clone https://github.com/PaddlePaddle/PaddleOCR
8. cd PaddleOCR
9. pip3 install -r requirements.txt

