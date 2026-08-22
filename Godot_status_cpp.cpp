#include<iostream>
#include<filesystem>


namespace filesystem = std::filesystem;
// filesystem
using filesystem::exists;
using filesystem::is_directory;
using filesystem::recursive_directory_iterator;
using filesystem::directory_entry;



// std
using std::cout;
using std::cin;
using std::getline;
using std::string;

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


int main()
{
    string directory = "";
    recursive_directory_iterator project_folder;
    
    
    cout << "Please give us your godot project directory (either absolute or relative path): ";
    getline(cin, directory);
    
    while( !(exists(directory) || is_directory(directory)) )
    {
        get_valid_directory(directory);
    }
    project_folder = recursive_directory_iterator(directory);
    
    
    cout << "Checking if it is a godot folder... \n";
    
    while(!is_godot_project(project_folder))
    {
        cout << "Warning! Couldn't detect .godot file. \n";
        get_valid_directory(directory);
        project_folder = recursive_directory_iterator(directory);
    }
    
    cout << "Success!"; 
    
    
    
    
    
    return 0;
}