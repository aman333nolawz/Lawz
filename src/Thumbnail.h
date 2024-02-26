#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <LTextureView.h>

class Topbar;
class CustomSurface;

class Thumbnail : public Louvre::LTextureView {
public:
  Thumbnail(Topbar *topbar, CustomSurface *surface);
  ~Thumbnail();
  Topbar *topbar;
  CustomSurface *surface;

  void pointerButtonEvent(Louvre::LPointer::Button button,
                          Louvre::LPointer::ButtonState state) override;
};

#endif