#include "Thumbnail.h"
#include "Surface.h"
#include "Topbar.h"
#include <LPointer.h>

Thumbnail::Thumbnail(Topbar *topbar, CustomSurface *surface)
    : LTextureView(surface->thumbnailTexture, &topbar->view), topbar(topbar),
      surface(surface) {
  surface->thumbnailViews.push_back(this);
  enableParentOpacity(false);
  enableInput(true);
  setBufferScale(2);
  topbar->updateGUI();
}

Thumbnail::~Thumbnail() {
  LVectorRemoveOneUnordered(surface->thumbnailViews, this);
  setParent(nullptr);
  topbar->updateGUI();
}

void Thumbnail::pointerButtonEvent(LPointer::Button button,
                                   LPointer::ButtonState state) {
  if (button == Louvre::LPointer::Button::Left &&
      state == Louvre::LPointer::ButtonState::Released)
    surface->setMinimized(false);
}