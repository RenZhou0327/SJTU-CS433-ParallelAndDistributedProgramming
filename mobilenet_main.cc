#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "init_model.cuh"
#include "layers.cuh"

#define INPUTSHAPE 3 * 244 * 244
#define OUTPUTSHAPE 1000
#define TESTNUM 10
#define ITERNUM 500
float inputArr[TESTNUM][INPUTSHAPE];
float benchOutArr[TESTNUM][OUTPUTSHAPE];

void readInput(char *filename)
{
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    for (int i = 0; i < TESTNUM; i++)
        for (int j = 0; j < INPUTSHAPE; j++)
            fscanf(fp, "%f", &inputArr[i][j]);
}

void readOutput(char *filename)
{
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    for (int i = 0; i < TESTNUM; i++)
        for (int j = 0; j < OUTPUTSHAPE; j++)
            fscanf(fp, "%f", &benchOutArr[i][j]);
}

void checkOutput(float *out1, float *out2)
{
    float maxDiff = 0;
    for (int i = 0; i < OUTPUTSHAPE; i++)
    {
        maxDiff = (fabs(out1[i] - out2[i]) > maxDiff) ? fabs(out1[i] - out2[i]) : maxDiff;
    }
    if (maxDiff > 1e-5)
    {
        printf("Output dismatch. MaxDiff is %.7f\n", maxDiff);
    }
}

// TODO: 实现自己的inference
void inference(float *input, float *output);

int main()
{
    
    initModel(); // 读取网络权重
    
    readInput("./mobilenetInput.txt");   // 读取输入
    readOutput("./mobilenetOutput.txt"); // 读取标准输出


    float sumTime = 0;
    for (int i = 0; i < TESTNUM; i++)
    {
        float inferOut[1000];
        for (int j = 0; j < ITERNUM; j++)
        {
            float Onetime;
            cudaEvent_t start, stop;
            cudaEventCreate(&start);
            cudaEventCreate(&stop);
            cudaEventRecord(start, 0);

            // 执行Inference
            inference(inputArr[i], inferOut);
            
            cudaDeviceSynchronize();
            cudaEventRecord(stop, 0);
            cudaEventSynchronize(stop);
            cudaEventElapsedTime(&Onetime, start, stop);
            // 累加单次推理消耗时间
            sumTime += Onetime;
        }
        checkOutput(benchOutArr[i], inferOut);
    }
    printf("Average Time is: %f\n", (sumTime / TESTNUM / ITERNUM));
}


void inference(float *input, float *output)
{
    
    // Block1
    conv2d();
    relu6();

    // Block2:
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block3:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block4:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block5:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block6:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block7:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block8:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block9:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block10:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block11:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block12:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block13:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block14:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block15:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block16:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block17:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    add_layer();

    // Block18:
    point_wise_conv();
    relu6();
    depth_wise_conv();
    relu6();
    point_wise_conv();

    // Block19:
    point_wise_conv();
    relu6();

    // Block20:
    avg_pool();
    linear_layer();
    
}