#include "base/filesystem.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>

#include "base/humane_sort.h"

FileEntry::FileEntry(const std::string& n, const std::string& p, bool d)
    : name(n),
    path(p),
    is_directory(d)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::optional<std::string> GetCurrentDirectory()
{
    std::error_code error;
    const auto p = std::filesystem::current_path(error);
    if(error)
    {
        std::cerr << "Error getting current directory: " << error << "\n";
        return std::nullopt;
    }
    else
    {
        return p.u8string();
    }
}

std::optional<std::string> AsAbsolute(const std::string& p)
{
    std::error_code error;

    const auto relative = std::filesystem::path(p);
    const auto absolute = std::filesystem::canonical(relative, error);
    if(error)
    {
        std::cerr << "Error making path '" << p << "'absolute: " << error << "\n";
        return std::nullopt;
    }

    return absolute.u8string();
}

std::optional<bool> Exists(const std::string& p)
{
    const auto path = std::filesystem::path(p);
    std::error_code error;
    const bool exists = std::filesystem::exists(path, error);
    if(error)
    {
        std::cerr << "Error checking '" << p << "' if it exists: " << error << "\n";
        return std::nullopt;
    }

    return exists;
}

std::optional<std::vector<FileEntry>> List(const std::string& dir, const ListSettings& settings)
{
    const auto should_sort = settings.sort_files;
    const auto sort = [should_sort](std::vector<FileEntry>* files)
    {
        if(should_sort == false) { return; }
        std::sort(files->begin(), files->end(), [](const FileEntry& lhs, const FileEntry& rhs) {
            return humane_strcmp(lhs.name.c_str(), rhs.name.c_str()) < 0;
        });
    };

    std::vector<FileEntry> files;
    std::vector<FileEntry> folders;

    std::error_code error;
    auto directory_iterator = std::filesystem::directory_iterator(dir, error);
    if(error)
    {
        std::cerr << "Error listing file in " << dir << ": " << error << "\n";
        return std::nullopt;
    }

    for(auto& iterator: directory_iterator)
    {
        const auto p = iterator.path();
        const auto file_name = p.filename().u8string();
        const auto file_path = p.u8string();
        if(iterator.is_directory())
        {
            folders.emplace_back(file_name, file_path, true);
        }
        else if(iterator.is_regular_file())
        {
            files.emplace_back(file_name, file_path, false);
        }
        // ignore other types
    }

    if(settings.directories_first)
    {
        sort(&files);
        sort(&folders);
        folders.insert(folders.end(), files.begin(), files.end());
        return folders;
    }
    else
    {
        folders.insert(folders.end(), files.begin(), files.end());
        sort(&folders);
        return folders;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////


struct filesystem_local : filesystem
{
    bool load_file(const std::string& path, std::function<void (std::string&)> on_line) override
    {
        std::ifstream file;
        file.open(path.c_str(), std::ios::in | std::ios::binary);
        if(file.is_open() == false)
        {
            return false;
        }

        std::string line;
        while(std::getline(file,line))
        {
            on_line(line);
        }

        return true;
    }
};


std::unique_ptr<filesystem> create_local_filesystem()
{
    return std::make_unique<filesystem_local>();
}
