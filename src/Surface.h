#ifndef SURFACE_H
#define SURFACE_H

#include <LSurface.h>
#include <LSurfaceView.h>
#include "Compositor.h"
#include "Global.h"

using namespace Louvre;

class Thumbnail;
class Output;

class CustomSurface : public LSurface
{
public:
    CustomSurface(const void *params);
    ~CustomSurface();

    void roleChanged() override;
    void orderChanged() override;
    void minimizedChanged() override;

    // Output where the surface is currently most visible
    Output *primaryOutput();

    // Take a snapshot of the surface
    LTexture *capture(const LSize &dstSizeB);

    LSurfaceView view{(LSurface *)this, &G::compositor()->surfacesLayer};

    // Single texture shared by all thumbnail views
    LTexture *thumbnailTexture{nullptr};

    // Thumbnail views, one for each output's topbar
    std::vector<Thumbnail *> thumbnailViews;

    // The output where the surface was most visible before being minimized
    Output *outputBeforeMinimize{nullptr};

    // The relative % surface position to outputBeforeMinimize
    LPointF relativePosBeforeMinimize;
};

#endif
