#pragma once

#include <optional>
#include <string>
#include <vector>

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

std::optional<std::string> GetCurrentDirectory();
std::optional<std::string> AsAbsolute(const std::string& p);
std::optional<bool> Exists(const std::string& p);
std::optional<std::vector<FileEntry>> List(const std::string& dir, const ListSettings& settings);
