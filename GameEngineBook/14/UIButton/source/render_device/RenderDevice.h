#pragma once

#ifndef RENDER_DEVICE_H
#define RENDER_DEVICE_H

#include <unordered_map>

class RenderDevice {
public:
	static void Init(RenderDevice* instance);
	static RenderDevice* instance() {
		return instance_;
	}
public:
	enum class RenderState {
		None=0,
		STENCIL_TEST
	};
public:
	virtual void Enable(RenderState render_state);
	virtual void Disable(RenderState render_state);
protected:
	std::unordered_map<RenderState, bool> render_state_map_;
private:
	inline static RenderDevice* instance_;
};

#endif // !RENDER_DEVICE_H
