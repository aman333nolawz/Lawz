#ifndef TOPBAR_H
#define TOPBAR_H

#include <LSolidColorView.h>
#include "Compositor.h"
#include "Global.h"

class Output;

class Topbar : public LObject
{
public:
    Topbar(Output *output);
    ~Topbar();

    void destroyThumbnails();
    void copyThmubnailsFromAnotherTopbar();
    void updateGUI();

    Output *output;
    LSolidColorView view{
        0.11f, 0.11f, 0.18f, 0.8f, &G::compositor()->overlayLayer};
};

#endif