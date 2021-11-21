#include "common.h"
#include "skew.h"
#include "sais.h"

extern "C" void skew_int(int n, int *str) {
    skew(n, str);
}

extern "C" void skew_ull(ulli n, ulli *str) {
    skew(n, str);
}

extern "C" void sais_int(int n, int *str) {
    sais(n, str);
}

extern "C" void sais_ull(ulli n, ulli *str) {
    sais(n, str);
}
