## python环境搭建
1. pip install onnx
2. pip install onnx-simplifier
3. pip install onnxruntime
4. pip install onnxruntime-gpu
5. 安装tensorrt：到tensorrt的下载目录下，找到python 目录，安装本地文件：pip install tensorrt-8.6.1-cp310-none-win_amd64.whl
6. 安装pycuda：conda install -c conda-forge pycuda
7. pip install cuda-python

## 踩坑记录
1. 未安装onnx-simplifier时，onnx模型转trt engine，提示错误：报错：Your ONNX model has been generated with INT64 weights, while TensorRT does not natively support INT64. Attempting to cast down to INT32，参考：https://blog.csdn.net/yyc1820252161/article/details/130219902，python -m onnxsim model.onnx model_sim.onnx
