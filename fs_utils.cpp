#include "fs_utils.hpp"
#include <iostream>

std::filesystem::path get_directory_from_filepath(const std::filesystem::path &filepath) {
    return filepath.parent_path();
}

bool has_extension(const std::filesystem::path &file_path, const std::string &extension) {
    // Get the file extension
    std::string file_extension = file_path.extension().string();

    // Compare extensions (case insensitive)
    return (file_extension == extension ||
            (file_extension.size() > 1 && file_extension[0] == '.' && file_extension.substr(1) == extension));
}

std::filesystem::path get_parent_directory(const std::filesystem::path &current_dir) {
    // Check if the current directory is the root directory
    if (current_dir == current_dir.root_path()) {
        // Already at the root directory, do nothing
        return current_dir;
    }

    // Get the parent directory
    return current_dir.parent_path();
}

std::filesystem::path get_home_directory() {
#ifdef _WIN32
    const char *home_drive = std::getenv("HOMEDRIVE");
    const char *home_path = std::getenv("HOMEPATH");
    if (home_drive && home_path) {
        return std::filesystem::path(home_drive) / home_path;
    }
    return std::getenv("USERPROFILE") ? std::filesystem::path(std::getenv("USERPROFILE")) : std::filesystem::path();
#else
    const char *home = std::getenv("HOME");
    return home ? std::filesystem::path(home) : std::filesystem::path();
#endif
}

std::vector<std::filesystem::path> list_files_and_directories(const std::filesystem::path &path, bool include_hidden) {
    std::vector<std::filesystem::path> paths;
    try {
        for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(path)) {
            // Check if the file is hidden (starts with a dot on Unix-like systems)
            if (!include_hidden && entry.path().filename().string().starts_with(".")) {
                continue;
            }
            paths.push_back(entry.path());
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return paths;
}

// Function to list all files in a directory
std::vector<std::filesystem::path> list_files_in_directory(const std::filesystem::path &path) {
    return list_files_and_directories(path, false); // Exclude hidden files by default
}

// Function to list all files in a directory that match a regex pattern
std::vector<std::filesystem::path> list_files_matching_regex(const std::filesystem::path &path, const std::regex &pattern) {
    std::vector<std::filesystem::path> all_files = list_files_in_directory(path);  // This function should return std::vector<std::filesystem::path>
    std::vector<std::filesystem::path> matching_files;

    // Iterate through all files and match with regex pattern
    for (const std::filesystem::path &file : all_files) {
        if (std::regex_search(file.string(), pattern)) {  // Use .string() to compare with regex (regex works on strings)
            matching_files.push_back(file);
        }
    }

    return matching_files;
}
