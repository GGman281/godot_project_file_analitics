#include<iostream>
#include<filesystem>
#include<vector>

#include "fetch.hpp"
#include "blacklist_control.hpp"
#include "validation.hpp"
#include "cathegory_view.hpp"

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


int main()
{
    std::vector<string> blacklist = {".godot"};
    string directory = "";
    char answer;
    recursive_directory_iterator project_folder;
    
    
    cout << "Please give us your godot project directory (either absolute or relative path): ";
    getline(cin, directory);
    
    while( !exists(directory) || !is_directory(directory) )
    {
        get_valid_directory(directory);
    }
    project_folder = recursive_directory_iterator(directory);
    
    
    cout << "Checking if it is a godot folder... \n";
    
    while(!is_godot_project(project_folder))
    {
        cout << "Warning! Couldn't detect .godot file. \n";
        get_valid_directory(directory);
    }
    project_folder = recursive_directory_iterator(directory);
    
    
    cout << "Success! \n"; 
    
    cout << "Would you like to change blacklist options? (y/n): ";
    cin >> answer;
    validate_answer(answer, "y n");
    if(answer == 'y')
    {
        blacklist_change(blacklist);
    }
    cout << "Fetching... \n";
    
    fetch_report fetch_result = fetch(project_folder, blacklist);
    
    cout << "Done! outputing results... \n";
    present_information(fetch_result);
    
    cout << "Would you like to view the files of certain category? \n";
    cout << "(y/n) ";
    cin >> answer;
    validate_answer(answer, "y n");
    
    
    if(answer == 'n')
    {
        cout << "Thanks for using this program. Self termination engaged.";
        return 0;
    }
    
    category_view(fetch_result);
    
    return 0;
}