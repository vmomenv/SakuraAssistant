
cmake_minimum_required(VERSION 3.5.1)

project(spark-assistant LANGUAGES CXX VERSION 1.0.2)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
include(cmake/SparkFindLibraries.cmake)
include(cmake/SparkFindQt5Config.cmake)
# include(cmake/SparkFindQt6Config.cmake)
include(cmake/SparkFindDtkConfig.cmake)

# 资源文件路径
set(QRC_SOURCES "src/sparkassistant/resources.qrc")

spark_add_executable_path(${PROJECT_NAME} src/sparkassistant
    ${QRC_SOURCES}
)
target_link_qt5(${PROJECT_NAME})
target_link_dtk(${PROJECT_NAME})

include(cmake/SparkBuildGraphviz.cmake)

set(CMAKE_INSTALL_PREFIX "/usr")
install(TARGETS ${PROJECT_NAME}
    DESTINATION bin)
install(DIRECTORY src/sparkassistant/pkg/usr
    DESTINATION "/")


find_package(DebPackage PATHS ${CMAKE_SOURCE_DIR})
add_package_descript(cmake/package-deb.descript)


# 预留，Appimage 构建支持
# 1. 在顶层构建中导入 Appimage 的构建
# include(cmake/SparkDesktopMacros.cmake)
# include(cmake/SparkAppimageConfig.cmake)  # 导入来自 Spark 构建的 Appimage 构建
# add_appimage_icon(assets/spark.png)       # 添加到 Appimage 中的默认的图标
# add_appimage_desktop()                    # 添加到 Appimage 中的默认desktop(使用来自 Spark 构建的 Desktop 构建中配置的信息(必须要求 spark-desktop))
# add_appimage()                            # 应用对 Appimage 的构建
