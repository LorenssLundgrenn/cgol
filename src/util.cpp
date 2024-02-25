#include "util.hpp"

void arr_fill(int* src, int item, int size) {
    for (int i = 0; i < size; i++) {
        src[i] = item;
    }
}

void arr_copy(int* src, int* dest, int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}