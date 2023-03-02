
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