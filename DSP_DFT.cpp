// DSP_FFT.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <windows.h>
#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

// 线程数
#define NUM_THREADS 4

// DFT长度
#define N 1024

// define the complex data type
typedef std::complex<double> complex;

// define the DFT data type
typedef std::vector<complex> FFTData;
// pi
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// define the thread data structure
struct ThreadData {
    int threadId;
    int startIndex;
    FFTData indata;
    FFTData outdata;
};
// 线程函数
DWORD WINAPI FFTThread(LPVOID lpParam) {
    ThreadData* threadData = (ThreadData*)lpParam;

    int threadId = threadData->threadId;
    int startIndex = threadData->startIndex;
    FFTData& indata = threadData->indata;//指向输入信号
    FFTData& outdata = threadData->outdata;//指向dft结果

    // 计算每个线程的开始和结束索引
    int start = startIndex;
    int end = startIndex + N / NUM_THREADS;

    // perform the FFT calculation on the subset of the data
    for (int k = start; k < end; k++) {
        complex sum(0, 0);

        for (int n = 0; n < N; n++) {
            complex expval = std::exp(complex(0, -2 * M_PI * n * k / N));
            sum += indata[n] * expval;
        }
        outdata[k] = sum;
    }

    return 0;
}

int main() {
    //输入数据
    FFTData inputData(N);
    for (int i = 0; i < N; i++) {
        inputData[i] = complex(std::sin(2000 * M_PI * i / N), 0);
    }
    //将输入信号存入txt方便用matlab绘出
    std::ofstream in_output("input.txt");
    for (const auto& z : inputData) {
        in_output << z.real() << "\n";
    }
    in_output.close();
    // 创建句柄
    HANDLE threadHandles[NUM_THREADS];

    //threadData类型数据结构
    ThreadData threadData[NUM_THREADS];

    // 创建线程
    FFTData outputData(N);
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].threadId = i;
        threadData[i].startIndex = i * N / NUM_THREADS;
        threadData[i].indata = inputData;
        threadData[i].outdata = outputData;
        threadHandles[i] = CreateThread(NULL, 0, FFTThread, &threadData[i], 0, NULL);
    }
    // 等待线程结束
    WaitForMultipleObjects(NUM_THREADS, threadHandles, TRUE, INFINITE);

    for (int i = 0; i < NUM_THREADS; i++) {
        for (int j = 0; j < N; j++) {
            outputData[j] = threadData[i].outdata[j];
        }
    }
     // 在控制台输出fft计算的结果
    for (int i = 0; i < N; i++) {
        std::cout << outputData[i];
    }
    // 打开文件
    std::ofstream output("output.txt");
    // 写文件
    for (const auto& z : outputData) {
        output << z.real() << " " << z.imag() << "\n";
    }
    // 关闭文件
    output.close();
    return 0;
}
