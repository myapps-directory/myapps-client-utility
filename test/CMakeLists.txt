
set( client_utilityTestSuite
    test_file_monitor_basic.cpp
)
#
create_test_sourcelist( client_utilityTests test_client_utility.cpp ${client_utilityTestSuite})

add_executable(test_client_utility ${client_utilityTests})

target_link_libraries(test_client_utility
    myapps_client_utility
    SolidFrame::solid_utility
    SolidFrame::solid_system
    Boost::filesystem
    ${SYSTEM_DYNAMIC_LOAD_LIBRARY}
    ${SYSTEM_BASIC_LIBRARIES}
    Threads::Threads
)

add_test(NAME TestClientUtilityFileMonitorBasic       COMMAND  test_client_utility test_file_monitor_basic)
