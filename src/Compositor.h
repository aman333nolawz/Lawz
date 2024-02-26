#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <LCompositor.h>
#include <LScene.h>
#include <LSceneView.h>
#include <LLayerView.h>
#include <LTextureView.h>
#include <LPointer.h>
using namespace Louvre;

class Compositor : public LCompositor
{
public:
    Compositor();
    void initialized() override;
    LOutput *createOutputRequest(const void *params) override;
    LSurface *createSurfaceRequest(const void *params) override;
    LPointer *createPointerRequest(const void *params) override;

    LScene scene;
    LLayerView bgLayer{scene.mainView()};
    LLayerView surfacesLayer{scene.mainView()};
    LLayerView overlayLayer{scene.mainView()};
    LLayerView fullScreenLayer{scene.mainView()};
    LLayerView cursorLayer{scene.mainView()};

    LTextureView softwareCursor{nullptr, &cursorLayer};
};

#endif