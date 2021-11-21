#ifndef UTIL_H
#define UTIL_H

#include "common.h"

template <typename _T> _T preprocess(_T, _T *);
template <typename _T> void counting_sort(ulli, _T *, _T *, _T *, std::function<_T(_T)>);

template <typename _T> void naive_nsqlogn(_T, _T *);

#endif
