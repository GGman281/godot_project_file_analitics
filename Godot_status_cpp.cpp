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
    int total_files_count = 0;
    int total_folders_count = 0;
    int editor_files_count = 0;
    int editor_folders_count = 0;
};

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
}


fetch_report fetch(recursive_directory_iterator project_folder)
{
    fetch_report fetch_report_value;
    for(const directory_entry& entry : project_folder)
    {
        if(entry.path().string().find(".godot") != string::npos)
        {
            if(entry.is_directory())
            {
                fetch_report_value.total_folders_count++;
                fetch_report_value.editor_folders_count++;
            }
            else if(entry.is_regular_file())
            {
                fetch_report_value.total_files_count++;
                fetch_report_value.editor_files_count++;
            }
            continue;
        }
        
        
        if(entry.is_directory())
        {
            fetch_report_value.total_folders_count++;
        }
        else if(entry.is_regular_file())
        {
            fetch_report_value.total_files_count++;
            check_file_type(fetch_report_value, entry);
        }
    }
    return fetch_report_value;
}




int main()
{
    string directory = "";
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
    
    cout << "Fetching... \n";
    
    fetch_report fetch_result = fetch(project_folder);
    
    cout << "Total folders: " << fetch_result.total_folders_count << "\n";
    cout << "Excluding editor folders: " << fetch_result.total_folders_count - fetch_result.editor_folders_count << "\n";
    cout << "Total files (excluding folders): " << fetch_result.total_files_count << "\n";
    cout << "Excluding editor files: " << fetch_result.total_files_count - fetch_result.editor_files_count << "\n";
    
    
    cout << "-------------------------\n"; 
    cout << "More detailed information: \n";
    cout << "Images count: " << fetch_result.images.size() << " (and " << fetch_result.vector_images.size() << " vector images) \n";
    cout << "Sound files count: " << fetch_result.sounds.size() << "\n";
    
    return 0;
}