#if(WIN32)
    set(Sources
        src/utility_file.cpp
    )

    if(WIN32)
        list(APPEND Sources src/file_monitor_win.cpp)
    endif()

    add_library(myapps_client_utility 
        locale.hpp arrvec.hpp auth_file.hpp app_list_file.hpp file_monitor.hpp ${Sources}
    )

    target_link_libraries(myapps_client_utility SolidFrame::solid_system)
    add_dependencies(myapps_client_utility build-cereal)

    target_include_directories(myapps_client_utility PUBLIC
        ${Boost_INCLUDE_DIRS}
    )

    add_subdirectory(test)
#endif()