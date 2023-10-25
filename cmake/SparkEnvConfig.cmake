cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# set(CMAKE_BUILD_TYPE "Debug")

option(SPARK_DEBUG_MESSAGE "CMake Spark Module Debug Message." OFF)
set(SPAKK_DEBUG_LOGFILE "${CMAKE_BINARY_DIR}/spark_debug.log" CACHE STRING "Spark Build Debug logfile." FORCE)
file(WRITE ${SPAKK_DEBUG_LOGFILE})

macro(spark_debug_message)
    if(SPARK_DEBUG_MESSAGE)
        set(SPARK_ONECE_LOG ${ARGN})
        message("[SPARK_MESSAGE]: " ${SPARK_ONECE_LOG})
        file(APPEND ${SPAKK_DEBUG_LOGFILE} ${SPARK_ONECE_LOG} "\n")
        unset(SPARK_ONECE_LOG)
    endif(SPARK_DEBUG_MESSAGE)
endmacro(spark_debug_message)

