#include "common.h"
#include "util.h"

template <typename _T> inline bool character_less_than(const _T n, _T *str, _T *R, _T i, _T j) {
    if (str[i] != str[j]) {
        return str[i] < str[j];
    }
    if (i % 3 == 0) {
        return R[i+1] < R[j+1];
    }
    if (i + 1 >= n) return true;
    if (j + 1 >= n) return false;
    if (str[i + 1] != str[j + 1]) return str[i + 1] < str[j + 1];
    return R[i+2] < R[j+2];
}

template <typename _T> void skew(const _T n, _T *src, _T *bag, _T *count_sort_bag) {
    // Base Case: if n < 10, run naïve O(n^2 log n) algorithm.
    // Anyway this is executed only once!
    if (n < 10) {
        naive_nsqlogn(n, src);
        return;
    }
    // Step 1: sort the triplet.
    _T * const temp1 = bag, * const temp2 = bag + n; // uses 2n spaces
    {
        std::iota(temp2, temp2+n, (_T)0);
        counting_sort<_T>(n, temp1, temp2, count_sort_bag, [=] (const _T &i) { return i + 2 >= n ? 0 : (src[i + 2] + 1); });
        counting_sort<_T>(n, temp2, temp1, count_sort_bag, [=] (const _T &i) { return i + 1 >= n ? 0 : (src[i + 1] + 1); });
        counting_sort<_T>(n, temp1, temp2, count_sort_bag, [=] (const _T &i) { return src[i]; });
        _T a = -1, b = -1, c = -1;
        _T char_number = 0;
        for (_T i=0; i<n; ++i) {
            _T idx = temp1[i];
            if (idx < n - 2) {
                if (!(src[idx] == a && src[idx + 1] == b && src[idx + 2] == c)) {
                    ++char_number;
                    a = src[idx];
                    b = src[idx + 1];
                    c = src[idx + 2];
                }
            } else {
                ++char_number;
                a = -1;
            }
            temp2[idx] = char_number;
        }
    }
    // Step 2: construct the smaller string and recursively call.
    _T * const smaller_string = bag, *triplets_order = bag + n; // still at 2n
    const _T smaller_len = n - n / 3 + 1;
    {
        _T *ptr = smaller_string;
        for (_T i=0; i<n; i+=3) {
            *ptr++ = triplets_order[i];
        }
        *ptr++ = 0;
        for (_T i=1; i<n; i+=3) {
            *ptr++ = triplets_order[i];
        }
        // recursive call bag factor: 3n / (2/3)n = 9/2 > 3 so no problem at all!
        skew(smaller_len, smaller_string, bag + smaller_len, count_sort_bag);
    }
    // Step 3: construct the smaller rank array and sort the thirds using the information.
    _T * const thirds = bag + smaller_len, * const smaller_rank = bag + n + 1, * const small_temp = bag + 2 * n + 1; // memory usage at 7n/3
    const _T Z = (n + 2) / 3 + 1, thirds_len = n / 3;
    {
        for (_T i=1; i<smaller_len; ++i) {
            _T &idx = smaller_string[i];
            idx = idx < Z ? 3 * idx : 3 * (idx - Z) + 1;
            smaller_rank[idx] = i;
        }
        for (_T i=0; i<thirds_len; ++i) {
            thirds[i] = i * 3 + 2;
        }
        counting_sort<_T>(thirds_len, small_temp, thirds, count_sort_bag, [=] (const auto &i) { return i + 1 >= n ? 0 : smaller_rank[i + 1]; });
        counting_sort<_T>(thirds_len, thirds, small_temp, count_sort_bag, [=] (const auto &i) { return src[i]; });
    }
    // note that n/3 >= 2 since we dealt in the base case when n < 10 (so here n >= 10).
    smaller_rank[n] = smaller_rank[n + 1] = 0;
    // Step 4: merge two subarrays into one.
    _T *nonthirds = bag;
    _T *result = bag + 2 * n + 3; // memory usage finally 3n
    {
        _T *ptr = result;
        _T i = 1, j = 0;
        while (i < smaller_len && j < thirds_len) {
            _T idx_i = nonthirds[i], idx_j = thirds[j];
            if (character_less_than(n, src, smaller_rank, idx_i, idx_j)) {
                *ptr++ = idx_i;
                ++i;
            } else {
                *ptr++ = idx_j;
                ++j;
            }
        }
        while (i < smaller_len) {
            *ptr++ = nonthirds[i];
            ++i;
        }
        while (j < thirds_len) {
            *ptr++ = thirds[j];
            ++j;
        }
    }
    std::copy(result, result + n, src);
}

template <typename _T> void skew(const _T n, _T *src) {
    // memory management
    _T *bag = new _T[3 * (ulli)n + 50];
    _T *count_sort_bag = new _T[n];
    preprocess(n, src);
    skew(n, src, bag, count_sort_bag);
    delete []bag;
    delete []count_sort_bag;
}

template void skew<int>(int, int *);
template void skew<ulli>(ulli, ulli *);
