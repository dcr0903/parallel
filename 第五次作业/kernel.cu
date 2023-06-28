#include"cuda_runtime.h"
#include"device_functions.h"
#include"device_launch_parameters.h"
#include<stdio.h>
#include<iostream>

#define N 2000
#define BLOCK_SIZE 1024
float elm[N][N] = { 0.0 };

using namespace std;

__global__ void division_kernel(float* data, int k) {
	int tid = blockDim.x * blockIdx.x + threadIdx.x;//计算线程索引
	while (tid < N) {
		int element = data[k * N + k];
		int temp = data[k * N + tid];
		//请同学们思考，如果分配的总线程数小于 N 应该怎么办？
		data[k * N + tid] = (float)temp / element;
		r++;
		tid += r * blockDim.x;//计算线程索引
	}

	return;
}

__global__ void eliminate_kernel(float* data, int k) {
	int tx = blockDim.x * blockIdx.x + threadIdx.x;
	if (tx == 0)
		data[k * N + k] = 1.0;//对角线元素设为 1
	int row = k + 1 + blockIdx.x;//每个块负责一行
	while (row < N) {
		int tid = threadIdx.x;
		while (k + 1 + tid < N) {
			int col = k + 1 + tid;
			float temp_1 = data[(row * N) + col];
			float temp_2 = data[(row * N) + k];
			float temp_3 = data[k * N + col];
			data[(row * N) + col] = temp_1 - temp_2 * temp_3;
			tid = tid + blockDim.x;
		}
		__syncthreads();//块内同步
		if (threadIdx.x == 0) {
			data[row * N + k] = 0;
		}
		row += gridDim.x;
	}
	return;
}

void display() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << elm[i][j] << " ";
		}
		cout << endl;
	}
}
void init() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			elm[i][j] = 0.0;
		}
	}
	for (int i = 0; i < N; i++) {
		elm[i][i] = 1.0;
	}
	for (int r = 0; r < 5 * N; r++) {
		int i1 = rand() % N;
		int i2 = rand() % N;
		float rate = rand() % 10 / 10.0;;
		if (i1 != i2) {
			for (int j = 0; j < N; j++) {
				elm[i1][j] += rate * elm[i2][j];
			}
		}
	}
	//display();
}




extern "C" float* paraCuda() {
	init();
	float* temp = new float[N * N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			temp[i * N + j] = elm[i][j];
		}
	}
	cudaError_t ret;//用于错误检查，当 CUDA 接口调用成功会返回 cudaSucess
	float* gpudata;
	float* result = new float[N * N];
	int size = N * N * sizeof(float);

	ret = cudaMalloc(&gpudata, size);//分配显存空间
	if (ret != cudaSuccess) {
		printf("cudaMalloc gpudata failed!\n");
	}
	ret = cudaMemcpy(gpudata, temp, size, cudaMemcpyHostToDevice);//将数据传输至 GPU 端
	if (ret != cudaSuccess) {
		printf("cudaMemcpyHostToDevice failed!\n");
	}
	dim3 dimBlock(BLOCK_SIZE, 1);//线程块
	dim3 dimGrid(1, 1);//线程网格
	cudaEvent_t start, stop;//计时器
	float elapsedTime = 0.0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);//开始计时

	for (int k = 0; k < N; k++) {
		division_kernel << <dimGrid, dimBlock >> > (gpudata, k);//负责除法任务的核函数
		cudaDeviceSynchronize();//CPU 与 GPU 之间的同步函数
		ret = cudaGetLastError();
		if (ret != cudaSuccess) {
			printf("division_kernel failed, %s\n", cudaGetErrorString(ret));
		}
		eliminate_kernel << <dimGrid, dimBlock >> > (gpudata, k);//负责消去任务的核函数
		cudaDeviceSynchronize();
		ret = cudaGetLastError();
		if (ret != cudaSuccess) {
			printf("eliminate_kernel failed, %s\n", cudaGetErrorString(ret));
		}
	}


	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);//停止计时
	cudaEventElapsedTime(&elapsedTime, start, stop);
	printf("GPU_LU:%f ms\n", elapsedTime);
	cudaError_t cudaStatus2 = cudaGetLastError();
	if (cudaStatus2 != cudaSuccess) {
		fprintf(stderr, "Kernel launch failed: %s\n", cudaGetErrorString(cudaStatus2));
	}
	ret = cudaMemcpy(result, gpudata, size, cudaMemcpyDeviceToHost);//将数据传回 CPU 端
	if (ret != cudaSuccess) {
		printf("cudaMemcpyDeviceToHost failed!\n");
	}
	cudaFree(gpudata);//释放显存空间，用 CUDA 接口分配的空间必须用 cudaFree 释放
	//销毁计时器
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	return result;
}