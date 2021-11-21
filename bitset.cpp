#include "common.h"
#include "bitset.h"

static_assert(sizeof(byte) == 1, "unsigned char is not a byte");

bitset_object::bitset_object(bitset *pointer, ulli pos) {
    this->effective = pointer;
    this->pos = pos;
}

bitset_object::operator int() const {
    return this->effective->get(this->pos);
}

bitset_object &bitset_object::operator=(int x) {
    this->effective->set(pos, x);
    return *this;
}

bitset_object bitset_object::operator[](ulli x) {
    return bitset_object(this->effective, this->pos + x);
}

bitset::bitset(ulli size) {
    this->data = new byte[(size + 7) >> 3];
    this->size = size;
}

bitset::~bitset() {
    delete []this->data;
}

int bitset::get(int pos) const {
    return data[pos >> 3] & (1 << (pos & 7));
}

void bitset::set(int pos, int val) {
    byte bitval = 1 << (pos & 7);
    if (val) this->data[pos >> 3] |= bitval;
    else this->data[pos >> 3] &= ~bitval;
}

bitset_object bitset::operator[](ulli pos) {
    return bitset_object(this, pos);
}
