#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <filesystem>
#include <string>
#include <vector>
#include <regex>

std::string normalize_path_for_os(const std::string &path);
std::string get_path_delimiter();
std::filesystem::path expand_tilde(const std::filesystem::path &path);
std::filesystem::path get_directory_from_filepath(const std::filesystem::path &filepath);
std::string get_containing_directory(const std::string &filepath);
std::filesystem::path get_parent_directory(const std::filesystem::path &current_dir);
std::filesystem::path get_home_directory();

bool has_extension(const std::filesystem::path &file_path, const std::string &extension);

std::vector<std::filesystem::path> list_files_and_directories(const std::filesystem::path &path,
                                                              bool include_hidden = false);
std::vector<std::filesystem::path> list_files_in_directory(const std::filesystem::path &path);
std::vector<std::filesystem::path> list_files_matching_regex(const std::filesystem::path &path,
                                                             const std::regex &pattern);
std::vector<std::filesystem::path> rec_get_all_files(const std::string &base_dir,
                                                     const std::vector<std::string> &ignore_dirs, int limit = 1000);

bool file_exists_in_same_dir(const std::filesystem::path &file_path, const std::filesystem::path &target_file_name);

#endif // FS_UTILS_HPP
