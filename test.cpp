#include "common.h"
#include "skew.h"
#include "sais.h"
#include <cstdio>
#include <vector>
#include <chrono>

void test_correctness(std::function<void(int, int *)> testing_func) {
    int n = 25;
    int * const bag = new int[n * 3];
    int *v = bag, *w = bag + n, *u = bag + 2 * n;
    int T = 1e5;
    for (int i=1; i<=T; ++i) {
        n = 20 + rand() % 5 - 2;
        for (int i=0; i<n; ++i) {
            v[i] = rand() & 3;
        }
        std::copy(v, v + n, w);
        std::copy(v, v + n, u);
        testing_func(n, w);
        naive_nsqlogn(n, u);
        for (int i=0; i<n; ++i) {
            if (w[i] != u[i]) {
                fprintf(stderr, "WRONG!\n");
                fprintf(stderr, "the original string was ");
                for (int i=0; i<n; ++i) {
                    fprintf(stderr, "%c", 'a' + v[i]);
                }
                fprintf(stderr, "\n");
                fprintf(stderr, "the result of the testing function was");
                for (int i=0; i<n; ++i) {
                    fprintf(stderr, " %d", w[i]);
                }
                fprintf(stderr, "\n");
                fprintf(stderr, "the result of the naive was");
                for (int i=0; i<n; ++i) {
                    fprintf(stderr, " %d", u[i]);
                }
                fprintf(stderr, "\n");
                delete []bag;
                exit(-1);
            }
        }
    }
    printf("Ran %d case(s) without problems\n", T);
    delete []bag;
}

double test_speed(int n, std::function<void(int, int *)> testing_func, int cnt = 10) {
    int *src = new int[n];
    double S = 0;
    for (int i=0; i<cnt; ++i) {
        for (int j=0; j<n; ++j) {
            src[j] = rand() & 3;
        }
        clock_t last_time = clock();
        testing_func(n, src);
        clock_t now_time = clock();
        S += (now_time - last_time) / (double)CLOCKS_PER_SEC;
    }
    delete []src;
    return S / cnt;
}

int main() {
    srand(time(NULL));
    printf("skew test: ");
    fflush(stdout);
    test_correctness(skew<int>);
    printf("sais test: ");
    fflush(stdout);
    test_correctness(sais<int>);
    puts("=== Speed Test ===");
    for (int i=10; i<=100'000'000; i*=2) {
        const double skew_time = test_speed(i, skew<int>);
        const double sais_time = test_speed(i, sais<int>);
        printf("n = %d, skew = %.2lfs, sais = %.2lfs\n", i, skew_time, sais_time);
    }
    return 0;
}