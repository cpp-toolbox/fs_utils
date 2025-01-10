#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <filesystem>
#include <string>
#include <vector>
#include <regex>

std::filesystem::path get_directory_from_filepath(const std::filesystem::path &filepath);
std::filesystem::path get_parent_directory(const std::filesystem::path &current_dir);
std::filesystem::path get_home_directory();

bool has_extension(const std::filesystem::path &file_path, const std::string &extension);

std::vector<std::filesystem::path> list_files_and_directories(const std::filesystem::path &path, bool include_hidden = false);
std::vector<std::filesystem::path> list_files_in_directory(const std::filesystem::path &path);
std::vector<std::filesystem::path> list_files_matching_regex(const std::filesystem::path &path, const std::regex &pattern);

#endif // FS_UTILS_HPP
