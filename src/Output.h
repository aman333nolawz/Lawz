#ifndef OUTPUT_H
#define OUTPUT_H

#include <LOutput.h>
#include <LTextureView.h>
#include "Compositor.h"
#include "Topbar.h"
#include "Global.h"

using namespace Louvre;

class Topbar;

class Output : public LOutput
{
public:
    Output(const void *params);

    void initializeGL() override;
    void moveGL() override;
    void resizeGL() override;
    void paintGL() override;
    void uninitializeGL() override;

    void updateWallpaper();
    LTextureView wallpaperView{nullptr, &G::compositor()->bgLayer};

    Topbar topbar{this};

    void rescueViewsFromVoid();
};

#endif