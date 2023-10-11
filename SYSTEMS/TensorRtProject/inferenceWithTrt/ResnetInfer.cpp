
#include <fstream>
#include <iostream>
#include <filesystem>
#include <map>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <cassert>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "NvInfer.h"
#include "cuda_runtime_api.h"

#include "util.h"

using namespace std;

#define CHECK(status) \
    do\
    {\
        auto ret = (status);\
        if (ret != 0)\
        {\
            std::cerr << "Cuda failure: " << ret << std::endl;\
            abort();\
        }\
    } while (0)

// stuff we know about the network and the input/output blobs

static const int kIMAGE_SIZE = 256;
static const int kIMAGE_SIZE_CROP = 224;
static const int OUTPUT_SIZE = 2;

const char* INPUT_BLOB_NAME = "input";
const char* OUTPUT_BLOB_NAME = "output";

const std::vector<float> mean{ 0.485f, 0.456f, 0.406f };
const std::vector<float> stddev{ 0.229f, 0.224f, 0.225f };

using namespace nvinfer1;

//static Logger gLogger;

//构建Logger
class Logger : public ILogger
{
	void log(Severity severity, const char* msg) noexcept override
	{
		// suppress info-level messages
		if (severity <= Severity::kWARNING)
			std::cout << msg << std::endl;
	}
} gLogger;

void doInference(IExecutionContext& context, float* input, float* output, int batchSize)
{
	const ICudaEngine& mEngine = context.getEngine();

	cudaStream_t stream;
	cudaStreamCreate(&stream);

	// set input
	auto input_idx = mEngine.getBindingIndex("input");
	auto input_dims = nvinfer1::Dims4{ 1, 3 , 224, 224 };
	context.setBindingDimensions(input_idx, input_dims);
	auto input_size = util::getMemorySize(input_dims, sizeof(float));
	void* input_mem{ nullptr };
	cudaMalloc(&input_mem, input_size);

	// set output
	auto output_idx = mEngine.getBindingIndex("output");
	auto output_dims = context.getBindingDimensions(output_idx);
	auto output_size = util::getMemorySize(output_dims, sizeof(float));
	void* output_mem{ nullptr };
	cudaMalloc(&output_mem, output_size);

	cudaMemcpyAsync(input_mem, input, input_size, cudaMemcpyHostToDevice, stream);
	void* bindings[] = { input_mem, output_mem };
	bool status = context.enqueueV2(bindings, stream, nullptr);

	cudaMemcpyAsync(output, output_mem, output_size, cudaMemcpyDeviceToHost, stream);
	cudaStreamSynchronize(stream);

	cudaFree(input_mem);
	cudaFree(output_mem);
}



//加工图片变成拥有batch的输入， tensorrt输入需要的格式，为一个维度
void ProcessImage(const std::string& img_path, float input_data[]) {
	cv::Mat image = cv::imread(img_path); 
	if (image.empty() || !image.data) {
		std::cout << "read image failed! " << img_path << std::endl;
	}
	cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

	// scale image to fit
	int target_h, target_w;
	if (image.rows >= image.cols) {
		target_h = kIMAGE_SIZE;
		target_w = static_cast<int>((kIMAGE_SIZE * 1.0 / image.rows) * image.cols);
	}
	else {
		target_w = kIMAGE_SIZE;
		target_h = static_cast<int>((kIMAGE_SIZE * 1.0 / image.cols) * image.rows);
	}
	cv::Size scale(target_w, target_h);
	cv::resize(image, image, scale);

	// crop image
	int w_start = (image.cols - kIMAGE_SIZE_CROP) / 2;
	int w_end = kIMAGE_SIZE_CROP + w_start;
	int h_start = (image.rows - kIMAGE_SIZE_CROP) / 2;
	int h_end = h_start + kIMAGE_SIZE_CROP;
	image = image.colRange(w_start, w_end).rowRange(h_start, h_end);

	// set data to float array
	int count = 0, tnum1 = image.rows * image.cols, tnum2 = 2 * tnum1;
	for (int i = 0; i < kIMAGE_SIZE_CROP; i++)
	{
		uchar *uc_pixel = image.data + i * image.step;
		for (int j = 0; j < kIMAGE_SIZE_CROP; j++)
		{
			input_data[count] = (uc_pixel[0] / 255. - mean[0]) / stddev[0];
			input_data[count + tnum1] = (uc_pixel[1] / 255. - mean[1]) / stddev[1];
			input_data[count + tnum2] = (uc_pixel[2] / 255. - mean[2]) / stddev[2];

			uc_pixel += 3;
			count++;
		}
	}

	/*
	int count = 0;
	int tnum1 = kIMAGE_SIZE_CROP * kIMAGE_SIZE_CROP;
	int tnum2 = 2 * tnum1;
	for (int i = 0; i < kIMAGE_SIZE_CROP; i++)
	{
		const uchar* rptr = image.ptr<uchar>(i);
		for (int j = 0; j < kIMAGE_SIZE_CROP; j++) 
		{
			input_data[count] = (*(rptr + 3 * j + 0) / 255. - mean[0]) / stddev[0];
			input_data[count + tnum1] = (*(rptr + 3 * j + 1) / 255. - mean[1]) / stddev[1];
			input_data[count + tnum2] = (*(rptr + 3 * j + 2) / 255. - mean[2]) / stddev[2];
		}
	}*/
}

int infer() {

	//加载engine引擎
	char* trtModelStream{ nullptr };
	size_t size{ 0 };
	auto engine_file = "D://models/mobilenet_v3_small_3.trt";
	std::ifstream file(engine_file, std::ios::binary);
	if (file.good()) {
		file.seekg(0, file.end);
		size = file.tellg();
		file.seekg(0, file.beg);
		trtModelStream = new char[size];
		assert(trtModelStream);
		file.read(trtModelStream, size);
		file.close();
	}
	//反序列为engine，创建context

	IRuntime* runtime = createInferRuntime(gLogger);
	assert(runtime != nullptr);
	ICudaEngine* engine = runtime->deserializeCudaEngine(trtModelStream, size, nullptr);
	assert(engine != nullptr);
	IExecutionContext* context = engine->createExecutionContext();
	assert(context != nullptr);
	delete[] trtModelStream;

	//*********************推理*********************//
	const std::vector<float> mean{ 0.485f, 0.456f, 0.406f };
	const std::vector<float> stddev{ 0.229f, 0.224f, 0.225f };
	auto input_dims = nvinfer1::Dims4{ 1, 3 , 224, 224 };

	//   循环推理
	float time_read_img = 0.0;
	float time_infer = 0.0;
	static float data[3 * kIMAGE_SIZE_CROP * kIMAGE_SIZE_CROP];
	static float prob[OUTPUT_SIZE];
	int total_ct = 0, bad_ct = 0;
	int target_cls_id = 0;
	for (const auto & folder : std::vector<std::string>({ "D:/pictures/wydm/svm/val/0", "D:/pictures/wydm/svm/val/1" })) {
		for (const auto & entry : std::filesystem::directory_iterator(folder)) {
			std::cout << "Processing image: " << entry.path() << std::endl;

			// 处理图片为固定输出
			auto start = std::chrono::system_clock::now();  //时间函数
			std::string img_path = entry.path().u8string();
			ProcessImage(img_path, data);

			/*
			std::string input_filename = img_path;
			auto input_image{ util::RGBImageReader(input_filename, input_dims, mean, stddev) };
			input_image.read();
			auto input_buffer = input_image.process();
			*/
			auto end = std::chrono::system_clock::now();
			time_read_img = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() + time_read_img;

			//Run inference
			start = std::chrono::system_clock::now();  //时间函数
			doInference(*context, data, prob, 1);
			//doInference(*context, input_buffer.get(), prob, 1);
			end = std::chrono::system_clock::now();
			time_infer = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() + time_infer;
			std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

			//输出后处理

			//std::cout <<"prob="<<prob << std::endl;
			float cls_float = prob[0];
			int cls_id = 0;
			for (int i = 0; i < OUTPUT_SIZE; i++) {
				float tmp = prob[i];
				if (cls_float < tmp) {
					cls_float = tmp;
					cls_id = i;
				}
			}

			total_ct++;
			if (cls_id != target_cls_id) {
				bad_ct++;
			}
			std::cout << "cls_id=" << cls_id << "\t cls_float=" << cls_float << std::endl;

		}
		target_cls_id++;
	}

	std::cout << "total ct: " << total_ct << ", bad_ct: " << bad_ct << std::endl;
	std::cout << "C++2engine" << "mean read img time =" << time_read_img / total_ct << "ms\t" << "mean infer img time =" << time_infer / total_ct << "ms" << std::endl;
	// Destroy the engine
	context->destroy();
	engine->destroy();
	runtime->destroy();
	return 0;
}

// 测试ok，fp16和fp32都可以正常运行 20231011
int main(int argc, char** argv)
{

	infer();

	return 0;
}

