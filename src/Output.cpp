#include <LPainter.h>
#include <LOpenGL.h>
#include <LTexture.h>
#include <LCompositor.h>
#include <LDNDManager.h>
#include <LDNDIconRole.h>
#include <LCursor.h>
#include <LSeat.h>
#include <LLog.h>
#include "Output.h"
#include "Compositor.h"
#include "Topbar.h"
#include "Global.h"
#include "Surface.h"

Output::Output(const void *params) : LOutput(params)
{
    /* Texture views have a default nullptr translucent region, which means they are considered
     * completely translucent by the scene. However, setting an empty LRegion (not nullptr) indicates
     * to the scene that the view is completely opaque. */
    const LRegion emptyRegion;
    wallpaperView.setTranslucentRegion(&emptyRegion);
}

void Output::rescueViewsFromVoid()
{
    // Search another initialized output
    const auto isDiff = [this](const Output *output)
    { return this != output; };
    const auto it{std::find_if(G::outputs().begin(), G::outputs().end(), isDiff)};
    Output *fallbackOutput{it != G::outputs().end() ? *it : nullptr};

    for (CustomSurface *surface : G::surfaces())
        if (surface->outputBeforeMinimize == this)
            surface->outputBeforeMinimize = fallbackOutput;
}

void Output::initializeGL()
{
    updateWallpaper();
    topbar.copyThmubnailsFromAnotherTopbar();
    G::scene()->handleInitializeGL(this);
    repaint();
}

void Output::moveGL()
{
    wallpaperView.setPos(pos());
    topbar.updateGUI();
    G::scene()->handleMoveGL(this);
    repaint();
}

void Output::resizeGL()
{
    Int32 totalWidth{0};

    for (Output *o : G::outputs())
    {
        o->setPos(LPoint(totalWidth, 0));
        totalWidth += o->size().w();
    }

    updateWallpaper();
    topbar.updateGUI();
    G::scene()->handleResizeGL(this);
    repaint();
}

void Output::paintGL()
{
    // Check if hw cursor is supported
    if (cursor()->hasHardwareSupport(this))
        G::compositor()->softwareCursor.setTexture(nullptr);
    else
    {
        G::compositor()->softwareCursor.setTexture(cursor()->texture());
        G::compositor()->softwareCursor.setPos(cursor()->rect().pos());
        G::compositor()->softwareCursor.setDstSize(cursor()->rect().size());
        G::compositor()->softwareCursor.setVisible(cursor()->visible());
    }

    // painter()->clearScreen();
    G::scene()->handlePaintGL(this);

    // LRegion damage;
    // damage.addRect(rect());
    // setBufferDamage(&damage);
}

void Output::uninitializeGL()
{
    rescueViewsFromVoid();
    G::scene()->handleUninitializeGL(this);

    if (wallpaperView.texture())
        delete wallpaperView.texture();

    topbar.destroyThumbnails();
}

void Output::updateWallpaper()
{
    if (wallpaperView.texture())
    {
        /* If the current wallpaper size is equal to the output size
         * then the output simply changed its scale */
        if (wallpaperView.texture()->sizeB() == sizeB())
        {
            wallpaperView.setBufferScale(scale());
            wallpaperView.setPos(pos());
            return;
        }

        delete wallpaperView.texture();
    }

    LTexture *originalWallpaper{
        LOpenGL::loadTexture("/home/nolawz/.config/wallpapers/manga.png")};

    if (!originalWallpaper)
    {
        LLog::error("[Lawz] Failed to load wallpaper.");
        return;
    }

    const LSize &originalSize{originalWallpaper->sizeB()};

    /* This section calculates the source wallpaper rect that needs to be copied and scaled to cover the entire output
     * while maintaining the image's aspect ratio. */

    // Source rect within the texture to clip
    LRect srcRect{0};

    // Calculate the texture's width if it were scaled to the output's height, while preserving the aspect ratio
    const Float32 scaledWidth{Float32(size().w() * originalSize.h()) / Float32(size().h())};

    // If the scaled width is greater than or equal to the output's width, we clip the texture's left and right sides
    if (scaledWidth >= originalSize.w())
    {
        srcRect.setW(originalSize.w());
        srcRect.setH((originalSize.w() * size().h()) / size().w());
        srcRect.setY((originalSize.h() - srcRect.h()) / 2);
    }
    // Otherwise, we clip the texture's top and bottom
    else
    {
        srcRect.setH(originalSize.h());
        srcRect.setW((originalSize.h() * size().w()) / size().h());
        srcRect.setX((originalSize.w() - srcRect.w()) / 2);
    }

    // Copy the srcRect of the original wallpaper and scale it to match the output buffer size
    wallpaperView.setTexture(originalWallpaper->copyB(sizeB(), srcRect));

    // Set the buffer scale of wallpaperView to match the output scale
    wallpaperView.setBufferScale(scale());

    // Delete the original wallpaper texture since we are using the scaled copy
    delete originalWallpaper;
    wallpaperView.setPos(pos());
}