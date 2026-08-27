#pragma once

#include<filesystem>
#include<vector>
#include<string>



// filesystem
namespace filesystem = std::filesystem;
using filesystem::recursive_directory_iterator;
using filesystem::directory_entry;

// std
using std::string;


struct fetch_report
{
    std::vector<directory_entry> types[8];
    // order
        string type_names[8] = {
        "Images",
        "Vector images",
        "Sounds", 
        "Saved scenes", 
        "Scripts", 
        "Shaders",
        "Import",
        "Other"
    };
    
    directory_entry heaviest;
    
    int total_files_count = 0;
    int total_folders_count = 0;
    int blacklisted_files_count = 0;
    int blacklisted_folders_count = 0;
};


void check_file_type(fetch_report& fetch_report_value, const directory_entry& entry);

fetch_report fetch(const string& directory, const std::vector<string>& blacklisted_list);

bool is_blacklisted(const directory_entry& entry, const std::vector<string>& blacklisted_list, fetch_report& fetch_report_value);

