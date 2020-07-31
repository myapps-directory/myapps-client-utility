add_library(ola_client_utility locale.hpp arrvec.hpp auth_file.hpp app_list_file.hpp src/utility_file.cpp file_monitor.hpp src/file_monitor_win.cpp)
target_link_libraries(ola_client_utility SolidFrame::solid_system)
add_dependencies(ola_client_utility build-cereal)

add_subdirectory(test)