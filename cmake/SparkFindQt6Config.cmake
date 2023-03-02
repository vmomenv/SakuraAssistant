cmake_minimum_required(VERSION 3.5.1)

find_package(Qt6 COMPONENTS Core Widgets Network Concurrent)

# function(target_link_qt6 NAME)
#     target_link_libraries(${NAME} 
#         Qt6::Core
#         Qt6::Widgets
#         Qt6::Network)
# endfunction(target_link_qt6 NAME)

# 使用 spark_add_link 生成 target_link_qt6 以替代上面内容 
spark_add_link(qt6 Qt6::Core Qt6::Widgets Qt6::Network)


# spark_add_link_qt6 
# 自定义宏 target_link_qt6 以扩展 target_link_qt6_<name> 结构
    # _IN_NAME: 此宏使用嵌套宏 spark_add_link 时追加 <name> 名称
    # 同等于 spark_add_link(qt_<name> ${ARGN})
macro(spark_add_link_qt6 _IN_NAME)
    spark_add_link(qt6_${_IN_NAME} ${ARGN})
endmacro(spark_add_link_qt6 _IN_NAME)

# 使用 spark_add_link_qt6 生成 target_link_qt6_<name> 的宏
