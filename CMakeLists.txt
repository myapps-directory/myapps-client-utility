
add_custom_target(versioning
    COMMAND ${CMAKE_COMMAND} -DINFILE="${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in"  -DOUTFILE="${CMAKE_CURRENT_BINARY_DIR}/version.cpp.tmp" -DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_configure_file.cmake"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/version.cpp.tmp" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/version.cpp PROPERTIES GENERATED TRUE)

add_library(ola_client_utility 
    locale.hpp arrvec.hpp auth_file.hpp app_list_file.hpp src/utility_file.cpp file_monitor.hpp src/file_monitor_win.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

target_include_directories(ola_client_utility PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

add_dependencies(ola_client_utility versioning)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version_impl.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/version_impl.hpp)

target_link_libraries(ola_client_utility SolidFrame::solid_system)
add_dependencies(ola_client_utility build-cereal)

add_subdirectory(test)