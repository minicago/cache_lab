/* 
 * trans.c - 矩阵转置B=A^T
 *每个转置函数都必须具有以下形式的原型：
 *void trans（int M，int N，int a[N][M]，int B[M][N]）；
 *通过计算，块大小为32字节的1KB直接映射缓存上的未命中数来计算转置函数。
 */ 
#include <stdio.h>
#include "cachelab.h"
int is_transpose(int M, int N, int A[N][M], int B[M][N]);
char transpose_submit_desc[] = "Transpose submission";  //请不要修改“Transpose_submission”
#define min(a,b) (((a)<(b))?(a):(b))

void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

//                          请在此处添加代码
//*************************************Begin********************************************************
    if(N == 32 && M == 32){
        for (int i = 0; i < N; i += 8)
            for (int j = 0; j < M; j += 8){
                if (i != j){
                    for (int k = 0; k < 8; k++)
                        for (int s = 0; s < 8; s++)
                            B[j + s][i + k] = A[i + k][j + s];                     
                }
                if (i == j){
                    if(!(i & 8)){
                        for (int k = 0; k < 8; k++)
                            for (int s = 0; s < 8; s++)
                                B[j + 8 + s][i + 8 + k] = A[i + k][j + s];                              
                        for (int k = 0; k < 8; k++)
                            for (int s = 0; s < 8; s++)
                                B[j + s][i + k] = A[i + 8 + k][j + 8 + s];
                        
                        for (int s = 7; s >= 0; s--)
                            for (int k = 0; k < 8; k++)
                                B[j + 8 + s][i + 8 + k] ^= B[j + s][i + k] ^= B[j + 8 + s][i + 8 + k] ^= B[j + s][i + k];                        
                    }
                }

            }     
    }

    else if(N == 64 && M == 64){
        register int t0,t1,t2,t3;
        for (int i = 0; i < N; i += 8)
            for (int j = 0; j < M; j += 8){
                if (i != j){
                    for (int k = 0; k < 4; k++)
                        for (int s = 0; s < 4; s++){
                            B[j + s][i + k] = A[i + k][j + s];  
                            B[j + s][i + k + 4] = A[i + k][j + s + 4];
                        }
                    
                    for (int s = 0; s < 4; s++){
                        t0 = B[j + s][i + 0 + 4];
                        t1 = B[j + s][i + 1 + 4];
                        t2 = B[j + s][i + 2 + 4];
                        t3 = B[j + s][i + 3 + 4];
                        B[j + s][i + 0 + 4] = A[i + 0 + 4][j + s];
                        B[j + s][i + 1 + 4] = A[i + 1 + 4][j + s];
                        B[j + s][i + 2 + 4] = A[i + 2 + 4][j + s];
                        B[j + s][i + 3 + 4] = A[i + 3 + 4][j + s];                    
                        B[j + s + 4][i + 0] = t0;
                        B[j + s + 4][i + 1] = t1;
                        B[j + s + 4][i + 2] = t2;
                        B[j + s + 4][i + 3] = t3;
                        B[j + s + 4][i + 0 + 4] = A[i + 0 + 4][j + s + 4];
                        B[j + s + 4][i + 1 + 4] = A[i + 1 + 4][j + s + 4];
                        B[j + s + 4][i + 2 + 4] = A[i + 2 + 4][j + s + 4];
                        B[j + s + 4][i + 3 + 4] = A[i + 3 + 4][j + s + 4];
                    }        
                                                                                         
                }
                if (i == j){
                    if(!(i & 8)){
                        for (int k = 0; k < 4; k++)
                            for (int s = 0; s < 4; s++){
                                B[8 + j + s][8 + i + k] = A[i + k][j + s];  
                                B[8 + j + s][8 + i + k + 4] = A[i + k][j + s + 4];
                            }
                        for (int k = 0; k < 4; k++)
                            for (int s = 0; s < 4; s++){
                                B[j + s][i + k] = A[8 + i + k][8 + j + s];  
                                B[j + s][i + k + 4] = A[8 + i + k][8 + j + s + 4];
                            }

                        for (int s = 0; s < 4; s++){
                            t0 = B[j + s][i + 0 + 4];
                            t1 = B[j + s][i + 1 + 4];
                            t2 = B[j + s][i + 2 + 4];
                            t3 = B[j + s][i + 3 + 4];
                            B[j + s][i + 0 + 4] = A[8 + i + 0 + 4][8 + j + s];
                            B[j + s][i + 1 + 4] = A[8 + i + 1 + 4][8 + j + s];
                            B[j + s][i + 2 + 4] = A[8 + i + 2 + 4][8 + j + s];
                            B[j + s][i + 3 + 4] = A[8 + i + 3 + 4][8 + j + s];                    
                            B[j + s + 4][i + 0] = t0;
                            B[j + s + 4][i + 1] = t1;
                            B[j + s + 4][i + 2] = t2;
                            B[j + s + 4][i + 3] = t3;
                            B[j + s + 4][i + 0 + 4] = A[8 + i + 0 + 4][8 + j + s + 4];
                            B[j + s + 4][i + 1 + 4] = A[8 + i + 1 + 4][8 + j + s + 4];
                            B[j + s + 4][i + 2 + 4] = A[8 + i + 2 + 4][8 + j + s + 4];
                            B[j + s + 4][i + 3 + 4] = A[8 + i + 3 + 4][8 + j + s + 4];
                        }        

                        for (int s = 0; s < 4; s++){
                            t0 = B[8 + j + s][8 + i + 0 + 4];
                            t1 = B[8 + j + s][8 + i + 1 + 4];
                            t2 = B[8 + j + s][8 + i + 2 + 4];
                            t3 = B[8 + j + s][8 + i + 3 + 4];
                            B[8 + j + s][8 + i + 0 + 4] = A[i + 0 + 4][j + s];
                            B[8 + j + s][8 + i + 1 + 4] = A[i + 1 + 4][j + s];
                            B[8 + j + s][8 + i + 2 + 4] = A[i + 2 + 4][j + s];
                            B[8 + j + s][8 + i + 3 + 4] = A[i + 3 + 4][j + s];                    
                            B[8 + j + s + 4][8 + i + 0] = t0;
                            B[8 + j + s + 4][8 + i + 1] = t1;
                            B[8 + j + s + 4][8 + i + 2] = t2;
                            B[8 + j + s + 4][8 + i + 3] = t3;
                            B[8 + j + s + 4][8 + i + 0 + 4] = A[i + 0 + 4][j + s + 4];
                            B[8 + j + s + 4][8 + i + 1 + 4] = A[i + 1 + 4][j + s + 4];
                            B[8 + j + s + 4][8 + i + 2 + 4] = A[i + 2 + 4][j + s + 4];
                            B[8 + j + s + 4][8 + i + 3 + 4] = A[i + 3 + 4][j + s + 4];
                        }

                        for (int s = 7; s >= 0; s--)
                            for (int k = 0; k < 8; k++)
                                B[j + 8 + s][i + 8 + k] ^= B[j + s][i + k] ^= B[j + 8 + s][i + 8 + k] ^= B[j + s][i + k];
                        
                     
                    }
                }
            }
    }

    else {
        for (int i = 0; i < N; i += 16)
            for (int j = 0; j < M; j += 16)
                for (int k = 0; k < min(16, N-i); k++)
                    for (int s = 0; s < min(16, M-j); s++)
                        B[j + s][i + k] = A[i + k][j + s];
    }

//**************************************End**********************************************************
}

/* 
 * 我们在下面定义了一个简单的方法来帮助您开始，您可以根据下面的例子把上面值置补充完整。
 */ 

/* 
 * 简单的基线转置功能，未针对缓存进行优化。
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions-此函数向驱动程序注册转置函数。
 *在运行时，驱动程序将评估每个注册的函数并总结它们的性能。这是一种试验不同转置策略的简便方法。
 */
void registerFunctions()
{
    /* 注册解决方案函数  */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* 注册任何附加转置函数 */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - 函数检查B是否是A的转置。在从转置函数返回之前，可以通过调用它来检查转置的正确性。
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

