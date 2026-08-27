#include "validation.hpp"
#include "blacklist_control.hpp"
#include <limits>


using std::cout;
using std::cin;
using std::getline;

bool is_blacklisted(const directory_entry& entry, const std::vector<string>& blacklisted_list, fetch_report& fetch_report_value)
{
    for(const string& blacklisted_item : blacklisted_list)
    {
        if(entry.path().string().find(blacklisted_item) != string::npos && entry.path().filename().extension() != ".godot")
        {
            if(entry.is_directory())
            {
                fetch_report_value.total_folders_count++;
                fetch_report_value.blacklisted_folders_count++;
            }
            else if(entry.is_regular_file())
            {
                fetch_report_value.total_files_count++;
                fetch_report_value.blacklisted_files_count++;
            }
            return true;
        }
    }
    return false;
}

void blacklist_change(std::vector<string>& blacklisted_list)
{
    cout << "Warning!\n";
    cout << "Filter works by detecting keywords in file's path.\n";
    cout << "files with \".godot\" extension (project.godot for example) cannot be blacklisted!\n";
    cout << "\n";
    if(blacklisted_list.empty())
    {
        cout << "\nBlacklist is empty.\n\n";
    }
    else
    {
        cout << "\nBlacklist contains: \n";
        int position = 1;
        for(string blacklist_item : blacklisted_list)
        {
            cout << position << ": " <<blacklist_item << "\n";
            position++;
        }
        cout << "\n";
    }
    
    char answer = ' ';
    while(answer != 'e')
    {
        if(answer !='a' && answer != 'r')
        {
            cout << "Write \"a\" to add item or \"r\" to remove item from the list;\n";
            cout << "Write \"e\" if you want to exit blacklist menu\n";
            answer = validate_answer("a r e");
        }
        
        if(answer == 'a')
        {
            string directory = "";
            cout << "What would you like to add: ";
            cin.ignore(); // apparently it keeps \n here from previous entry and puts it as answer for next getline
            getline(cin, directory);
            if(directory.length() == 0)
            {
                cout << "Empty field detected. Not added anything.\n";
                continue;
            }
            blacklisted_list.push_back(directory);
            
            cout << "Add another one?\n";
            cout << "(y/n): ";
            answer = validate_answer("y n");
            if(answer == 'n')
            {
                continue;
            }
            answer = 'a';
        }
        else if(answer == 'r')
        {
            if(blacklisted_list.empty())
            {
                cout << "\nError: can't remove anything from empty blacklist!\n\n";
                answer = ' ';
                continue;
            }
            cout << "\nBlacklist contains: \n";
            int position = 1;
            for(const string& blacklist_item : blacklisted_list)
            {
                cout << position << ": " <<blacklist_item << "\n";
                position++;
            }
            cout << "\n";
            cout << "Put the number of a category you want to delete. (Yor put -1 to go back to cancel): ";
            int number;
            cin >> number;
            while(cin.fail())
            {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max());
                cout << "Please enter a number: ";
                cin >> number;
            }
            if(number == -1)
            {
                answer = ' ';
                continue;
            }
            
            while(number < 1 || number > blacklisted_list.size())
            {
                cout << "Please provide valid number on the list: ";
                cin >> number;
                while(cin.fail())
                {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max());
                    cout << "Please enter a number: ";
                    cin >> number;
                }
            }
            
            blacklisted_list.erase(blacklisted_list.begin() + number - 1);
            
            cout << "Delete another one?\n";
            cout << "(y/n): ";
            answer = validate_answer("y n");
            if(answer == 'n')
            {
                continue;
            }
            answer = 'r';
            continue;
        }
    }
}