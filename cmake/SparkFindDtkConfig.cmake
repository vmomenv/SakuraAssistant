cmake_minimum_required(VERSION 3.5.1)

# include(SparkFindQt5Config.cmake)
find_package(Dtk COMPONENTS Core Widget Gui)

function(target_link_dtk NAME)
    target_link_libraries(${NAME} 
        ${DtkCore_LIBRARIES}
        ${DtkWidget_LIBRARIES}
        ${DtkGui_LIBRARIES})
endfunction(target_link_dtk NAME)