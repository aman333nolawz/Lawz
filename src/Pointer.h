#ifndef POINTER_H
#define POINTER_H

#include <Louvre/LNamespaces.h>
#include <Louvre/LPointer.h>

using namespace Louvre;

class Pointer : public LPointer {
public:
	Pointer(const void *params);

	void pointerMoveEvent(Float32 x, Float32 y, bool isAbsolute) override;
	void pointerButtonEvent(Button button, ButtonState state) override;
	void pointerAxisEvent(Float64 axisX, Float64 axisY, Int32 discreteX, Int32 discreteY, AxisSource source) override;
};

#endif