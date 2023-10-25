cmake_minimum_required(VERSION 3.5.1)

set(SPARK_FIND_QT6 TRUE)

find_package(Qt6 COMPONENTS Core Widgets Network REQUIRED)

# function(target_link_qt6 NAME)
#     target_link_libraries(${NAME}
#         Qt6::Core
#         Qt6::Widgets
#         Qt6::Network)
# endfunction(target_link_qt6 NAME)

# 使用 spark_add_link 生成 target_link_qt6 以替代上面内容
spark_add_link(qt6 Qt6::Core Qt6::Widgets Qt6::Network)


# spark_add_link_qt6
# 自定义宏 spark_add_link_qt6 以扩展 target_link_qt6_<name> 结构
    # _IN_NAME: 此宏使用嵌套宏 spark_add_link 时追加 <name> 名称
    # 同等于 spark_add_link(qt_<name> ${ARGN})
macro(spark_add_link_qt6 _IN_NAME)
    spark_add_link(qt6_${_IN_NAME} ${ARGN})
endmacro(spark_add_link_qt6 _IN_NAME)

# 使用 spark_add_link_qt6 生成 target_link_qt6_<name> 的宏
# spark_add_link_qt5(Concurrent Qt6::Concurrent)

# 高级自定义
# spark_add_links_qt6
# 自定义宏 spark_add_links_qt6 以扩展 spark_add_link_qt6 宏配置组
    # 特点: 任意长度参数
    # qt6_item: 为进行遍历后的单项，类似于 python3 中的 (for item in items:)
    # 例如: qt6_item 为 Core：
        # spark_add_link_qt6(${qt6_item} Qt6::${qt6_item})
        # 展开为 spark_add_link_qt6(Core Qt6::Core)
        # 展开为 spark_add_link(qt6_Core Qt6::Core)
        # 展开为 spark_add_link(qt6_Core Qt6::Core)
        # 特性: 增加 qt6_Core 转 qt6_core
            # string(TOLOWER <string> <output_variable>)
macro(spark_add_links_qt6)
    set(qt6_items ${ARGN})
    foreach(qt6_item IN LISTS qt6_items)
        find_package(Qt6${qt6_item})
        spark_add_link_qt6(${qt6_item} Qt6::${qt6_item})

        string(TOLOWER "${qt6_item}" qt6_lower_item)
        spark_add_link_qt6(${qt6_lower_item} Qt6::${qt6_item})
        spark_debug_message("add_target_link_qt6_${qt6_item} or add_target_link_qt6_${qt6_lower_item}")
    endforeach(qt6_item IN LISTS qt6_items)
endmacro(spark_add_links_qt6)

# 找出所有 Qt6 模板
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt6@Qt6@;' | grep ^Qt6

# 掐头去尾，洗一次
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt5@Qt5@;' | grep ^Qt5 | sed 's@^Qt5@@; s@Config.cmake$@@; /^\s*$/d'

# 排序
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt5@Qt5@;' | grep ^Qt5 | sed 's@^Qt5@@; s@Config.cmake$@@; /^\s*$/d' | sort | pr -t -3
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt6@Qt6@;' | grep ^Qt6 | sed 's@^Qt6@@; s@Config.cmake$@@; /^\s*$/d' | sort | pr -t -3

spark_add_links_qt6(
    # BuildInternals
    # BuildInternals/StandaloneTests/Qt5CompatTests
    # BuildInternals/StandaloneTests/QtBaseTests
    # Concurrent
    # Core
    # Core5Compat
    # CoreTools
    # DBus
    # DBusTools
    # DeviceDiscoverySupportPrivate
    # EglFSDeviceIntegrationPrivate
    # EglFsKmsGbmSupportPrivate
    # EglFsKmsSupportPrivate
    # FbSupportPrivate
    # Gui
    # GuiTools
    # HostInfo
    # InputSupportPrivate
    # KmsSupportPrivate
    # Network
    # OpenGL
    # OpenGLWidgets
    # PrintSupport
    # QComposePlatformInputContextPlugin
    # QCupsPrinterSupportPlugin
    # QEglFSEmulatorIntegrationPlugin
    # QEglFSIntegrationPlugin
    # QEglFSKmsEglDeviceIntegrationPlugin
    # QEglFSKmsGbmIntegrationPlugin
    # QEglFSX11IntegrationPlugin
    # QEvdevKeyboardPlugin
    # QEvdevMousePlugin
    # QEvdevTabletPlugin
    # QEvdevTouchScreenPlugin
    # QGifPlugin
    # QGtk3ThemePlugin
    # QIBaseDriverPlugin
    # QIbusPlatformInputContextPlugin
    # QICOPlugin
    # QJpegPlugin
    # QLibInputPlugin
    # QLinuxFbIntegrationPlugin
    # QMinimalEglIntegrationPlugin
    # QMinimalIntegrationPlugin
    # QMYSQLDriverPlugin
    # QNetworkManagerNetworkInformationPlugin
    # QODBCDriverPlugin
    # QOffscreenIntegrationPlugin
    # QPSQLDriverPlugin
    # QSQLiteDriverPlugin
    # QTlsBackendCertOnlyPlugin
    # QTlsBackendOpenSSLPlugin
    # QTsLibPlugin
    # QTuioTouchPlugin
    # QVkKhrDisplayIntegrationPlugin
    # QVncIntegrationPlugin
    # QXcbEglIntegrationPlugin
    # QXcbGlxIntegrationPlugin
    # QXcbIntegrationPlugin
    # QXdgDesktopPortalThemePlugin
    # Sql
    # Test
    # Widgets
    # WidgetsTools
    # XcbQpaPrivate
    # Xml
)
