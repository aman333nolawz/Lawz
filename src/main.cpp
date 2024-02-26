#include <LLauncher.h>
#include <LLog.h>

#include "Compositor.h"

using namespace Louvre;

int main()
{
    setenv("LOUVRE_DEBUG", "4", 0);

    LLauncher::startDaemon();

    Compositor compositor;

    if (!compositor.start())
    {
        LLog::fatal("[Lawz] Failed to start compositor.");
        return 1;
    }

    while (compositor.state() != LCompositor::Uninitialized)
        compositor.processLoop(-1);

    return 0;
}