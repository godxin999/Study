add_rules("mode.debug", "mode.release")
add_languages("c++20")
add_requires("glfw 3.3.9","glad")


target("learnOpenGL")
    set_kind("binary")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")
    add_packages("glfw","glad")