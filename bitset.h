#ifndef BITSET_H
#define BITSET_H

#include "common.h"

typedef unsigned char byte;

class bitset_object;

class bitset { 
private:
    byte *data;
    ulli size;
public:
    bitset(ulli);
    ~bitset();
    int get(int) const;
    void set(int, int);
    bitset_object operator[](ulli);
};

class bitset_object {
private:
    bitset *effective;
    ulli pos;
public:
    bitset_object(bitset *, ulli);
    operator int() const;
    bitset_object &operator=(int);
    bitset_object operator[](ulli);
};

#endif
