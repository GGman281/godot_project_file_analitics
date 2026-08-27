#pragma once

#include<filesystem>
#include<vector>
#include<string>
#include "fetch.hpp"


// filesystem
namespace filesystem = std::filesystem;
using filesystem::recursive_directory_iterator;
using filesystem::directory_entry;

// std
using std::string;

bool is_blacklisted(const directory_entry& entry, const std::vector<string>& blacklisted_list, fetch_report& fetch_report_value);

void blacklist_change(std::vector<string>& blacklisted_list);