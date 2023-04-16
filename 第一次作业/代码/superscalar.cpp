#include <windows.h>
#include <cstring>
#include <iostream>
#define MAX int(1e6)
using namespace std;

int num[MAX];
int sum;
int sum1, sum2, sum3, sum4;
int n = 4096;

int count = 50;

void initNum(int n) {
    for (int i = 0; i < n; i++)
        num[i] = i;
}

void serialCal(int n) {
    for (int i = 0; i < n; i++)
        sum += num[i];
}

void twoChainCal(int n) {
    for (int i = 0; i < n / 2; i++) {
        sum1 += num[2 * i];
        sum2 += num[2 * i + 1];
    }
    sum = sum1 + sum2;
}

void fourChainCal(int n) {
    for (int i = 0; i < n / 4; i++) {
        sum1 += num[4 * i];
        sum2 += num[4 * i + 1];
        sum3 += num[4 * i + 2];
        sum4 += num[4 * i + 3];
    }
    sum = sum1 + sum2 + sum3 + sum4;
}

void recursiveFunctionCal(int n) {
    if (n == 1) {
        sum = num[0];
        return;
    } else {
        for (int i = 0; i < n / 2; i++)
            num[i] += num[n - 1 - i];
        n /= 2;
        recursiveFunctionCal(n);
    }
}
void recursiveFunctionUnrollCal(int n) {
    if (n == 1) {
        sum = num[0];
        return;
    } else {
        for (int i = 0; i < n / 2; i += 2) {
            num[i] += num[n - 1 - i];
            num[i + 1] += num[n - 2 - i];
        }
        if (n == 2)
            num[0] += num[1];
        n /= 2;
        recursiveFunctionCal(n);
    }
}

void doubleLoopCal(int n) {
    while (n /= 2) {
        for (int i = 0; i < n; i++)
            num[i] += num[2 * n - i - 1];
    }
    sum = num[0];
}

int main() {
    long freq, head, tail;
    cout << "n = " << n << ", count = " << count << endl;
    initNum(n);
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int i = 0; i < count; i++) {
        sum = 0;
        serialCal(n);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "serial: " << (tail - head) * 1000.0 / freq / count
         << "ms, ans = " << sum << endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int i = 0; i < count; i++) {
        sum = 0;
        sum1 = 0;
        sum2 = 0;
        twoChainCal(n);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "two chain: " << (tail - head) * 1000.0 / freq / count
         << "ms, ans = " << sum << endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for (int i = 0; i < count; i++) {
        sum = 0;
        sum1 = 0;
        sum2 = 0;
        sum3 = 0;
        sum4 = 0;
        fourChainCal(n);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "four chain: " << (tail - head) * 1000.0 / freq / count
         << "ms, ans = " << sum << endl;
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sum = 0;
    initNum(n);
    recursiveFunctionCal(n);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "recursive function: " << (tail - head) * 1000.0 / freq
         << "ms, ans = " << sum << endl;
    initNum(n);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sum = 0;
    initNum(n);
    recursiveFunctionUnrollCal(n);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "recursive function unroll: " << (tail - head) * 1000.0 / freq
         << "ms, ans = " << sum << endl;
    initNum(n);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sum = 0;
    initNum(n);
    doubleLoopCal(n);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "double loop: " << (tail - head) * 1000.0 / freq
         << "ms, ans = " << sum << endl;
}
