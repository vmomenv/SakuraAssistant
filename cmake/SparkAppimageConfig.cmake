# SparkAppimageConfig.cmake

# 1. 在使用时通过准备默认的 icon 与 SparkDesktopConfig.cmake 在生成 desktop 文件
    # 时进行借用 desktop 文件中的基本描述完成 spark-appimage.dekstop 文件的生成，
    # 并在使用 add_appimage 时完成所有逻辑判断与目标的定义
        # add_appimage_icon
        # add_appimage_desktop
        # add_appimage

# 2. 在 add_appimage 中，我们判断了是否为 cmake 提供了 LINUXDEPLOYQT 宏，
    # 并获取此工具的真实路径。并继续判断了 APPIMAGETOOL 宏与该工具的真实路径。
    # 然后，创建一个目录，用于即将进行的 Appimage 打包。

    # 通过调用 target_linuxdeploy() 来完成 linuxdeploy 的目标创建
    # 通过调用 target_appimage() 来完成 appimage 的目标创建

# 3. 对于 linuxdeploy 目标，大致上就是通过执行 linuxdeployqt 命令与 -appimage 
    # 参数来创建可用于 Appimage 打包的内容结构，并且使用一些参数来处理相关库的依赖。
    # 其次就是，将 spark-appimage.desktop 复制为 default.desktop
    # 另一个就是 spark-appimage.png 复制为 default.png

# 4. 对于 appimage 目标，大致上就是通过执行 appimagetool 命令将准备好打包的目录
    # 结构进行打包为 Appimage 可执行文件，其中为了避免可能缺失的文件，重复了对
    # default.desktop 文件与 default.png 文件的生成。
    # 这是一个依赖的 copy-desktop-appimage 目标，并先行执行

# if ()
set(APPIMAGE_OUTPUT  "${CMAKE_BINARY_DIR}/appimage")
set(APPIMAGE_ICON    "${APPIMAGE_OUTPUT}/default.png")
set(APPIMAGE_DESTKOP "${APPIMAGE_OUTPUT}/default.desktop")
# set(LINUXDEPLOYQT)
# set(APPIMAGETOOL)

function(execute_linuxdeploy _PATH)
    execute_process(COMMAND ${LINUXDEPLOYQT}
        WORKING_DIRECTORY "${APPIMAGE_OUTPUT}"
        )
endfunction(execute_linuxdeploy _PATH)

function(target_linuxdeploy)
    add_custom_target(linuxdeploy pwd
        BYPRODUCTS appimage
        COMMAND cp ../${PROJECT_NAME} .
        COMMAND "${LINUXDEPLOYQT}" ${PROJECT_NAME} -appimage -unsupported-allow-new-glibc -verbose=3 -no-strip|| true
        COMMAND cp ../spark-appimage.desktop default.desktop
        COMMAND cp ../spark-appimage.png     default.png
        WORKING_DIRECTORY "${APPIMAGE_OUTPUT}")
endfunction(target_linuxdeploy)

function(target_appimage)
    add_custom_target(copy-desktop-appimage
        COMMAND cp ../spark-appimage.desktop default.desktop
        COMMAND cp ../spark-appimage.png     default.png
        WORKING_DIRECTORY "${APPIMAGE_OUTPUT}")
    add_custom_target(appimage pwd
        COMMAND ${APPIMAGETOOL} ${APPIMAGE_OUTPUT}
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        DEPENDS copy-desktop-appimage)
endfunction(target_appimage)

function(add_appimage)
    # check linuxdeploy
    if(NOT DEFINED LINUXDEPLOYQT)
        message("AppImage> Not Found LINUXDEPLOYQT Variable!")
        return()
    endif(NOT DEFINED LINUXDEPLOYQT)
    if(CMAKE_VERSION VERSION_LESS 3.19 AND NOT EXISTS ${LINUXDEPLOYQT})
        message("> cmake version is less than 3.19")
        message(WARNING "!Relative paths are not supported!")
    else()
        file(REAL_PATH ${LINUXDEPLOYQT} LINUXDEPLOYQT_REAL_PATH)
    endif(CMAKE_VERSION VERSION_LESS 3.19 AND NOT EXISTS ${LINUXDEPLOYQT})
    message("AppImage> Found LINUXDEPLOYQT Variable: ${LINUXDEPLOYQT_REAL_PATH}")

    # check appimagetool
    if(NOT DEFINED APPIMAGETOOL)
        message("AppImage> Not Found APPIMAGETOOL Variable!")
        return()
    endif(NOT DEFINED APPIMAGETOOL)
    if(CMAKE_VERSION VERSION_LESS 3.19 AND NOT EXISTS ${LINUXDEPLOYQT})
        # execute_process(COMMAND realpath ${APPIMAGETOOL} OUTPUT_VARIABLE APPIMAGETOOL_REAL_PATH)
        message("> cmake version is less than 3.19")
        message(WARNING "!Relative paths are not supported!")
    else()
        file(REAL_PATH ${APPIMAGETOOL} APPIMAGETOOL_REAL_PATH)
    endif(CMAKE_VERSION VERSION_LESS 3.19 AND NOT EXISTS ${LINUXDEPLOYQT})
    message("AppImage> Found APPIMAGETOOL Variable: ${LINUXDEPLOYQT_REAL_PATH}")

    # do add_custome_target
    make_directory(${APPIMAGE_OUTPUT})
    target_linuxdeploy()
    target_appimage()
endfunction(add_appimage)

function(add_appimage_desktop)
    configure_file(cmake/spark-appimage.desktop.in
        ${CMAKE_BINARY_DIR}/spark-appimage.desktop @ONLY)
endfunction(add_appimage_desktop)

function(add_appimage_icon _ICON_PATH)
    if(CMAKE_VERSION VERSION_LESS 3.21)
        message("> cmake version is less than 3.21")
        configure_file(${_ICON_PATH} ${CMAKE_BINARY_DIR}/spark-appimage.png COPYONLY)
    else()
        file(COPY_FILE ${_ICON_PATH} ${CMAKE_BINARY_DIR}/spark-appimage.png)
    endif(CMAKE_VERSION VERSION_LESS 3.21)
endfunction(add_appimage_icon _ICON_PATH)



# 如果glic>=2.27,你就需要加上参数 -unsupported-allow-new-glibc (意思就是不再低版本发行版使用了)
# 或 -unsupported-bundle-everything（大概的意思是尝试兼容，实际测试，到其他发行版直接用不了了，有可能是发行版的原因，还是建议用前者，虽然放弃了低版本）

# -unsupported-bundle-everything
    # 捆绑所有依赖库，包括 ld-linux.so 加载器和 glibc。这将允许构建在较新系统上的应用程序在较旧的目标系统上运行，但不建议这样做，因为它会导致捆绑包超出所需的大小（并且可能到其他发行版无法使用）
# -unsupported-allow-new-glibc
    # 允许 linuxdeployqt 在比仍受支持的最旧 Ubuntu LTS 版本更新的发行版上运行。这将导致 AppImage无法在所有仍受支持的发行版上运行，既不推荐也不测试或支持

# ./linuxdeployqt-7-x86_64.AppImage 程序目录/程序 -appimage -unsupported-allow-new-glibc
# ./linuxdeployqt-7-x86_64.AppImage 程序目录/程序 -appimage -unsupported-bundle-everything




# 1. 在顶层构建中导入 Appimage 的构建
# include(cmake/SparkAppimageConfig.cmake)  # 导入来自 Spark 构建的 Appimage 构建
# add_appimage_icon(assets/spark.png)       # 添加到 Appimage 中的默认的图标
# add_appimage_desktop()                    # 添加到 Appimage 中的默认desktop(使用来自 Spark 构建的 Desktop 构建中配置的信息(必须要求 spark-desktop))
# add_appimage()                            # 应用对 Appimage 的构建

# 2. 在 Makefile 进行构建目标构建 Appimage 的构建流 --
# 在 Makefile 进行构建目标构建 Appimage (要求提供工具的绝对路径，然后可依次进行linuxdeployqt, genrate-appimage)
# 来自于 https://github.com/probonopd/linuxdeployqt 	的 linuxdeployqt
# 来自于 https://github.com/AppImage/AppImageKit		的 appimagetool
# 来自于 https://gitlink.org.cn/zinface/bundle-linuxdeployqt.git  托管存储的工具

# 或指定你所想存放克隆项目的位置
# BUNDLE_LINUXDEPLOYQT := $(shell pwd)/build/bundle-linuxdeployqt

# download-bundle-linuxdeploytools:
# 	-git clone https://gitlink.org.cn/zinface/bundle-linuxdeployqt.git $(BUNDLE_LINUXDEPLOYQT)

# LINUXDEPLOYQT := "$(BUNDLE_LINUXDEPLOYQT)/linuxdeployqt-continuous-x86_64.AppImage"
# APPIMAGETOOL  := "$(BUNDLE_LINUXDEPLOYQT)/appimagetool-x86_64.AppImage"

# linuxdeploy: release download-bundle-linuxdeploytools
# 	cd build && cmake .. -DLINUXDEPLOYQT=$(LINUXDEPLOYQT) -DAPPIMAGETOOL=$(APPIMAGETOOL)
# 	cd build && make linuxdeploy

# genrate-appimage:
# 	cd build && cmake .. -DLINUXDEPLOYQT=$(LINUXDEPLOYQT) -DAPPIMAGETOOL=$(APPIMAGETOOL)
# 	cd build && make appimage
