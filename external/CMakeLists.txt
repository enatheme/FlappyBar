# imgui + sfml built as a lib, intentionally not using full warning flags
add_library(imgui imgui/lib/imgui.cpp imgui/lib/imgui_draw.cpp imgui/lib/imgui-SFML.cpp)
target_link_libraries(imgui PUBLIC sfml-graphics sfml-window sfml-system GL m)
#set_property(TARGET imgui PROPERTY CXX_CLANG_TIDY "")
#set_property(TARGET imgui PROPERTY CXX_CPPCHECK "")
