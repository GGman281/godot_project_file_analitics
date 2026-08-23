#include<iostream>
#include<filesystem>
#include<vector>

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

struct fetch_report
{
    std::vector<directory_entry> images;
    std::vector<directory_entry> vector_images;
    std::vector<directory_entry> sounds;  
    std::vector<directory_entry> scenes;  
    std::vector<directory_entry> scripts;  
    std::vector<directory_entry> shaders;
    std::vector<directory_entry> other;
    directory_entry heaviest;
    
    int total_files_count = 0;
    int total_folders_count = 0;
    int blacklisted_files_count = 0;
    int blacklisted_folders_count = 0;
};

void get_valid_directory(string& directory)
{
    cout << "Please enter a valid directory: ";
    getline(cin, directory);
}

void validate_answer(char& answer, const string possible_answers)
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

void check_file_type(fetch_report& fetch_report_value, const directory_entry& entry)
{
    // supported image formats by godot.                https://docs.godotengine.org/en/stable/tutorials/assets_pipeline/importing_images.html
    const string SUPPORTED_RASTER_IMAGE_EXTENSIONS[10] = {
        ".bmp", 
        ".dds",
        ".ktx", 
        ".exr",
        ".hdr",
        ".jpg",
        ".jpeg",
        ".png",
        ".tga",
        ".webp"};
    const string SUPPORTED_SOUND_EXTENSIONS[3] = {".wav", ".ogg", ".mp3"};
    
    
    const string FILE_EXTENSION = entry.path().filename().extension().string();
    
    
    // for images check
    if(find(SUPPORTED_RASTER_IMAGE_EXTENSIONS, std::end(SUPPORTED_RASTER_IMAGE_EXTENSIONS), FILE_EXTENSION) != std::end(SUPPORTED_RASTER_IMAGE_EXTENSIONS))
    {
        fetch_report_value.images.push_back(entry);
    }
    // vector image format
    else if(FILE_EXTENSION == ".svg") 
    {
        fetch_report_value.vector_images.push_back(entry);
    }
    // sounds or music (there usually no significant distinction since both can be the same length or volume)
    else if(find(SUPPORTED_SOUND_EXTENSIONS, std::end(SUPPORTED_SOUND_EXTENSIONS), FILE_EXTENSION) != std::end(SUPPORTED_SOUND_EXTENSIONS))
    {
        fetch_report_value.sounds.push_back(entry); 
    }
    // game scenes
    else if(FILE_EXTENSION == ".tscn")
    {
        fetch_report_value.scenes.push_back(entry);
    }
    // game scripts
    else if(FILE_EXTENSION == ".gd")
    {
        fetch_report_value.scripts.push_back(entry);
    }
    // game shaders. In godot 3.x extensions differ
    else if(FILE_EXTENSION == ".gdshader" || FILE_EXTENSION == ".shader")
    {
        fetch_report_value.shaders.push_back(entry);
    }
    else
    {
        fetch_report_value.other.push_back(entry);
    }
    
}

bool is_blacklisted(const directory_entry& entry, const std::vector<string> blacklisted_list, fetch_report& fetch_report_value)
{
    for(const string blacklisted_item : blacklisted_list)
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

fetch_report fetch(recursive_directory_iterator project_folder, const std::vector<string> blacklisted_list)
{
    fetch_report fetch_report_value;
    
    for(const directory_entry& entry : project_folder)
    {
        if(is_blacklisted(entry, blacklisted_list, fetch_report_value))
        {
            continue;
        }
        
        if(entry.is_directory())
        {
            fetch_report_value.total_folders_count++;
        }
        else if(entry.is_regular_file())
        {
            if(!exists(fetch_report_value.heaviest))
            {
                fetch_report_value.heaviest = entry;
            }
            if(fetch_report_value.heaviest.file_size() < entry.file_size())
            {
                fetch_report_value.heaviest = entry;
            }
            fetch_report_value.total_files_count++;
            check_file_type(fetch_report_value, entry);
        }
    }
    return fetch_report_value;
}

void present_information(fetch_report fetch_result)
{
    cout << "-------------------------\n"; 
    cout << "Total folders: " << fetch_result.total_folders_count << "\n";
    cout << "Excluding blacklisted folders: " << fetch_result.total_folders_count - fetch_result.blacklisted_folders_count << "\n";
    cout << "Total files (excluding folders): " << fetch_result.total_files_count << "\n";
    cout << "Excluding blacklisted files: " << fetch_result.total_files_count - fetch_result.blacklisted_files_count << "\n";
    cout << "Images count: " << fetch_result.images.size() << " (and " << fetch_result.vector_images.size() << " vector images) \n";
    cout << "Sound files count: " << fetch_result.sounds.size() << "\n";
    cout << "Saved scenes count: " << fetch_result.scenes.size() << "\n";
    cout << "Scripts count: " << fetch_result.scripts.size() << "\n";
    cout << "Saved shaders count: " << fetch_result.shaders.size() << "\n";
    cout << "Other project files count: " << fetch_result.other.size() << "\n";
    cout << "Heaviest file: "<< fetch_result.heaviest.path().string() << " (" << std::setprecision(3) << std::fixed << fetch_result.heaviest.file_size()/1000.0 << " kb)\n";
    cout << "-------------------------\n"; 
}

void blacklist_change(std::vector<string>& blacklisted_list)
{
    cout << "Warning! Filter works by detecting keywords in file's path.\n";
    cout << "files with \".godot\" extension (project.godot for example) cannot be blacklisted!\n";
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
    
    cout << "Write \"a\" to add item or \"r\" to remove item from the list;\n";
    cout << "Write \"e\" if you want to exit blacklist menu\n";
    char answer;
    while(answer != 'e')
    {
        validate_answer(answer, "a r e");
        
        if(answer == 'a')
        {
            string directory = "";
            cout << "(Warning) Please keep in mind this program doesn't check for typos or if the folder even exists!\n";
            cout << "Provide folder's relative or absolute path you want to blacklist: ";
            
            cin.ignore(); // apparently it keeps \n here from previous entry and puts it as answer for next getline
            getline(cin, directory);
            
            blacklisted_list.push_back(directory);
            
            cout << "Add another one?\n";
            cout << "(y/n): ";
            cin >> answer;
            validate_answer(answer, "y n");
            if(answer == 'n')
            {
                cout << "Write \"a\" to add item or \"r\" to remove item from the list;\n";
                cout << "Write \"e\" if you want to exit blacklist menu\n";
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
                cout << "Write \"a\" to add item or \"r\" to remove item from the list;\n";
                cout << "Write \"e\" if you want to exit blacklist menu\n";
                continue;
            }
            cout << "\nBlacklist contains: \n";
            int position = 1;
            for(string blacklist_item : blacklisted_list)
            {
                cout << position << ": " <<blacklist_item << "\n";
                position++;
            }
            cout << "\n";
            cout << "Put the number of a cathegory you want to delete. (Yor put -1 to go back to cancel): ";
            int number;
            cin >> number;
            if(number == -1)
            {
                answer = ' ';
                continue;
            }
            
            while(number-1 < 0 || number-1 > blacklisted_list.size())
            {
                cout << "Please provide valid number on the list: ";
                cin >> number;
            }
            
            blacklisted_list.erase(blacklisted_list.begin() + number - 1);
            
            cout << "Delete another one?\n";
            cout << "(y/n): ";
            cin >> answer;
            validate_answer(answer, "y n");
            if(answer == 'n')
            {
                cout << "Write \"a\" to add item or \"r\" to remove item from the list;\n";
                cout << "Write \"e\" if you want to exit blacklist menu\n";
                continue;
            }
            answer = ' ';
            continue;
        }
    }
}


void display_category_content(const std::vector<directory_entry> category)
{
    if(category.empty())
    {
        cout << "\nCategory is empty.\n\n";
        return;
    }
    
    for(const directory_entry& item : category)
    {
        cout << item.path().string() << "\n";
    }
}

void category_view(fetch_report fetch_result, char& answer)
{
    while(answer != 'e')
    {
        cout << "Which chategory's files would you like to display? \n";
        cout << "1) Images\n";
        cout << "2) Vector images\n";
        cout << "3) Sounds\n";
        cout << "4) Saved scenes\n";
        cout << "5) Scripts\n";
        cout << "6) Saved shaders\n";
        cout << "7) Other files\n";
        cout << "\"e\" to exit\n";
        
        cin >> answer;
        validate_answer(answer, "1 2 3 4 5 6 7 e");
        
        switch(answer)
        {
            case '1':
                display_category_content(fetch_result.images);
                break;
            case '2':
                display_category_content(fetch_result.vector_images);
                break;
            case '3':
                display_category_content(fetch_result.sounds);
                break;
            case '4':
                display_category_content(fetch_result.scenes);
                break;
            case '5':
                display_category_content(fetch_result.scripts);
                break;
            case '6':
                display_category_content(fetch_result.shaders);
                break;
            case '7':
                display_category_content(fetch_result.other);
                break;
            default:
                cout << "you're not supposed to see this message.\n"; 
                break;
        }
    }
}



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
    
    category_view(fetch_result, answer);
    
    return 0;
}