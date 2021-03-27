if(WINT32)
    set(Sources
        src/utility_file.cpp
    )

    if(WIN32)
        list(APPEND Sources src/file_monitor_win.cpp)
    endif()

    add_library(ola_client_utility 
        locale.hpp arrvec.hpp auth_file.hpp app_list_file.hpp file_monitor.hpp ${Sources}
    )

    target_link_libraries(ola_client_utility SolidFrame::solid_system)
    add_dependencies(ola_client_utility build-cereal)

    add_subdirectory(test)
endif()