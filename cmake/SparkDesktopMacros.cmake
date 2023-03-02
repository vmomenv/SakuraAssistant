
macro(spark_desktop_macros _APP_NAME _APP_NAME_ZH_CN _APP_COMMENT _APP_TYPE _APP_EXECUTE_PATH _APP_EXECUTE_ICON_PATH _APP_CATEGORIES)
    set(APP_NAME ${_APP_NAME})
    set(APP_NAME_ZH_CN ${_APP_NAME_ZH_CN})
    set(APP_COMMENT ${_APP_COMMENT})
    set(APP_TYPE ${_APP_TYPE})
    set(APP_EXECUTE_PATH ${_APP_EXECUTE_PATH})
    set(APP_EXECUTE_ICON_PATH ${_APP_EXECUTE_ICON_PATH})
    set(APP_CATEGORIES ${_APP_CATEGORIES})
    configure_file(cmake/spark-desktop.desktop.in
        ${CMAKE_BINARY_DIR}/${_APP_NAME}.desktop
    )
endmacro(spark_desktop_macros _APP_NAME _APP_NAME_ZH_CN _APP_COMMENT _APP_TYPE _APP_EXECUTE_PATH _APP_EXECUTE_ICON_PATH _APP_CATEGORIES)

# include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
# spark_desktop_macros(
    # 应用名称: Name=
    # 应用名称: Name[zh_CN]=
    # 应用说明: Comment=
    # 应用类型: Type=
    # 执行程序: Exec=
    # 图标路径: Icon=
    # 应用分类: Category=
# )

# configure_file(<input> <output>
#                [NO_SOURCE_PERMISSIONS | USE_SOURCE_PERMISSIONS |
#                 FILE_PERMISSIONS <permissions>...]
#                [COPYONLY] [ESCAPE_QUOTES] [@ONLY]
#                [NEWLINE_STYLE [UNIX|DOS|WIN32|LF|CRLF] ])

# install(FILES ${APP_NAME}.desktop
#     DESTINATION /usr/share/applications
# )