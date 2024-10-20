#include "vec2/vec2.h"
#include "aabb/aabb.h"
#include "sprite/sprite.h"

typedef void (*FunctionPointer)();

typedef struct {
  aabb aabb;
  Sprite* sprite;
  FunctionPointer on_click;
} ui_element;

ui_element* ui_element_create(aabb aabb, Sprite* sprite, FunctionPointer on_click);
void ui_element_init(ui_element* element, aabb aabb, Sprite* sprite, FunctionPointer on_click);
void ui_element_on_click(ui_element* element);
void ui_element_draw(ui_element* element);
