add_library(ola_client_utility locale.hpp arrvec.hpp auth_file.hpp src/auth_file.cpp file_monitor.hpp src/file_monitor_win.cpp)
target_link_libraries(ola_client_utility solid_system)

add_subdirectory(test)