#include <windows.h>
#include <cstring>
#include <iostream>
#define N 4096
using namespace std;

int b[N][N], a[N];
int sum[N];

int count = 50;

void ordinaryCal(int n) {
    for (int k = 0; k < count; k++)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                sum[i] += b[j][i] * a[j];
            }
        }
}

void optimizedCal(int n) {
    for (int k = 0; k < count; k++)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                sum[j] += b[i][j] * a[j];
        }
}

void initAB(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            b[i][j] = i + j;
        a[i] = i;
    }
}

void initSum(int n) {
    memset(sum, 0, sizeof(int) * n);
}

int main() {
    long long head, tail, freq;

    cout << "n = " << N << ", count = " << count << endl;
    initAB(N);
    initSum(N);
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    ordinaryCal(N);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "ord: " << (tail - head) * 1000.0 / freq / count << "ms" << endl;
    initSum(N);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    optimizedCal(N);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "opt: " << (tail - head) * 1000.0 / freq / count << "ms" << endl;
}
