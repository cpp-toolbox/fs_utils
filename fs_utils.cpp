#include "fs_utils.hpp"
#include <iostream>

std::string get_directory_from_filepath(const std::string &filepath) {
    std::filesystem::path path(filepath);
    return path.parent_path().string();
}

bool has_extension(const std::string &file_path, const std::string extension) {
    // Get the file extension
    std::filesystem::path path(file_path);
    std::string file_extension = path.extension().string();

    // Compare extensions (case insensitive)
    return (file_extension == extension ||
            (file_extension.size() > 1 && file_extension[0] == '.' && file_extension.substr(1) == extension));
}

std::string get_parent_directory(const std::string &current_dir) {
    std::filesystem::path path(current_dir);

    // Check if the current directory is the root directory
    if (path == path.root_path()) {
        // Already at the root directory, do nothing
        return current_dir;
    }

    // Get the parent directory
    path = path.parent_path();
    return path.string();
}

std::string get_home_directory() {
#ifdef _WIN32
    const char *home_drive = std::getenv("HOMEDRIVE");
    const char *home_path = std::getenv("HOMEPATH");
    if (home_drive && home_path) {
        return std::string(home_drive) + home_path;
    }
    return std::getenv("USERPROFILE") ? std::getenv("USERPROFILE") : "";
#else
    const char *home = std::getenv("HOME");
    return home ? home : "";
#endif
}

std::vector<std::string> list_files_and_directories(const std::string &path, bool include_hidden) {
    std::vector<std::string> paths;
    try {
        for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(path)) {
            // Check if the file is hidden (starts with a dot on Unix-like systems)
            if (!include_hidden && entry.path().filename().string().starts_with(".")) {
                continue;
            }
            paths.push_back(entry.path().string());
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return paths;
}

/**
 * @brief Checks if a given path is a directory.
 *
 * @param path The path to check.
 * @return true if the path is a directory, false otherwise.
 */
bool is_directory(const std::string path) {
    try {
        if (std::filesystem::exists(path)) {            // Check if the path exists
            return std::filesystem::is_directory(path); // Check if it's a directory
        }
        std::cerr << "Error: Path does not exist." << std::endl;
        return false; // Return false if the path doesn't exist
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false; // Return false if an exception occurs
    }
}

// Function to list all files in a directory
std::vector<std::string> list_files_in_directory(const std::string &path) {
    return list_files_and_directories(path, false); // Exclude hidden files by default
}

// Function to list all files in a directory that match a regex pattern
std::vector<std::string> list_files_matching_regex(const std::string &path, const std::regex &pattern) {
    std::vector<std::string> all_files = list_files_in_directory(path);
    std::vector<std::string> matching_files;

    // Iterate through all files and match with regex pattern
    for (const std::string &file : all_files) {
        if (std::regex_search(file, pattern)) {
            matching_files.push_back(file);
        }
    }

    return matching_files;
}
