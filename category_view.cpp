#include "category_view.hpp"


void present_information(const fetch_report& fetch_result)
{
    cout << "-------------------------\n"; 
    cout << "Total folders: " << fetch_result.total_folders_count << "\n";
    cout << "Excluding blacklisted folders: " << fetch_result.total_folders_count - fetch_result.blacklisted_folders_count << "\n";
    cout << "Total files (excluding folders): " << fetch_result.total_files_count << "\n";
    cout << "Excluding blacklisted files: " << fetch_result.total_files_count - fetch_result.blacklisted_files_count << "\n";
    cout << "Images count: " << fetch_result.types[0].size() << " (and " << fetch_result.types[1].size() << " vector images) \n";
    for(int i = 2; i < std::distance(std::begin(fetch_result.types), std::end(fetch_result.types)); i++)
    {
        cout << fetch_result.type_names[i] << " files count: " << fetch_result.types[i].size() << "\n";
    }
    cout << "Heaviest file: "<< fetch_result.heaviest.path().string() << " (" << std::setprecision(3) << std::fixed << fetch_result.heaviest.file_size()/1000.0 << " kb)\n";
    cout << "-------------------------\n"; 
}

void display_category_content(const std::vector<directory_entry>& category)
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

void category_view(const fetch_report& fetch_result)
{
    string answer;
    while(answer != "e")
    {
        cout << "Which chategory's files would you like to display? \n";
        int type_amount = std::distance(std::begin(fetch_result.types), std::end(fetch_result.types));
        for(int i = 0; i < type_amount; i++)
        {
            cout << i+1 << ") " << fetch_result.type_names[i] << "\n";
        }
        cout << "\"e\" to exit\n";
        
        cin >> answer;
        if(answer == "e")
        {
            return;
        }
        while(answer.length() == 0 || (stoi(answer)-1 < 0 || stoi(answer)-1 >= type_amount))
        {
            cout << "Please enter a valid answer: ";
            cin >> answer;
        }
        display_category_content(fetch_result.types[stoi(answer)-1]);
        cout << "\n\n";
    }
}
