#include "RenderDevice.h"

void RenderDevice::Init(RenderDevice* instance){
	instance_ = instance;
}

void RenderDevice::Enable(RenderDevice::RenderState render_state){
	render_state_map_[render_state] = true;
}

void RenderDevice::Disable(RenderDevice::RenderState render_state){
	render_state_map_[render_state] = false;
}
