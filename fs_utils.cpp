#include "fs_utils.hpp"
#include <iostream>
#include <iostream>
#include <unordered_set>
#include <cstdlib> // for std::getenv

std::string normalize_path_for_os(const std::string &path) {	
    std::filesystem::path fp(path);

#if defined(_WIN32) || defined(_WIN64)
    fp.make_preferred(); // Converts slashes to backslashes on Windows
    return fp.string();  // Windows case
#else
    std::string normalized = fp.string();
    std::replace(normalized.begin(), normalized.end(), '\\', '/');
    return normalized;
#endif
}

std::string get_path_delimiter() {
    return std::string(1, std::filesystem::path::preferred_separator);
}


std::filesystem::path expand_tilde(const std::filesystem::path &path) {
    if (path.empty() || path.string()[0] != '~') {
        return path;
    }

    const char *home_dir = std::getenv("HOME");
    if (home_dir) {
        std::string expanded_path = home_dir + std::string(path.string().substr(1)); // Remove '~' and append to HOME
        return std::filesystem::path(expanded_path);
    }

    return path; // Return the original path if HOME environment variable is not found
}

std::vector<std::filesystem::path> rec_get_all_files(const std::string &base_dir,
                                                     const std::vector<std::string> &ignore_dirs, int limit) {
    std::vector<std::filesystem::path> files;
    std::unordered_set<std::string> ignore_set(ignore_dirs.begin(), ignore_dirs.end());
    size_t count = 0;

    try {
        std::filesystem::recursive_directory_iterator dir_iter(
            base_dir, std::filesystem::directory_options::skip_permission_denied);

        while (dir_iter != std::filesystem::end(dir_iter)) {
            const auto &entry = *dir_iter;

            if (entry.is_directory()) {
                auto dir_name = entry.path().filename().string();
                /*std::cout << "Looking at: " << dir_name << std::endl;*/

                if (ignore_set.find(dir_name) != ignore_set.end()) {
                    /*std::cout << "Skipping: " << dir_name << " and its subdirectories" << std::endl;*/
                    dir_iter.disable_recursion_pending(); // Skip this directory and its subdirectories
                }
            } else if (entry.is_regular_file()) {
                std::string relative_path = entry.path().string();
                if (relative_path.rfind("./", 0) == 0) {     // Check if it starts with "./"
                    relative_path = relative_path.substr(2); // Remove the "./"
                }

                /*std::cout << "adding: " << relative_path << std::endl;*/
                files.push_back(relative_path);
                ++count;
                if (count >= limit) {
                    break;
                }
            }

            ++dir_iter; // Move to the next entry
        }
    } catch (const std::exception &e) {
        std::cerr << "Error while iterating directory: " << e.what() << std::endl;
    }

    return files;
}

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

std::string get_containing_directory(const std::string &filepath) {
    std::filesystem::path path_obj(filepath);
    std::filesystem::path directory = path_obj.parent_path();
    return normalize_path_for_os(directory.string());
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
std::vector<std::filesystem::path> list_files_matching_regex(const std::filesystem::path &path,
                                                             const std::regex &pattern) {
    std::vector<std::filesystem::path> all_files =
        list_files_in_directory(path); // This function should return std::vector<std::filesystem::path>
    std::vector<std::filesystem::path> matching_files;

    // Iterate through all files and match with regex pattern
    for (const std::filesystem::path &file : all_files) {
        if (std::regex_search(file.string(), pattern)) { // Use .string() to compare with regex (regex works on strings)
            matching_files.push_back(file);
        }
    }

    return matching_files;
}

bool file_exists_in_same_dir(const std::filesystem::path &file_path, const std::filesystem::path &target_file_name) {
    try {
        // Check if the given file exists
        if (!std::filesystem::exists(file_path)) {
            std::cerr << "The specified file does not exist: " << file_path << std::endl;
            return false;
        }

        // Ensure it's a regular file
        if (!std::filesystem::is_regular_file(file_path)) {
            std::cerr << "The specified path is not a regular file: " << file_path << std::endl;
            return false;
        }

        // Get the directory of the given file
        std::filesystem::path directory = file_path.parent_path();

        // Construct the full path to the target file
        std::filesystem::path target_file_path = directory / target_file_name;

        // Check if the target file exists
        if (std::filesystem::exists(target_file_path)) {
            return true; // Target file exists
        } else {
            return false; // Target file does not exist
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception &e) {
        std::cerr << "General error: " << e.what() << std::endl;
        return false;
    }
}
