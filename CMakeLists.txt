#cmake最低版本
cmake_minimum_required(VERSION 3.16)

#项目名字，语言，注意C大写
project(MCLIB C)

# -------- project之后再设置输出目录 ✔ --------
# 可执行文件输出到 build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# 动态库输出到 build/lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# 静态库输出到 build/lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#设置c的标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 先创建库目标mclib
add_library(mclib STATIC
    src/mystring.c
)

# 再设置头文件路径
target_include_directories(mclib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# 测试程序
add_executable(test_demo src/main.c)
target_link_libraries(test_demo PRIVATE mclib)