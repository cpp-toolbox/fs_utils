#ifndef FS_UTILS_HPP
#define FS_UTILS_HPP

#include <filesystem>
#include <string>
#include <vector>

/**
 * @file fs_utils.hpp
 * @brief Filesystem utility functions for common path and file operations.
 */

namespace fs_utils {

/**
 * @brief Check if a filesystem path exists.
 * @param path Path string to check.
 * @return true if the path exists, false otherwise.
 */
bool path_exists(const std::string &path);

/**
 * @brief Normalize a path string to match the current operating system's conventions.
 *
 * Converts delimiters and resolves inconsistencies.
 *
 * @param path Input path string.
 * @return Normalized path string.
 */
std::string normalize_path_for_os(const std::string &path);

/**
 * @brief Get the directory delimiter for the current operating system.
 *
 * @return "/" on POSIX systems, "\\" on Windows.
 */
std::string get_path_delimiter();

/**
 * @brief Expand a leading tilde (~) to the user's home directory.
 * @param path Path potentially starting with "~".
 * @return Expanded filesystem path.
 */
std::filesystem::path expand_tilde(const std::filesystem::path &path);

/**
 * @brief Extract the directory component from a full file path.
 * @param filepath Full file path.
 * @return Directory containing the file.
 */
std::filesystem::path get_directory_from_filepath(const std::filesystem::path &filepath);

/**
 * @brief Get the containing directory as a string.
 * @param filepath Full file path.
 * @return Directory containing the file, as a string.
 */
std::string get_containing_directory(const std::string &filepath);

/**
 * @brief Extract the filename from a full path string.
 * @param path_str Input path string.
 * @return Filename portion of the path.
 */
std::string get_filename_from_path(const std::string &path_str);

/**
 * @brief Get the parent directory of a given path.
 * @param current_dir Input path.
 * @return Parent directory of the input path.
 */
std::filesystem::path get_parent_directory(const std::filesystem::path &current_dir);

/**
 * @brief Get the home directory of the current user.
 * @return Filesystem path to the home directory.
 */
std::filesystem::path get_home_directory();

/**
 * @brief Check if a file path has a specific extension.
 * @param file_path Path to the file.
 * @param extension Extension to check (with or without dot).
 * @return true if the file has the specified extension, false otherwise.
 */
bool has_extension(const std::filesystem::path &file_path, const std::string &extension);

/**
 * @brief List files and directories in a given path.
 * @param path Directory to list.
 * @param include_hidden Whether to include hidden files (default: false).
 * @return Vector of filesystem paths.
 */
std::vector<std::filesystem::path> list_files_and_directories(const std::filesystem::path &path,
                                                              bool include_hidden = false);

/**
 * @brief List only files in a given directory (non-recursive).
 * @param path Directory to list.
 * @return Vector of file paths.
 */
std::vector<std::filesystem::path> list_files_in_directory(const std::filesystem::path &path);

/**
 * @brief List files matching a regular expression pattern.
 * @param path Directory to search.
 * @param pattern_str Regular expression pattern as a string.
 * @return Vector of matching file paths.
 */
std::vector<std::filesystem::path> list_files_matching_regex(const std::filesystem::path &path,
                                                             const std::string &pattern_str);

/**
 * @brief Recursively get all files from a base directory.
 * @param base_dir Directory to search.
 * @param ignore_dirs Directories to exclude.
 * @param limit Maximum number of files to return (default: 1000).
 * @return Vector of file paths.
 */
std::vector<std::filesystem::path> rec_get_all_files(const std::string &base_dir,
                                                     const std::vector<std::string> &ignore_dirs, int limit = 1000);

/**
 * @brief Check if a target file exists in the same directory as another file.
 * @param file_path Path to a reference file.
 * @param target_file_name Name of the target file.
 * @return true if the target file exists in the same directory, false otherwise.
 */
bool file_exists_in_same_dir(const std::filesystem::path &file_path, const std::filesystem::path &target_file_name);

/**
 * @brief Create a new directory.
 * @param dir_path Path of the directory to create.
 * @return true if the directory was created, false otherwise.
 */
bool create_directory(const std::filesystem::path &dir_path);

/**
 * @brief Create a new file and write content to it.
 * @param file_path Path of the file to create.
 * @param content Content to write into the file.
 * @return true if the file was created successfully, false otherwise.
 */
bool create_file_with_content(const std::filesystem::path &file_path, const std::string &content);

/**
 * @brief Get the directory containing a given file.
 * @param file_path File path.
 * @return Directory containing the file, as a string.
 */
std::string get_directory_of_file(const std::string &file_path);

/**
 * @brief Get the relative path from a source to a target.
 * @param source Base path.
 * @param target Target path.
 * @return Relative path from source to target.
 */
std::filesystem::path get_relative_path(const std::filesystem::path &source, const std::filesystem::path &target);

} // namespace fs_utils

#endif // FS_UTILS_HPP
