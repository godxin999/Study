add_rules("mode.debug", "mode.release")
set_languages("c++latest")
add_requires("glfw 3.3.9")
add_requires("glad")
add_requires("stb")
add_requires("glm 1.0.0")
add_requires("spdlog 1.13.0", {configs = {header_only = true,std_format = true}})
add_requires("imgui v1.90.4-docking",{configs = {glfw_opengl3 = true}})

target("learnOpenGL")
    set_kind("binary")
    set_policy("build.c++.modules", true)
    add_files("src/**/*.cppm","src/**/*.cpp")
    add_packages("glfw","glad","stb","glm","spdlog","imgui");