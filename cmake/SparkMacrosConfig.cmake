cmake_minimum_required(VERSION 3.5.1)

# 定义一些 macro 用于自动生成构建结构

# spark_add_library <lib_name> [files]...
# 构建一个库，基于指定的源文件
    # 并根据库名生成 target_link_<lib_name> 函数
macro(spark_add_library _lib_name)
    message("================ ${_lib_name} Library ================")
    add_library(${_lib_name} ${ARGN})
    
    set(SRCS ${ARGN})
    foreach(item IN LISTS SRCS)
        message(" -> ${item}")
    endforeach(item IN LISTS SRCS)
    
    function(target_link_${_lib_name} TARGET)
        message("${_lib_name}")
        target_link_libraries(${TARGET} ${_lib_name})
    endfunction(target_link_${_lib_name} TARGET)

endmacro(spark_add_library _lib_name)

# spark_add_library_path <lib_name> <lib_path>
# 构建一个库，基于指定的路径
    # 并根据库名生成 target_link_<lib_name> 函数
        # 函数内增加以 <lib_path> 头文件搜索路径
macro(spark_add_library_path _lib_name _lib_path)
    aux_source_directory(${_lib_path} ${_lib_name}_SOURCES)

    message("================ spark_add_library_path: ${_lib_name} ================")
    file(GLOB UI_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${_lib_path}/*.ui)
    add_library(${_lib_name} ${${_lib_name}_SOURCES} ${UI_LIST})
    foreach(item IN LISTS ${_lib_name}_SOURCES)
        message(" -> ${item}")
    endforeach(item IN LISTS ${_lib_name}_SOURCES)
    
    function(target_link_${_lib_name} TARGET)
        # message("target_link_${_lib_name}")
        message(" -> (include): ${_lib_path}")
        target_include_directories(${TARGET} PUBLIC "${_lib_path}")
        target_link_libraries(${TARGET} ${_lib_name})
    endfunction(target_link_${_lib_name} TARGET)

    function(target_include_${_lib_name} TARGET)
        # message("target_link_${_lib_name}")
        message(" -> (include): ${_lib_path}")
        target_include_directories(${TARGET} PUBLIC "${_lib_path}")
        # target_link_libraries(${TARGET} ${_lib_name})
    endfunction(target_include_${_lib_name} TARGET)

endmacro(spark_add_library_path _lib_name _lib_path)

# spark_add_executable <exec_name> [files]...
# 构建一个可执行文件，基于指定的源文件
    # Qt编译时源文件包括很多类型，需要指定 *.h/*.cpp/*.qrc/*.qm/... 等
macro(spark_add_executable _exec_name)

    message("================ ${_exec_name} Executable ================")
    add_executable(${_exec_name} ${ARGN})

endmacro(spark_add_executable _exec_name)

macro(spark_add_executable_path _exec_name _exec_path)
    aux_source_directory(${_exec_path} ${_exec_name}_SOURCES)

    message("================ ${_exec_name} Executable ================")
    file(GLOB UI_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${_exec_path}/*.ui)
    add_executable(${_exec_name} ${${_exec_name}_SOURCES} ${ARGN} ${UI_LIST})
    foreach(item IN LISTS ${_exec_name}_SOURCES)
        message(" -> ${item}")
    endforeach(item IN LISTS ${_exec_name}_SOURCES)
    
    # function(target_link_${_exec_name} TARGET)
    #     message("target_link_${_lib_name}")
    message(" -> (include): ${_exec_path}")
    target_include_directories(${_exec_name} PUBLIC "${_exec_path}")
        # target_link_libraries(${TARGET} ${_lib_name})
    # endfunction(target_link_${_exec_name} TARGET)
    # target_link_${_exec_name}(${_exec_name})

endmacro(spark_add_executable_path _exec_name _exec_path)

# spark_find_library
# 搜索一个库，基于指定的库名，调用 pkg-config 搜索库
    # 并根据库名生成一个 target_link_<prefix> 函数
macro(spark_find_library _prefix)
    find_package(PkgConfig REQUIRED)
    
    # libnotify
    pkg_check_modules(${_prefix} ${ARGN})
    function(target_link_${_prefix} TARGET)
        target_include_directories(${TARGET} PUBLIC 
            ${${_prefix}_INCLUDE_DIRS})
        target_link_libraries(${TARGET} 
            ${${_prefix}_LIBRARIES})
    endfunction(target_link_${_prefix} TARGET)
    
endmacro(spark_find_library _prefix)


# spark_add_executable_paths
# 自定义构建宏，基于指定的前缀名称，处理后续参数为子目录
    # item: 为进行遍历后的单项，类似于 python3 中的 (for item in items:)
    # file: 为在目录中不以递归(GLOB_RECURSE)方式寻找 qrc 文件，需要将其参与编译才能被 rcc
    # 并根据 prefix-<item> 生成构建目标，
macro(spark_add_executable_paths _prefix_path)
    set(PATHS ${ARGN})
    foreach(item IN LISTS PATHS)
        file(GLOB QRCS "${item}/*.qrc")
        message(">>> add_executable: " "${_prefix_path}-${item} ${item} + ${QRCS}")
        spark_add_executable_path(${_prefix_path}-${item} ${item} ${QRCS})
        target_link_qt5(${_prefix_path}-${item})
    endforeach(item IN LISTS PATHS)
endmacro(spark_add_executable_paths _prefix_path)

# spark_add_link
# 自定义宏以代替当前使用 fucntion 定义 target_link_<name> 结构
    # _IN_NAME: 此宏生成 target_link_<name> 的要求参数
    # ARGN: 此宏剩余的参数列表
        # 在使用 target_link_<name> 时
        # _NAME: 用于此 fucntion 中的要求参数: <_NAME>目标将要连接此库
macro(spark_add_link _IN_NAME)
    function(target_link_${_IN_NAME} _NAME)
        message("LINK ${_NAME} ${ARGN}")
        target_link_libraries(${_NAME} 
            ${ARGN})
    endfunction(target_link_${_IN_NAME} _NAME)
endmacro(spark_add_link _IN_NAME)