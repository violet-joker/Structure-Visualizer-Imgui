add_rules("mode.debug", "mode.release")

target("main")
    set_kind("binary")
    add_files("src/main.cpp")
    add_files("build/imgui/*.cpp")
    add_links("GL", "glfw")