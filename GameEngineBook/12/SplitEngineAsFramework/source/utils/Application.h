#pragma once

#ifndef APPLICATION_H
#define	APPLICATION_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application {
public:
	static const std::string& assets_path() {
		return assets_path_;
	}
	static void set_assets_path(std::string assets_path) {
		assets_path_ = assets_path;
	}
	static void InitOpenGL();
	static void Run();
	static void UpdateScreenSize();
	static void Update();
	static void Render();
private:
	inline static std::string assets_path_;
	inline static GLFWwindow* glfw_window_;
};


#endif