#pragma once

#ifndef APPLICATION_H
#define	APPLICATION_H

#include <string>

class Application {
public:
	static const std::string& assets_path() {
		return assets_path_;
	}
	static void set_assets_path(std::string assets_path) {
		assets_path_ = assets_path;
	}
private:
	inline static std::string assets_path_;
};


#endif