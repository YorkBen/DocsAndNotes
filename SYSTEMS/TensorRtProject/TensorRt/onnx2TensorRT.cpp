// TensorRt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// tensorrt相关
#include <NvInfer.h>

// onnx解析器相关
#include <NvOnnxParser.h>

#include <iostream>
#include <fstream>

/*
// cuda_runtime相关
#include <cuda_runtime.h>
#include <NvInferRuntime.h>
#include <opencv2/opencv.hpp>

// 常用头文件
#include <stdio.h>
#include <math.h>
#include <string>

#include <vector>
#include <memory>
#include <functional>
#include <chrono>
*/


class Logger : public nvinfer1::ILogger
{
	void log(Severity severity, const char* msg) noexcept override
	{
		if (severity <= Severity::kWARNING)
			std::cout << msg << std::endl;
	}
} logger;


void ONNX2TensorRT(const char* ONNX_file, std::string save_engine)
{
	// 创建构建器实例
	nvinfer1::IBuilder* builder = nvinfer1::createInferBuilder(logger);

	// 创建网络定义
	uint32_t flag = 1U << static_cast<uint32_t>(nvinfer1::NetworkDefinitionCreationFlag::kEXPLICIT_BATCH);
	nvinfer1::INetworkDefinition* network = builder->createNetworkV2(flag);

	// 创建一个ONNX解析器来填充网络
	nvonnxparser::IParser* parser = nvonnxparser::createParser(*network, logger);

	// 读取模型文件并处理任何错误
	parser->parseFromFile(ONNX_file, static_cast<int32_t>(nvinfer1::ILogger::Severity::kWARNING));
	for (int32_t i = 0; i < parser->getNbErrors(); ++i)
	{
		std::cout << parser->getError(i)->desc() << std::endl;
	}

	// 5. 创建一个构建配置，指定TensorRT应该如何优化模型
	nvinfer1::IBuilderConfig* config = builder->createBuilderConfig();

	// 6. 设置属性来控制TensorRT如何优化网络
	// 设置内存池的空间
	config->setMemoryPoolLimit(nvinfer1::MemoryPoolType::kWORKSPACE, 16 * (1 << 20));
	// 设置低精度，注释掉FP32
	if (builder->platformHasFastFp16())
	{
		config->setFlag(nvinfer1::BuilderFlag::kFP16);
	}

	// 7. 指定配置后，构建引擎
	nvinfer1::IHostMemory* serializedModel = builder->buildSerializedNetwork(*network, *config);

	// 8. 保存TensorRT模型
	std::ofstream p(save_engine, std::ios::binary);
	p.write(reinterpret_cast<const char*>(serializedModel->data()), serializedModel->size());

	// 9. 序列化引擎包含权重的必要副本，因此不需要解析器、网络定义、构建配置和构建器，可以安全地删除
	delete parser;
	delete network;
	delete config;
	delete builder;

	// 10. 将引擎保存到磁盘，并且可以删除它被序列化到的缓冲区
	delete serializedModel;
}

void exportONNX(const char* ONNX_file, std::string save_engine)
{
	std::ifstream file(ONNX_file, std::ios::binary);
	if (!file.good())
	{
		std::cout << "Load ONNX file failed! No file found from:" << ONNX_file << std::endl;
		return;
	}

	std::cout << "Load ONNX file from: " << ONNX_file << std::endl;
	std::cout << "Starting export ..." << std::endl;

	ONNX2TensorRT(ONNX_file, save_engine);

	std::cout << "Export success, saved as: " << save_engine.c_str() << std::endl;
}

bool build_model(const char* ONNX_file, std::string save_engine) {
	//TRTLogger logger;

	// 下面的builder, config, network是基本需要的组件
	// 形象的理解是你需要一个builder去build这个网络，网络自身有结构，这个结构可以有不同的配置
	nvinfer1::IBuilder* builder = nvinfer1::createInferBuilder(logger);
	// 创建一个构建配置，指定TensorRT应该如何优化模型，tensorRT生成的模型只能在特定配置下运行
	nvinfer1::IBuilderConfig* config = builder->createBuilderConfig();
	// 创建网络定义，其中createNetworkV2(1)表示采用显性batch size，新版tensorRT(>=7.0)时，不建议采用0非显性batch size
	nvinfer1::INetworkDefinition* network = builder->createNetworkV2(1);

	// onnx parser解析器来解析onnx模型
	auto parser = nvonnxparser::createParser(*network, logger);
	if (!parser->parseFromFile(ONNX_file, 1)) {
		std::cout << "Failed to parse: " << ONNX_file << std::endl;
		return false;
	}

	// 设置工作区大小
	printf("Workspace Size = %.2f MB\n", (1 << 28) / 1024.0f / 1024.0f);
	config->setMaxWorkspaceSize(1 << 28);

	/*
	// 需要通过profile来使得batchsize时动态可变的，这与我们之前导出onnx指定的动态batchsize是对应的
	int maxBatchSize = 8;
	auto profile = builder->createOptimizationProfile();
	auto input_tensor = network->getInput(0);
	auto input_dims = input_tensor->getDimensions();

	// 设置batchsize的最大/最小/最优值
	input_dims.d[0] = 1;
	profile->setDimensions(input_tensor->getName(), nvinfer1::OptProfileSelector::kMIN, input_dims);
	profile->setDimensions(input_tensor->getName(), nvinfer1::OptProfileSelector::kOPT, input_dims);

	input_dims.d[0] = maxBatchSize;
	profile->setDimensions(input_tensor->getName(), nvinfer1::OptProfileSelector::kMAX, input_dims);
	config->addOptimizationProfile(profile);
	*/
	// 开始构建tensorrt模型engine
	nvinfer1::ICudaEngine* engine = builder->buildEngineWithConfig(*network, *config);

	if (engine == nullptr) {
		printf("Build engine failed.\n");
		return false;
	}

	// 将构建好的tensorrt模型engine反序列化（保存成文件）
	nvinfer1::IHostMemory* model_data = engine->serialize();
	//FILE* f = fopen(save_engine.c_str(), "wb");
	//fwrite(model_data->data(), 1, model_data->size(), f);
	//fclose(f);

	std::ofstream p(save_engine, std::ios::binary);
	p.write(reinterpret_cast<const char*>(model_data->data()), model_data->size());

	// 逆序destory掉指针
	model_data->destroy();
	engine->destroy();
	network->destroy();
	config->destroy();
	builder->destroy();

	printf("Build Done.\n");
	return true;
}


int main()
{
	// 输入信息
	const char* ONNX_file = "D:/models/mobilenet_v3_small_20231008_1.onnx";
	std::string save_engine = "D:/models/mobilenet_v3_small_2.trt";

	exportONNX(ONNX_file, save_engine);
	//build_model(ONNX_file, save_engine);

	return 0;
}
