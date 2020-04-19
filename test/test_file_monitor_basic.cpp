#include "ola/client/utility/file_monitor.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <iomanip>
#include <thread>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;
namespace fs = boost::filesystem;
namespace {
void change_file(const fs::path& _path) {
    ofstream ofs(_path.generic_string());

    ofs << _path << endl;
    ofs.close();
}
}
int test_file_monitor_basic(int argc, char* argv[])
{
    ola::client::utility::FileMonitor file_monitor;

    boost::system::error_code err;
    fs::remove_all("test", err);

    fs::create_directory("test", err);

    file_monitor.start();
    
    file_monitor.add("test/t1.txt", [](const fs::path& _dir, const fs::path& _name, const chrono::system_clock::time_point& _time_point) {
        const std::time_t t_c = std::chrono::system_clock::to_time_t(_time_point);
        cout << "Modified: " << _dir << "/" << _name << " " << std::put_time(std::localtime(&t_c), "%F %T") << endl;
    });
    this_thread::sleep_for(chrono::seconds(1));
    file_monitor.add("test/t2.txt", [](const fs::path& _dir, const fs::path& _name, const chrono::system_clock::time_point& _time_point) {
        const std::time_t t_c = std::chrono::system_clock::to_time_t(_time_point);
        cout << "Modified: " << _dir << "/" << _name << " " << std::put_time(std::localtime(&t_c), "%F %T") << endl;
    });
    this_thread::sleep_for(chrono::seconds(1));
    file_monitor.add("test/t3.txt", [](const fs::path& _dir, const fs::path& _name, const chrono::system_clock::time_point& _time_point) {
        const std::time_t t_c = std::chrono::system_clock::to_time_t(_time_point);
        cout << "Modified: " << _dir << "/" << _name << " " << std::put_time(std::localtime(&t_c), "%F %T") << endl;
    });

    this_thread::sleep_for(chrono::seconds(2));
    cout << "change t1" << endl;
    change_file("test/t1.txt");
    this_thread::sleep_for(chrono::seconds(1));
    cout << "change t2" << endl;
    change_file("test/t2.txt");
    this_thread::sleep_for(chrono::seconds(1));
    cout << "change t3" << endl;
    change_file("test/t3.txt");
    
    return 0;
}