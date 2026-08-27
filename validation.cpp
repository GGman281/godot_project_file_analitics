#include <cctype>
#include<iostream>
#include "validation.hpp"


// std
using std::cout;
using std::cin;
using std::getline;


char validate_answer(const string& possible_answers)
{
    char answer;
    cin >> answer;
    // answer = given answer by user
    // possible_answers = answers that are accessible. Spacebar doesn't count and can be used as a separator
    answer = (char)tolower(answer);
    while(possible_answers.find(answer) == string::npos || answer == ' ')
    {
        cout << "Please put valid answer (";
        for(const char character : possible_answers)
        {
            if(character != ' ')
            {
                cout << character;
            }
            else
            {
                cout << "/";
            }
        }
        cout << "): ";
        cin >> answer;
    }
    return answer;
}

void get_valid_directory(string& directory)
{
    getline(cin, directory);
    while( !exists(directory) || !is_directory(directory) )
    {
        cout << "Please enter a valid directory: ";
        getline(cin, directory);
    }
}




bool is_godot_project(const string& directory)
{
    recursive_directory_iterator project_folder(directory);
    
    for(const directory_entry& entry : project_folder)
    {
        if(!entry.is_directory() && entry.path().filename() == "project.godot")
        {
            cout << "Success! \n";
            return true;
        }
    }
    return false;
}
