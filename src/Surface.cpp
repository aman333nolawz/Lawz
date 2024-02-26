#include <LCursor.h>
#include <LSeat.h>
#include <LKeyboard.h>
#include <LPointer.h>
#include "Surface.h"
#include "Thumbnail.h"
#include "Output.h"

CustomSurface::CustomSurface(const void *params) : LSurface(params)
{
    view.enableParentOffset(false);
}

Output *CustomSurface::primaryOutput()
{
    Output *bestOutput{nullptr};
    Int32 bestArea;
    const LRect surfaceRect{rolePos(), size()};

    // Check the output where large portion of surface was
    for (Output *output : G::outputs())
    {
        LRegion tmpRegion;
        tmpRegion.addRegion(surfaceRect);
        tmpRegion.clip(output->rect());

        const LBox &extents{tmpRegion.extents()};
        const Int32 area{(extents.x2 - extents.x1) * (extents.y2 - extents.y1)};
        if (area > bestArea)
        {
            bestArea = area;
            bestOutput = output;
        }
    }
    return bestOutput;
}

LTexture *CustomSurface::capture(const LSize &dstSizeB)
{
    LSceneView tmpSceneView{
        size() * 2, // Framebuffer size : x2 the surface size to make it look good on HiDPI displays.
        2,          // Scaling factor   : Affects how the children views are rendered
        nullptr     // Parent view      : Not required because we are using it for offscreen rendering
    };

    /* Instead of moving each view to the scene view, we move the scene to the views position.
     * This is why disabling parent offset is required. */
    tmpSceneView.setPos(rolePos());

    G::reparentSurfaceAndChildrenViews(this, &tmpSceneView, true);
    tmpSceneView.render();
    G::reparentSurfaceAndChildrenViews(this, &G::compositor()->surfacesLayer, true);

    return tmpSceneView.texture()->copyB(
        dstSizeB, // Destination size in buffer coordiantes
        LRect(),  // Passing an empty src rect means copying the entire texture
        true      // Enable high quality downscaling
    );
}

void CustomSurface::minimizedChanged()
{
    /* When a surface is minimized, all its children are also minimized
     * we only want to display toplevels in the topbar so we ignore the rest */
    if (!toplevel())
        goto toggleVisibility;

    if (minimized())
    {
        // We don't want the surface to continue receiving input events while minimized
        if (hasPointerFocus())
            seat()->pointer()->setFocus(nullptr);
        if (hasKeyboardFocus())
            seat()->keyboard()->setFocus(nullptr);

        /* Save the current surface position relative to the output position as a percentage so we can restore
         * it later even if the outputs arrangement changes or the given output is no longer available */
        if ((outputBeforeMinimize = primaryOutput()))
        {
            const LPointF localPos{pos() - outputBeforeMinimize->pos()};
            relativePosBeforeMinimize = localPos / LSizeF(outputBeforeMinimize->size());
        }
        /* In case the surface is not visible on any output, we select the first available output and position
         * the surface at 1/4 of the output as a fallback */
        else if (!G::outputs().empty())
        {
            outputBeforeMinimize = G::outputs().front();
            relativePosBeforeMinimize = LPointF(0.25f, 0.25f);
        }
        else
        {
            setMinimized(false);
            return;
        }

        const LSize thumbnailSizeB{
            2 * (THUMBNAIL_HEIGHT * size().w()) / size().h(),
            2 * THUMBNAIL_HEIGHT};

        thumbnailTexture = capture(thumbnailSizeB);

        for (Output *output : G::outputs())
            new Thumbnail(&output->topbar, this);
    }
    else
    {
        // If nullptr, then it was uninitialized while the surface was minimized
        if (!outputBeforeMinimize)
        {
            if (!G::outputs().empty())
            {
                outputBeforeMinimize = G::outputs().front();
                relativePosBeforeMinimize = LPointF(0.25f, 0.25f);
            }
        }

        while (!thumbnailViews.empty())
            delete thumbnailViews.front();

        delete thumbnailTexture;
        thumbnailTexture = nullptr;

        // Restore back the previous unminimized position
        setPos(outputBeforeMinimize != nullptr ? outputBeforeMinimize->pos() + (relativePosBeforeMinimize * outputBeforeMinimize->size()) : LPoint(0, TOPBAR_HEIGHT));
        outputBeforeMinimize = nullptr;
        raise();
    }

toggleVisibility:
    view.setVisible(!minimized());
}

void CustomSurface::roleChanged()
{
    if (cursorRole())
        view.setVisible(false);

    else if (dndIcon())
    {
        view.setParent(&G::compositor()->cursorLayer);
        view.insertAfter(nullptr, false);
        setPos(cursor()->pos());
    }
}

void CustomSurface::orderChanged()
{
    CustomSurface *prev{(CustomSurface *)prevSurface()};

    while (prev && prev->view.parent() != view.parent())
        prev = (CustomSurface *)prev->prevSurface();

    if (prev)
        view.insertAfter(&prev->view, false);
    else
        view.insertAfter(nullptr, false);
}

CustomSurface::~CustomSurface()
{
    while (!thumbnailViews.empty())
        delete thumbnailViews.back();
    if (thumbnailTexture)
        delete thumbnailTexture;
}