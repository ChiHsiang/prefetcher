#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <malloc.h>
#include "impl.c"

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRE "\x1B[32m"

void verify_data(int *testout, int *expected, int n);

int main(int argc, const char *argv[])
{
    if (argc <= 1)
        return 0;
    int n = strtol(argv[1], NULL, 10);
    if (n < 8)
        n = 8;
    printf("verify %d x %x matrix transpose\n", n, n);

    int i, j, k;
    int *testin;
    int *testout;
    int *expected;

    testin = memalign(4, n * n * sizeof(int));
    testout = memalign(4, n * n * sizeof(int));
    expected = memalign(4, n * n * sizeof(int));

    k = 0;
    printf("testin\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            *( testin + i * n + j ) = k++;
            printf("%3d ", *( testin + i * n + j ));
        }
        printf("\n");
    }
    printf("\n");

    printf("expected\n");
    for (i = 0; i < n; i++) {
        k = i;
        for (j = 0; j < n; j++) {
            k = j * n + i;
            *( expected + i * n + j ) = k;
            printf("%3d ", *( expected + i * n + j ));
        }
        printf("\n");
    }
    printf("\n");

    naive_transpose(testin, testout, n, n);
    printf("naive_transpose : \n");
    verify_data(testout, expected, n);

    sse_transpose(testin, testout, n, n);
    printf("sse_transpose : \n");
    verify_data(testout, expected, n);

    avx_transpose(testin, testout, n, n);
    printf("avx_transpose : \n");
    verify_data(testout, expected, n);

    sse_prefetch_transpose(testin, testout, n, n);
    printf("sse_prefetch_transpose : \n");
    verify_data(testout, expected, n);

    avx_prefetch_transpose(testin, testout, n, n);
    printf("avx_prefetch_transpose : \n");
    verify_data(testout, expected, n);

    return 0;
}

void verify_data(int *testout, int *expected, int n)
{
    int i, j;
    if ( memcmp( testout, expected, n * n * sizeof(int) ) == 0 )
        printf("%sResult Correct\n", KGRE);
    else
        printf("%sResult Incorrect\n", KRED);

    printf("%s", KNRM);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%3d ", *( testout + i * n + j ));
        }
        printf("\n");
    }
    printf("\n");
}

