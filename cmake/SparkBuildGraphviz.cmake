cmake_minimum_required(VERSION 3.5.1)

# 添加构建项目依赖图目标: make builddeps
add_custom_target(builddeps
    COMMAND "${CMAKE_COMMAND}" "--graphviz=graphviz/builddeps.dot" .
    COMMAND dot -Tpng graphviz/builddeps.dot -o builddeps.png
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)