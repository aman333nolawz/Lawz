#ifndef G_H
#define G_H

#include "LView.h"
#include <LCompositor.h>
#include <list>

using namespace Louvre;

#define TOPBAR_HEIGHT 32
#define TOPBAR_PADDING 4
#define THUMBNAIL_MARGIN 4
#define THUMBNAIL_HEIGHT (TOPBAR_HEIGHT - 2 * TOPBAR_PADDING)

class Compositor;
class Output;
class CustomSurface;

class G
{
public:
    static inline Compositor *compositor()
    {
        return (Compositor *)LCompositor::compositor();
    }

    static inline std::vector<Output *> &outputs()
    {
        return (std::vector<Output *> &)LCompositor::compositor()->outputs();
    }

    static inline std::list<CustomSurface*> &surfaces()
    {
        return (std::list<CustomSurface*> &)LCompositor::compositor()->surfaces();
    }

    static void reparentSurfaceAndChildrenViews(CustomSurface *surface, LView *newParent, bool subsurfaceRoleOnly = false);

    static LScene *scene();
};

#endif
