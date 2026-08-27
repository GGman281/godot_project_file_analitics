#include<iostream>
#include<filesystem>
#include<vector>
#include<string>
#include "fetch.hpp"


// filesystem
namespace filesystem = std::filesystem;
using filesystem::directory_entry;

// std
using std::cout;
using std::cin;
using std::string;

void present_information(const fetch_report& fetch_result);

void display_category_content(const std::vector<directory_entry>& category);

void category_view(const fetch_report& fetch_result);