#include "Global.h"
#include "Compositor.h"
#include "Surface.h"

LScene *G::scene()
{
  return &compositor()->scene;
}

void G::reparentSurfaceAndChildrenViews(CustomSurface *surface, LView *newParent, bool subsurfaceRoleOnly)
{
  surface->view.setParent(newParent);

  CustomSurface *next{surface};

  if (subsurfaceRoleOnly)
  {
    while ((next = (CustomSurface *)next->nextSurface()))
    {
      if (next->isSubchildOf(surface) && next->subsurface())
        next->view.setParent(newParent);
    }
  }
  else
  {
    while ((next = (CustomSurface *)next->nextSurface()))
    {
      if (next->isSubchildOf(surface))
        next->view.setParent(newParent);
    }
  }
}
