cmake_minimum_required(VERSION 3.5.1)

find_package(Qt5 COMPONENTS Core Widgets Network)

# function(target_link_qt5 NAME)
#     target_link_libraries(${NAME} 
#         Qt5::Core
#         Qt5::Widgets
#         Qt5::Network)
# endfunction(target_link_qt5 NAME)

# 使用 spark_add_link 生成 target_link_qt5 以替代上面内容 
spark_add_link(qt5 Qt5::Core Qt5::Widgets Qt5::Network)


# spark_add_link_qt5 
# 自定义宏 target_link_qt5 以扩展 target_link_qt5_<name> 结构
    # _IN_NAME: 此宏使用嵌套宏 spark_add_link 时追加 <name> 名称
    # 同等于 spark_add_link(qt_<name> ${ARGN})
macro(spark_add_link_qt5 _IN_NAME)
    spark_add_link(qt5_${_IN_NAME} ${ARGN})
endmacro(spark_add_link_qt5 _IN_NAME)

# 使用 spark_add_link_qt5 生成 target_link_qt5_<name> 的宏
