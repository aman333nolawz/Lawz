#include "Compositor.h"
#include "Output.h"
#include "Pointer.h"
#include "Surface.h"
#include <LKeyboard.h>
#include <LOutput.h>
#include <LSeat.h>
#include <Louvre/LNamespaces.h>

Compositor::Compositor() {
  scene.mainView()->setClearColor(0.f, 0.f, 0.f, 1.f);
  softwareCursor.enableDstSize(true);
}

void Compositor::initialized() {
  seat()->keyboard()->setKeymap(nullptr, nullptr, nullptr, nullptr);

  Int32 totalWidth{0};

  // Initialize all avaliable outputs
  for (LOutput *output : seat()->outputs()) {
    // Set double scale to outputs with DPI >= 200
    output->setScale(output->dpi() >= 200 ? 2.f : 1.5f);

    // Assuming your outputs are not rotated or flipped
    output->setTransform(LFramebuffer::Normal);

    output->setPos(LPoint(totalWidth, 0));
    totalWidth += output->size().w();

    addOutput(output);
    output->repaint();
  }
}

LOutput *Compositor::createOutputRequest(const void *params) {
  return new Output(params);
}

LSurface *Compositor::createSurfaceRequest(const void *params) {
  return new CustomSurface(params);
}

LPointer *Compositor::createPointerRequest(const void *params) {
  return new Pointer(params);
}