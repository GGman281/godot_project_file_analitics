#pragma once

#include<iostream>
#include<filesystem>
#include<string>


// filesystem
namespace filesystem = std::filesystem;
using filesystem::exists;
using filesystem::is_directory;
using filesystem::recursive_directory_iterator;
using filesystem::directory_entry;

// std
using std::cout;
using std::cin;
using std::getline;
using std::string;

char validate_answer(const string& possible_answers);

void get_valid_directory(string& directory);

bool is_godot_project(const string& directory);