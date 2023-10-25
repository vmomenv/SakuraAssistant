
# find_plus
# 寻找 INVAl 传入的字符串，如果存在 + 字符将写入位置到 OUTVAL
function(find_plus INVAL OUTVAL)
    string(FIND "${INVAL}" "+" plus_index)
    set(${OUTVAL} ${plus_index} PARENT_SCOPE)
endfunction(find_plus INVAL OUTVAL)

function(find_plus_v INVAL OUTVAL)
    string(FIND "${${INVAL}}" "+" plus_index)
    set(${OUTVAL} ${plus_index} PARENT_SCOPE)
endfunction(find_plus_v INVAL OUTVAL)

function(find_colon INVAL OUTVAL)
    string(FIND "${INVAL}" ":" colon_index)
    set(${OUTVAL} ${colon_index} PARENT_SCOPE)
endfunction(find_colon INVAL OUTVAL)

function(find_colon_v INVAL OUTVAL)
    string(FIND "${${INVAL}}" ":" colon_index)
    set(${OUTVAL} ${colon_index} PARENT_SCOPE)
endfunction(find_colon_v INVAL OUTVAL)

function(find_dir INVAL OUTVAL)
    string(FIND "${INVAL}" "/" _STR ${ARGN})
    set(${OUTVAL} ${_STR} PARENT_SCOPE)
endfunction(find_dir INVAL OUTVAL)

function(find_dir_v INVAL OUTVAL)
    string(FIND "${${INVAL}}" "/" _STR ${ARGN})
    set(${OUTVAL} ${_STR} PARENT_SCOPE)
endfunction(find_dir_v INVAL OUTVAL)

#
function(str_left INVAL INDEX OUTVAL)
    set(LEFT_INDEX ${INDEX})    
    string(SUBSTRING "${INVAL}" 0 ${LEFT_INDEX} _LEFT_V)
    set(${OUTVAL} ${_LEFT_V} PARENT_SCOPE)
endfunction(str_left INVAL INDEX OUTVAL)

function(str_right INVAL INDEX OUTVAL)
    math(EXPR RIGHT_INDEX ${INDEX}+1)
    string(SUBSTRING "${INVAL}" ${RIGHT_INDEX} -1 _RIGHT_V)
    set(${OUTVAL} ${_RIGHT_V} PARENT_SCOPE)
endfunction(str_right INVAL INDEX OUTVAL)

function(str_left_v INVAL INDEX OUTVAL)
    set(LEFT_INDEX ${${INDEX}})    
    string(SUBSTRING "${${INVAL}}" 0 ${LEFT_INDEX} _LEFT_V)
    set(${OUTVAL} ${_LEFT_V} PARENT_SCOPE)
endfunction(str_left_v INVAL INDEX OUTVAL)

function(str_right_v INVAL INDEX OUTVAL)
    math(EXPR RIGHT_INDEX ${${INDEX}}+1)
    string(SUBSTRING "${${INVAL}}" ${RIGHT_INDEX} -1 _RIGHT_V)
    set(${OUTVAL} ${_RIGHT_V} PARENT_SCOPE)
endfunction(str_right_v INVAL INDEX OUTVAL)

#
function(find_colon_plus INVAL OUTVAL)
    find_colon(${INVAL} COLON_INDEX)
    str_right(${INVAL} ${COLON_INDEX} COLON_RIGHT)
    find_plus_v(COLON_RIGHT PLUS_INDEX)
    str_left_v(COLON_RIGHT PLUS_INDEX COLON_RIGHT_LEFT_PLUS)

    set(${OUTVAL} ${COLON_RIGHT_LEFT_PLUS} PARENT_SCOPE)
endfunction(find_colon_plus INVAL OUTVAL)

function(find_colon_plus_v INVAL OUTVAL)
    find_colon_v(${INVAL} COLON_INDEX)
    str_right_v(${INVAL} COLON_INDEX COLON_RIGHT)
    find_plus_v(COLON_RIGHT PLUS_INDEX)
    str_left_v(COLON_RIGHT PLUS_INDEX COLON_RIGHT_LEFT_PLUS)

    set(${OUTVAL} ${COLON_RIGHT_LEFT_PLUS} PARENT_SCOPE)
endfunction(find_colon_plus_v INVAL OUTVAL)

function(find_dir_plus INVAL OUTVAL)
    # t:*/*+d 
    #    ^
    find_dir("${INVAL}" SLASH_INDEX REVERSE)
    str_right("${INVAL}" ${SLASH_INDEX} SLASH_RIGHT)
    find_plus_v(SLASH_RIGHT PLUS_INDEX)
    str_left_v(SLASH_RIGHT PLUS_INDEX SLASH_RIGHT_LEFT_PLUS)

    set(${OUTVAL} ${SLASH_RIGHT_LEFT_PLUS} PARENT_SCOPE)
endfunction(find_dir_plus INVAL OUTVAL)

function(find_dir_plus_v INVAL OUTVAL)
    # t:*/*+d 
    #    ^
    find_dir("${${INVAL}}" SLASH_INDEX REVERSE)
    str_right("${${INVAL}}" ${SLASH_INDEX} SLASH_RIGHT)
    find_plus_v(SLASH_RIGHT PLUS_INDEX)
    str_left_v(SLASH_RIGHT PLUS_INDEX SLASH_RIGHT_LEFT_PLUS)

    set(${OUTVAL} ${SLASH_RIGHT_LEFT_PLUS} PARENT_SCOPE)
endfunction(find_dir_plus_v INVAL OUTVAL)


# spark_add_library_source <target> ...
# 扩展 一行一可执行目标 的构建的扩展宏
# 在构建时将会另外加入这些资源
macro(spark_add_library_source target)
    set(${target}_ADD_SOURCE ${ARGN})
endmacro(spark_add_library_source target)

# 冗余的 target_link_qt5 或 qt6 的处理逻辑
macro(_handle_spark_target_link_qt_macro _target)
    if(SPARK_FIND_QT5)
        target_link_qt5(${_target})
    endif(SPARK_FIND_QT5)

    if(SPARK_FIND_QT6)
        target_link_qt6(${_target})
    endif(SPARK_FIND_QT6)
endmacro(_handle_spark_target_link_qt_macro _target)

# spark_add_library_realpaths
# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
macro(spark_add_library_realpaths)

    set(REALPATHS ${ARGN})
    foreach(REALPATH IN LISTS REALPATHS)

        # # 找 : 号下标，这是找:号的函数
        # find_colon(${REALPATH} COLON_INDEX)
        # 找 / 号下标，这是找/号的函数
        find_dir_v(REALPATH SLASH_INDEX REVERSE)
        # 找 + 号下标，这是找+号的函数
        find_plus_v(REALPATH PLUS_INDEX)

        # +
        if(PLUS_INDEX LESS 0)
            # 完全没有 + 的情况下，它就是一个基于目录的构建
            set(dir ${REALPATH})
            str_right_v(REALPATH SLASH_INDEX target)

            spark_add_library_path(${target} 
                ${dir}
                ${${target}_ADD_SOURCE}
            )
            _handle_spark_target_link_qt_macro(${target})
        else()
            # 有 + 的情况下，获取 + 号下标右侧所有内容为 target_depends_str 并转为列表
            str_right_v(REALPATH PLUS_INDEX target_depends_str)
            string(REPLACE "+" ";" target_depends "${target_depends_str}")
            
            find_dir_plus_v(REALPATH target)
            str_left_v(REALPATH PLUS_INDEX dir)

            spark_add_library_path(${target} 
                ${dir}
                ${${target}_ADD_SOURCE}
            )
            target_include_directories(${target} PUBLIC ${dir})
            target_link_libraries(${target} ${target_depends})
        endif(PLUS_INDEX LESS 0)

    endforeach(REALPATH IN LISTS REALPATHS)

endmacro(spark_add_library_realpaths)


# spark_aux_source_paths
# 将指定路径中的文件变成可用的AUX源文件列表
macro(spark_aux_source_paths AUX_VAR)
    set(${AUX_VAR} "")
    set(${AUX_VAR}_PATHS ${ARGN})

    foreach(aux_path IN LISTS ${AUX_VAR}_PATHS)
        # spark_debug_message("aux_path: ${aux_path}")
        aux_source_directory(${aux_path} ${AUX_VAR})
    endforeach(aux_path IN LISTS ${AUX_VAR}_PATHS)

endmacro(spark_aux_source_paths AUX_VAR)

# spark_file_glob
# 使用用 file(GLOB) 的匹配规则，并一次可匹配多个规则
#
macro(spark_file_glob FGLOB_VAR)
    set(${FGLOB_VAR} "")
    set(${FGLOB_VAR}_PATHS ${ARGN})

    foreach(fglob_path IN LISTS ${FGLOB_VAR}_PATHS)

        file(GLOB FGLOB_PATH_SRCS ${fglob_path})
        foreach(fglob_path_src IN LISTS FGLOB_PATH_SRCS)
            # spark_debug_message(" -> ${item}")
            list(APPEND ${FGLOB_VAR} ${fglob_path_src})
        endforeach(fglob_path_src IN LISTS FGLOB_PATH_SRCS)

    endforeach(fglob_path IN LISTS ${FGLOB_VAR}_PATHS)

endmacro(spark_file_glob FGLOB_VAR)


# spark_add_source_paths
# 将指定路径中的文件变成可用的源文件列表
#
macro(spark_add_source_paths SOURCE_VAR)
    set(${SOURCE_VAR} "")
    set(${SOURCE_VAR}_PATHS ${ARGN})

    spark_aux_source_paths(${SOURCE_VAR} ${ARGN})
    foreach(source_path IN LISTS ${SOURCE_VAR}_PATHS)
        # list(APPEND ${SOURCE_VAR}_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_PATH})
        # aux_source_directory(${SOURCE_PATH} _SOURCES)
        # foreach(item IN LISTS _SOURCES)
        #     # spark_debug_message(" -> ${item}")
        #     list(APPEND ${SOURCE_VAR} ${item})
        # endforeach(item IN LISTS _SOURCES)

        # file(GLOB HEADER_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_PATH}/*.h)
        # foreach(item IN LISTS HEADER_LIST)
        #     # spark_debug_message(" -> ${item}")
        #     list(APPEND ${SOURCE_VAR} ${item})
        # endforeach(item IN LISTS HEADER_LIST)

        file(GLOB UI_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${source_path}/*.ui)
        foreach(ui_src IN LISTS UI_SRCS)
            # spark_debug_message(" -> ${item}")
            list(APPEND ${SOURCE_VAR} ${ui_src})
        endforeach(ui_src IN LISTS UI_SRCS)
    endforeach(source_path IN LISTS ${SOURCE_VAR}_PATHS)
endmacro(spark_add_source_paths SOURCE_VAR)


# spark_add_library_file_glob
    # 
macro(spark_add_library_file_glob _lib_name)
    spark_file_glob(${_lib_name}_SOURCES ${ARGN})
    spark_add_library(${_lib_name} ${${_lib_name}_SOURCES})
endmacro(spark_add_library_file_glob _lib_name)



# spark_add_executable_source <target> ...
# 扩展 一行一可执行目标 的构建的扩展宏
# 在构建时将会另外加入这些资源
macro(spark_add_executable_source target)
    set(${target}_ADD_SOURCE ${ARGN})
endmacro(spark_add_executable_source target)

# 冗余的 spark_add_executable_realpaths 的 dir 处理逻辑
macro(_handle_spark_add_executable_realpaths_if_dir_empty_macro)
    if("${dir}" STREQUAL "")
        spark_add_executable(${target}
            ${${target}_ADD_SOURCE}
        )
    else()
        spark_add_executable_path(${target}
            ${dir}
            ${${target}_ADD_SOURCE}
        )
    endif("${dir}" STREQUAL "")
endmacro(_handle_spark_add_executable_realpaths_if_dir_empty_macro)

# spark_add_executable_realpaths
# 基于传入的项进行构建
# 可接受的值为: 可执行目标:路径列表
# 可接受的值为: 可执行目标:路径列表+依赖库A+依赖库B
macro(spark_add_executable_realpaths)

    set(REALPATHS ${ARGN})
    foreach(REALPATH IN LISTS REALPATHS)

        # 找 : 号下标，这是找:号的函数
        find_colon(${REALPATH} COLON_INDEX)

        if(COLON_INDEX LESS 0)
            # do not anything
        else()
            # 找到 : 号，将截取 target 名称
            # string(SUBSTRING "${REALPATH}" 0 ${COLON_INDEX} REALTARGET)
            find_colon_v(REALPATH COLON_INDEX)
            str_left_v(REALPATH COLON_INDEX target)
            str_right_v(REALPATH COLON_INDEX COLON_REMAIN)
            # message(FATAL_ERROR "构建一个: ${target}") # 已验证

        endif(COLON_INDEX LESS 0)

        # 找 + 号下标，这是找+号的函数
        find_plus_v(REALPATH PLUS_INDEX)

        if(PLUS_INDEX LESS 0)
            # 完全没有 + 的情况下，它就是一个基于目录的构建
            set(dir ${COLON_REMAIN})
            # spark_add_executable_path(${target}
            #     ${dir}
            #     ${${target}_ADD_SOURCE}
            # )
            _handle_spark_add_executable_realpaths_if_dir_empty_macro()
            _handle_spark_target_link_qt_macro(${target})
        else()
            # 有 + 的情况下，获取 + 号下标右侧所有内容为 target_depends_str 并转为列表
            str_right_v(REALPATH PLUS_INDEX target_depends_str)
            string(REPLACE "+" ";" target_depends "${target_depends_str}")

            # 再从主要内容中获取 dir ，以及
            find_colon_plus_v(REALPATH dir)
            # spark_add_executable_path(${target}
            #     ${dir}
            #     ${${target}_ADD_SOURCE}
            # )
            _handle_spark_add_executable_realpaths_if_dir_empty_macro()
            target_include_directories(${target} PUBLIC ${dir})
            target_link_libraries(${target} ${target_depends})
        endif(PLUS_INDEX LESS 0)
    endforeach(REALPATH IN LISTS REALPATHS)

endmacro(spark_add_executable_realpaths)


# 一行一库概念构建
# 1.构建一个库，基于指定的目录路径进行构建
#   src/widgets/DocTypeListView
#              ^目录将被用于制作的目标名称
#               目录下的所有文件将被用于制作此库的源代码文件
#
# 2.构建一个库，基于指定的目录路径进行构建，并依赖其后面所列出的依赖项
#   src/widgets/MaintainerInfoView+DocTypeListView+...
#                                  ^此库将被用于 MaintainerInfoView 库的依赖
#                                 ^此符号'+'将被视为依赖项列表的分隔符

# 一行一可执行目标概念
# 1.构建一个可执行目标，基于指定的目录路径进行构建(行不通，可执行目标很少为一个目录)
# 2.构建一个可执行目标，基于指定的文件路径进行构建(也许可以)
# 3.构建一个可执行目标，基于指定的文件名称进行构建()
# 4.构建一个可执行目标，基于指定命名规则(target:dir:dir+depend+depend...)
