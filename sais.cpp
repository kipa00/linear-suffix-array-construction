#include "common.h"
#include "util.h"
#include "bitset.h"
#include "sais.h"

// uses k extra spaces in bag.
template <typename _T> void induced_sort(const _T n, const _T k, const _T *src, _T *original_counts, _T *is_array, _T *bag, bitset_object& check_bag) {
    std::copy(original_counts, original_counts + k - 1, bag + 1);
    bag[0] = 0;
    // Step 1: write (n-1).
    is_array[bag[src[n - 1]]++] = n - 1;
    // Step 2: forward iteration, write to L-suffix positions.
    for (_T i=0; i<n; ++i) {
        const _T &J_i = is_array[i];
        if (J_i >= 1 && (int)check_bag[J_i - 1]) {
            is_array[bag[src[J_i - 1]]++] = J_i - 1;
        }
    }
    // Step 3: clear all the S-suffix positions.
    // We just check if it is re-written or not, so all we do is copy from the count bag.
    std::copy(original_counts, original_counts + k, bag);
    // Step 4: backward iteration, write to S-suffix positions.
    for (_T i=n; i --> 0;) {
        _T J_i = is_array[i];
        // J_i is an S-suffix position and the position is never re-written yet
        if (!(int)check_bag[J_i] && i < bag[src[J_i]]) {
            J_i = 0;
        }
        if (J_i >= 1 && !check_bag[J_i - 1]) {
            is_array[--bag[src[J_i - 1]]] = J_i - 1;
        }
    }
}

// assumes that src uses all of 0 ~ k - 1 at least once
template <typename _T> void sais(const _T n, const _T k, _T *src, _T *bag, bitset_object check_bag) {
    // Step 1: base case
    if (n <= 0) return;
    if (k == n) {
        for (_T i=0; i<n; ++i) {
            bag[src[i]] = i;
        }
        std::copy(bag, bag + n, src);
        return;
    }
    // Step 2: get the type of suffixes.
    {
        check_bag[n - 1] = 1;
        for (int i=n-2; i>=0; --i) {
            if (src[i] > src[i+1]) {
                check_bag[i] = 1;
            } else if (src[i] < src[i+1]) {
                check_bag[i] = 0;
            } else {
                check_bag[i] = (int)check_bag[i+1];
            }
        }
    }
    // Step 3: write S-suffixes and IS.
    _T * const counts = bag, * const is_array = bag + k, * const used_counts = bag + n + k; // n + 2k so basically 3n
    {
        std::fill(counts, counts + n + k, 0);
        for (_T i=0; i<n; ++i) {
            ++counts[src[i]];
        }
        for (_T i=1; i<k; ++i) {
            counts[i] += counts[i-1];
        }
        std::copy(counts, counts + k, used_counts);
        for (_T i=n; i --> 0;) {
            if (!check_bag[i]) {
                is_array[--used_counts[src[i]]] = i;
            }
        }
        induced_sort(n, k, src, counts, is_array, used_counts, check_bag);
    }
    // Step 4, 5: compare the LMS blocks, write the smaller string and recursively call.
    _T smaller_n, * const smaller_string = bag + k;
    {
        _T last = 0, char_count = 0;
        for (_T i=0; i<n; ++i) {
            const _T &idx = is_array[i];
            if (idx > 0 && (int)check_bag[idx-1] && !check_bag[idx]) {
                if (last > 0) {
                    _T k;
                    const _T &j = last;
                    bool equal = true;
                    for (k=0; !k || !(
                        (idx + k >= n || ((int)check_bag[idx+k-1] && !check_bag[idx+k]))
                        || (j + k >= n || ((int)check_bag[j+k-1] && !check_bag[j+k]))
                    ); ++k) {
                        if (src[idx+k] != src[j+k]) {
                            equal = false;
                            break;
                        }
                    }
                    equal = equal && (
                        (idx + k >= n || ((int)check_bag[idx+k-1] && !check_bag[idx+k]))
                        && (j + k >= n || ((int)check_bag[j+k-1] && !check_bag[j+k]))
                    );
                    if (!equal) {
                        ++char_count;
                        last = idx;
                    }
                } else {
                    last = idx;
                }
                used_counts[idx] = char_count;
            }
        }
        _T *ptr = smaller_string;
        for (_T i=1; i<n; ++i) {
            if ((int)check_bag[i-1] && !check_bag[i]) {
                *ptr++ = used_counts[i];
            }
        }
        smaller_n = (_T)std::distance(smaller_string, ptr);
        // among 3n, used half of them and the recursive factor is still 3
        sais(smaller_n, char_count + 1, smaller_string, bag + k + smaller_n, check_bag[n]);
    }
    // Step 6: rewrite sorted LMS suffixes and IS.
    _T * const temp = bag + k + smaller_n, * const is_array_revisited = bag + k + n;
    {
        _T *ptr = temp;
        for (_T i=1; i<n; ++i) {
            if ((int)check_bag[i-1] && !check_bag[i]) {
                *ptr++ = i;
            }
        }
        _T last = src[temp[smaller_string[smaller_n-1]]] ^ 1, pos = 0;
        std::fill(is_array_revisited, is_array_revisited + n, 0);
        for (_T i=smaller_n; i --> 0;) {
            const int &idx = temp[smaller_string[i]];
            if (last != src[idx]) {
                last = src[idx];
                pos = counts[last];
            }
            is_array_revisited[--pos] = idx;
        }
        induced_sort(n, k, src, counts, is_array_revisited, counts + k, check_bag);
    }
    std::copy(is_array_revisited, is_array_revisited + n, src);
}

template <typename _T> void sais(_T n, _T *src) {
    const _T k = preprocess(n, src);
    _T *bag = new _T[3ULL * n + 50];
    bitset bitset_bag(2 * n + 50);
    sais(n, k, src, bag, bitset_bag[0]);
    delete []bag;
}

template void sais<int>(int, int *);
template void sais<ulli>(ulli, ulli *);
