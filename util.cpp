#include "common.h"

template <typename _T> _T preprocess(_T n, _T *arr) {
    if (!n) return 0;
    _T mn = arr[0], mx = arr[0];
    for (_T i=1; i<n; ++i) {
        mn = std::min(mn, arr[i]);
        mx = std::max(mx, arr[i]);
    }
    if (mx - mn > std::max((ulli)n * (sizeof(ulli) * 8 - 1 - __builtin_clzll((ulli)n)), 5ULL)) { // failback to sort
        _T *indexes = (_T *)std::malloc(n * sizeof(_T));
        std::iota(indexes, indexes + n, (_T)0);
        std::sort(indexes, indexes + n, [=] (const auto &i, const auto &j) {
            return arr[i] < arr[j];
        });
        _T last = mn ^ 1;
        _T characters = (_T)(-1);
        for (_T i=0; i<n; ++i) {
            _T &now = arr[indexes[i]];
            if (last != now) {
                ++characters;
                last = now;
            }
            now = characters;
        }
        return characters + 1;
    }
    const int K = mx - mn + 1;
    _T *indexes = (_T *)std::malloc(K * sizeof(_T));
    std::fill(indexes, indexes + K, 0);
    for (_T i=0; i<n; ++i) {
        indexes[arr[i] - mn] = 1;
    }
    indexes[0] = 0;
    for (_T i=1; i<K; ++i) {
        indexes[i] += indexes[i - 1];
    }
    for (_T i=0; i<n; ++i) {
        arr[i] = indexes[arr[i] - mn];
    }
    return indexes[mx - mn] + 1;
}

template <typename _T> void counting_sort(ulli n, _T *dst, _T *src, _T *bag, std::function<_T(_T)> f) {
    if (!n) return;
    _T mn = f(src[0]), mx = mn;
    for (ulli i=1; i<n; ++i) {
        const _T &val = f(src[i]);
        mn = std::min(mn, val);
        mx = std::max(mx, val);
    }
    _T K = mx - mn + 1;
    std::fill(bag, bag + K, 0);
    for (ulli i=0; i<n; ++i) {
        ++bag[f(src[i]) - mn];
    }
    for (ulli i=1; i<K; ++i) {
        bag[i] += bag[i-1];
    }
    for (ulli i=n; i --> 0;) {
        const _T &value = src[i];
        _T &ptr = bag[f(value) - mn];
        --ptr;
        dst[ptr] = value;
    }
}

template <typename _T> void naive_nsqlogn(_T n, _T *str) {
    _T *temp = new _T[n];
    std::iota(temp, temp+n, 0);
    std::sort(temp, temp + n, [=] (const auto &i, const auto &j) {
        for (_T k = 0; i + k < n && j + k < n; ++k) {
            if (str[i + k] != str[j + k]) {
                return str[i + k] < str[j + k];
            }
        }
        return i > j; // since the bigger index should be finishing first
    });
    for (_T i=0; i<n; ++i) {
        str[i] = temp[i];
    }
    delete []temp;
}

template int preprocess<int>(int, int *);
template ulli preprocess<ulli>(ulli, ulli *);
template void naive_nsqlogn<int>(int, int *);
template void naive_nsqlogn<ulli>(ulli, ulli *);
template void counting_sort<int>(ulli, int *, int *, int *, std::function<int(int)>);
template void counting_sort<ulli>(ulli, ulli *, ulli *, ulli *, std::function<ulli(ulli)>);
