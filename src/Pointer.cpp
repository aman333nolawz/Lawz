#include "Pointer.h"
#include "Global.h"
#include <LScene.h>
#include <LView.h>
#include <LCursor.h>

Pointer::Pointer(const void *params) : LPointer(params) {}

void Pointer::pointerMoveEvent(Float32 x, Float32 y, bool isAbsolute) {
  LView *view{G::scene()->handlePointerMoveEvent(x, y, isAbsolute)};

  if ((!view || view->type() != LView::Type::Surface) && !resizingToplevel() &&
      !movingToplevel()) {
    cursor()->useDefault();
    cursor()->setVisible(true);
  }
}

void Pointer::pointerButtonEvent(Button button, ButtonState state) {
  G::scene()->handlePointerButtonEvent(button, state);
}

void Pointer::pointerAxisEvent(Float64 axisX, Float64 axisY, Int32 discreteX,
                               Int32 discreteY, AxisSource source) {
  G::scene()->handlePointerAxisEvent(-axisX, -axisY, -discreteX, -discreteY,
                                     source);
}