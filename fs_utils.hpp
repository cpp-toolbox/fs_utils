#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <filesystem>
#include <string>
#include <vector>
#include <regex>

std::string get_directory_from_filepath(const std::string &filepath);
std::string get_parent_directory(const std::string &current_dir);
std::string get_home_directory();

bool is_directory(const std::string path);
bool has_extension(const std::string &file_path, const std::string extension);

std::vector<std::string> list_files_and_directories(const std::string &path, bool include_hidden = false);
std::vector<std::string> list_files_in_directory(const std::string &path);
std::vector<std::string> list_files_matching_regex(const std::string &path, const std::regex &pattern);

#endif // FS_UTILS_HPP
