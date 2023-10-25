cmake_minimum_required(VERSION 3.5.1)

set(SPARK_FIND_QT5 TRUE)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

# function(target_link_qt5 NAME)
#     target_link_libraries(${NAME}
#         Qt5::Core
#         Qt5::Widgets
#         Qt5::Network)
# endfunction(target_link_qt5 NAME)

# 使用 spark_add_link 生成 target_link_qt5 以替代上面内容
spark_add_link(qt5 Qt5::Core Qt5::Widgets Qt5::Network)


# spark_add_link_qt5
# 自定义宏 spark_add_link_qt5 以扩展 target_link_qt5_<name> 结构
    # _IN_NAME: 此宏使用嵌套宏 spark_add_link 时追加 <name> 名称
    # 同等于 spark_add_link(qt_<name> ${ARGN})
macro(spark_add_link_qt5 _IN_NAME)
    spark_add_link(qt5_${_IN_NAME} ${ARGN})
endmacro(spark_add_link_qt5 _IN_NAME)

# 使用 spark_add_link_qt5 生成 target_link_qt5_<name> 的宏
# spark_add_link_qt5(Concurrent Qt5::Concurrent)

# 高级自定义
# spark_add_links_qt5
# 自定义宏 spark_add_links_qt5 以扩展 spark_add_link_qt5 宏配置组
    # 特点: 任意长度参数
    # qt5_item: 为进行遍历后的单项，类似于 python3 中的 (for item in items:)
    # 例如: qt5_item 为 Core：
        # spark_add_link_qt5(${qt5_item} Qt5::${qt5_item})
        # 展开为 spark_add_link_qt5(Core Qt5::Core)
        # 展开为 spark_add_link(qt5_Core Qt5::Core)
        # 展开为 spark_add_link(qt5_Core Qt5::Core)
        # 特性: 增加 qt5_Core 转 qt5_core
            # string(TOLOWER <string> <output_variable>)
macro(spark_add_links_qt5)
    set(qt5_items ${ARGN})
    foreach(qt5_item IN LISTS qt5_items)
        find_package(Qt5${qt5_item})
        spark_add_link_qt5(${qt5_item} Qt5::${qt5_item})

        string(TOLOWER "${qt5_item}" qt5_lower_item)
        spark_add_link_qt5(${qt5_lower_item} Qt5::${qt5_item})
        spark_debug_message("add_target_link_qt5_${qt5_item} or add_target_link_qt5_${qt5_lower_item}")
    endforeach(qt5_item IN LISTS qt5_items)
endmacro(spark_add_links_qt5)


# Core	                用于其它模块的核心非图形类。
# GUI	                图形用户界面 GUI 组件基类。包括 OpenGL。
# Multimedia	        音频 视频 无线电 摄像头功能类。
# Multimedia Widgets	用于实现多媒体功能，基于 Widget 的类。
# Network	            使网络编程更容易和更可移植的类。

# QML	                QML 和 JavaScript 语言类。
# Quick	                以自定义用户界面 UI 构建高动态应用程序的声明性框架。
# Quick Controls	    为桌面、嵌入式及移动设备创建高性能用户界面提供轻量 QML 类型。这些类型运用简单样式化体系结构且非常高效。
# Quick Dialogs	        用于从 Qt Quick 应用程序创建系统对话框，并与之交互的类型。
# Quick Layouts	        布局是用于在用户界面中排列基于 Qt Quick 2 项的项。
# Quick Test	        用于 QML 应用程序的单元测试框架，其测试案例被编写成 JavaScript 函数。
                        # 注意： 二进制保证不兼容 Qt Quick Test，但源代码仍兼容。

# Qt SQL	            集成使用 SQL 数据库的类。
# Qt Test	            单元测试 Qt 应用程序和库的类。
                        # 注意： 二进制保证不兼容 Qt Test，但源代码仍兼容。
# Qt Widgets	        以 C++ 小部件扩展 Qt GUI 的类。



# 找出所有 Qt5 模板
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt5@Qt5@;' | grep ^Qt5

# 掐头去尾，洗一次
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt5@Qt5@;' | grep ^Qt5 | sed 's@^Qt5@@; s@Config.cmake$@@; /^\s*$/d'

# 排序
# find /usr/lib/x86_64-linux-gnu/cmake/ -name "*Config.cmake" | sed 's@^.*/Qt5@Qt5@;' | grep ^Qt5 | sed 's@^Qt5@@; s@Config.cmake$@@; /^\s*$/d' | sort | pr -t -3

spark_add_links_qt5(
    # AccessibilitySupport
    # AttributionsScannerTools
    # Concurrent
    # Core
    # DBus
    # Designer
    # DesignerComponents
    # DeviceDiscoverySupport
    # DocTools
    # EdidSupport
    # EglFSDeviceIntegration
    # EglFsKmsSupport
    # EglSupport
    # EventDispatcherSupport
    # FbSupport
    # FontDatabaseSupport
    # GlxSupport
    # Gui
    # Help
    # InputSupport
    # KmsSupport
    # LinguistTools
    # LinuxAccessibilitySupport
    # Network
    # OpenGL
    # OpenGLExtensions
    # PacketProtocol
    # PlatformCompositorSupport
    # Positioning
    # PositioningQuick
    # PrintSupport
    # Qml
    # QmlDebug
    # QmlDevTools
    # QmlImportScanner
    # QmlModels
    # QmlWorkerScript
    # Quick
    # QuickCompiler
    # QuickControls2
    # QuickParticles
    # QuickShapes
    # QuickTemplates2
    # QuickTest
    # QuickWidgets
    # SerialBus
    # SerialPort
    # ServiceSupport
    # Sql
    # Svg
    # Test
    # ThemeSupport
    # UiPlugin
    # UiTools
    # VulkanSupport
    # WebChannel
    # WebEngine
    # WebEngineCore
    # WebEngineWidgets
    # WebKit
    # WebKitWidgets
    # WebSockets
    # Widgets
    # X11Extras
    # XcbQpa
    # XkbCommonSupport
    # Xml
    # XmlPatterns
)
