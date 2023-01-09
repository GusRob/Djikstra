#include "Button.h"

Button::Button() = default;
Button::Button(void (* OnClick)(), int x, int y, int w, int h, int wp, std::string fp) {
  xPos = x;
  yPos = y;
  width = w;
  height = h;
  funcOnClick = OnClick;
  bgIm = fp;
  imWidthPad = wp;
}

void Button::Click() {
  selected = true;
  funcOnClick();
}
void Button::UnClick() {
  selected = false;
}

void Button::ToggleHover() {
  hover = !hover;
}

bool Button::IsWithin(int x, int y) {
  return (x >= xPos) && (x <= xPos + width) && (y >= yPos) && (y <= yPos + height);
}
