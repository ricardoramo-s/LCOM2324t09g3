#include "aabb.h"

bool AABB_contains(aabb* aabb, vec2* point) {
  if (point->x < aabb->position.x || point->x > aabb->position.x + aabb->size.x) {
        return false;
    }
    if (point->y < aabb->position.y || point->y > aabb->position.y + aabb->size.y) {
        return false;
    }
    return true;
}


bool AABB_overlaps(aabb* first, aabb* second) {
  if (first->position.x + first->size.x < second->position.x || 
        second->position.x + second->size.x < first->position.x) {
        return false;
    }
    if (first->position.y + first->size.y < second->position.y || 
        second->position.y + second->size.y < first->position.y) {
        return false;
    }
    return true;
}
