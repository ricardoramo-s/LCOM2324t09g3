#include "ui/ui_element.h"

ui_element* ui_element_create(aabb aabb, Sprite* sprite, FunctionPointer on_click) {
  ui_element* element = (ui_element*)malloc(sizeof(ui_element));
  if (element == NULL) {
    // Handle error here
    return NULL;
  }

  ui_element_init(element, aabb, sprite, on_click);
  return element;
}

void ui_element_init(ui_element* element, aabb aabb, Sprite* sprite, FunctionPointer on_click) {
  if (element == NULL) {
    return;
  }
  element->aabb = aabb;
  element->sprite = sprite;
  element->on_click = on_click;
}

void ui_element_on_click(ui_element* element) {
  if (element == NULL) {
    return;
  }
  if (element->on_click != NULL) {
    element->on_click();
  }
}

void ui_element_draw(ui_element* element) {
  Sprite_draw(element->sprite, element->aabb.position);
}
