#include "validation.hpp"




void validate_answer(char& answer, const string& possible_answers)
{
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
}

void get_valid_directory(string& directory)
{
    cout << "Please enter a valid directory: ";
    getline(cin, directory);
}




bool is_godot_project(recursive_directory_iterator project_folder)
{
    for(const directory_entry& entry : project_folder)
    {
        if(!entry.is_directory() && entry.path().filename().extension() == ".godot")
        {
            return true;
        }
    }
    return false;
}
