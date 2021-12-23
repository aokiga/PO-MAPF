#include "VertexState.h"

bool operator < (VertexInfo a, VertexInfo b) {
    if (a.f != b.f) return a.f < b.f;
    return a.g > b.g;
}
