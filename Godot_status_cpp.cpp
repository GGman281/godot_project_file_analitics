#include<iostream>
#include<vector>
#include<string>

#include "fetch.hpp"
#include "blacklist_control.hpp"
#include "validation.hpp"
#include "category_view.hpp"

// std
using std::cout;
using std::cin;
using std::string;


int main()
{
    std::vector<string> blacklist = {".godot"};
    string directory = "";
    char answer;
    
    
    cout << "Please give us your godot project directory (either absolute or relative path): ";
    get_valid_directory(directory);
    
    
    cout << "Checking if it is a godot folder... \n";
    while(!is_godot_project(directory))
    {
        cout << "Warning! Couldn't detect .godot file. \n";
        get_valid_directory(directory);
    }
    
    
    cout << "Would you like to change blacklist options? (y/n): ";
    answer = validate_answer("y n");
    if(answer == 'y')
    {
        blacklist_change(blacklist);
    }
    
    
    cout << "Fetching... \n";
    fetch_report fetch_result = fetch(directory, blacklist);
    
    
    cout << "Done! Outputing results... \n";
    present_information(fetch_result);
    
    
    cout << "Would you like to view the files of certain category? \n";
    cout << "(y/n) ";
    answer = validate_answer("y n");
    if(answer == 'n')
    {
        cout << "Thanks for using this program. Self termination engaged.";
        return 0;
    }
    
    category_view(fetch_result);
    
    return 0;
}