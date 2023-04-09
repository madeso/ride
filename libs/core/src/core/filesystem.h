#pragma once

#include <memory>
#include <functional>

struct FileEntry
{
    std::string name;
    std::string path;
    bool is_directory;

    FileEntry(const std::string& n, const std::string& p, bool d);
};

struct ListSettings
{
    bool sort_files = true;
    bool directories_first = true;
};

struct FileSystem
{
    FileSystem() = default;
    virtual ~FileSystem() = default;

    virtual bool load_file(const std::string& path, std::function<void (std::string&)> on_line) = 0;
    virtual std::optional<std::vector<FileEntry>> list(const std::string& dir, const ListSettings& settings) = 0;
    virtual std::optional<std::string> get_current_directory() = 0;
    
    // virtual std::optional<std::string> as_absolute(const std::string& p) = 0;
    // virtual std::optional<bool> exists(const std::string& p) = 0;
};

std::unique_ptr<FileSystem> create_local_filesystem();
