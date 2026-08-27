#include "fetch.hpp"



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
        fetch_report_value.types[0].push_back(entry);
    }
    // vector image format
    else if(FILE_EXTENSION == ".svg") 
    {
        fetch_report_value.types[1].push_back(entry);
    }
    // sounds or music (there usually no significant distinction since both can be the same length or volume)
    else if(find(SUPPORTED_SOUND_EXTENSIONS, std::end(SUPPORTED_SOUND_EXTENSIONS), FILE_EXTENSION) != std::end(SUPPORTED_SOUND_EXTENSIONS))
    {
        fetch_report_value.types[2].push_back(entry); 
    }
    // game scenes
    else if(FILE_EXTENSION == ".tscn")
    {
        fetch_report_value.types[3].push_back(entry);
    }
    // game scripts
    else if(FILE_EXTENSION == ".gd")
    {
        fetch_report_value.types[4].push_back(entry);
    }
    // game shaders. In godot 3.x extensions differ
    else if(FILE_EXTENSION == ".gdshader" || FILE_EXTENSION == ".shader")
    {
        fetch_report_value.types[5].push_back(entry);
    }
    // import files
    else if(FILE_EXTENSION == ".import")
    {
        fetch_report_value.types[6].push_back(entry);
    }
    else 
    {
        fetch_report_value.types[7].push_back(entry);
    }
    
}

fetch_report fetch(recursive_directory_iterator project_folder, const std::vector<string>& blacklisted_list)
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