#include<iostream>
#include<filesystem>
#include<vector>
#include "fetch.hpp"


// filesystem
namespace filesystem = std::filesystem;
using filesystem::recursive_directory_iterator;
using filesystem::directory_entry;

// std
using std::cout;
using std::cin;
using std::getline;
using std::string;


bool is_blacklisted(const directory_entry& entry, const std::vector<string>& blacklisted_list, fetch_report& fetch_report_value);

void blacklist_change(std::vector<string>& blacklisted_list);