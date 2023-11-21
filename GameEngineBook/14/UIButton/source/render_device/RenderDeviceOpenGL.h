#pragma once
#ifndef RENDERDEVICEOPENGL_H
#define RENDERDEVICEOPENGL_H

#include "RenderDevice.h"
class RenderDeviceOpenGL :public RenderDevice {
public:
	void Enable(RenderState render_state)override;
	void Disable(RenderState render_state)override;
};

#endif // !RENDERDEVICEOPENGL_H
