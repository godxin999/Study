#include "RenderDeviceOpenGL.h"
#include <glad/glad.h>

void RenderDeviceOpenGL::Enable(RenderDevice::RenderState render_state) {
	if (render_state_map_.find(render_state) != render_state_map_.end() && render_state_map_[render_state]) {
		return;
	}
	switch (render_state)
	{
	case RenderDevice::RenderState::None:
		break;
	case RenderDevice::RenderState::STENCIL_TEST:
		glEnable(GL_STENCIL_TEST);
		break;
	}
	RenderDevice::Enable(render_state);
}

void RenderDeviceOpenGL::Disable(RenderDevice::RenderState render_state) {
	if (render_state_map_.find(render_state) != render_state_map_.end() && !render_state_map_[render_state]) {
		return;
	}
	switch (render_state)
	{
	case RenderDevice::RenderState::None:
		break;
	case RenderDevice::RenderState::STENCIL_TEST:
		glDisable(GL_STENCIL_TEST);
		break;
	}
	RenderDevice::Disable(render_state);
}
