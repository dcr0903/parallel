#include"cuda_runtime.h"
#include"device_functions.h"
#include"device_launch_parameters.h"
#include<stdio.h>
#include<iostream>
using namespace std;

#define N 2000
#define BLOCK_SIZE 1024
#define isDisplay 0

extern "C" float* paraCuda();

int main() {

	float* res = paraCuda();
if(isDisplay)
for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << res[i * N + j] << " ";
		}
		cout << endl;
	}
}