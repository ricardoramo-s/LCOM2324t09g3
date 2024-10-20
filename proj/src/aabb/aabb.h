#pragma once

#include "sprite/sprite.h"
#include "vec2/vec2.h"
#include "stdbool.h"

typedef struct {
  vec2 position;
  vec2 size;
} aabb;

#define AABB(pos, sz) ((aabb){ .position = pos, .size = sz })

bool AABB_contains(aabb* aabb, vec2* point);
bool AABB_overlaps(aabb* first, aabb* second);
