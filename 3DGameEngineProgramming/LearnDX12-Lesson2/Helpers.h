#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		throw std::exception();
	}
}