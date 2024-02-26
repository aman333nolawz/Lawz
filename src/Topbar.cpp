#include "Topbar.h"
#include "Output.h"
#include "Thumbnail.h"
#include "Global.h"

Topbar::Topbar(Output *output) : output(output) {}

Topbar::~Topbar()
{
    destroyThumbnails();
}

void Topbar::destroyThumbnails()
{
    while (!view.children().empty())
        delete view.children().back();
}

void Topbar::copyThmubnailsFromAnotherTopbar()
{

    for (Output *o : G::outputs())
    {
        if (o == output)
            continue;

        for (Thumbnail *item : (std::list<Thumbnail *> &)o->topbar.view.children())
            new Thumbnail(this, item->surface);
        break;
    }

    updateGUI();
}

void Topbar::updateGUI()
{
    view.setSize(output->size().w(), TOPBAR_HEIGHT);
    view.setPos(output->pos());

    Int32 x{TOPBAR_PADDING};
    for (Thumbnail *thumbnail : (std::list<Thumbnail *> &)view.children())
    {
        thumbnail->setPos(x, TOPBAR_PADDING);
        x += thumbnail->size().w() + THUMBNAIL_MARGIN;
    }

    output->repaint();
}