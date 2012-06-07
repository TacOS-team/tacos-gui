#ifndef __FILEINFO_H__
#define __FILEINFO_H__
/**
 * @file fileinfo.h
 * FileInfo class definition
 */

#include <string>
#include <sys/stat.h>


namespace sombrero {

/**
 * FileInfo class definition
 * @brief The FileInfo class
 */ 
class FileInfo {

 protected:
  std::string filePath;
  std::string absoluteFilePath;
  struct stat fileStats;
  bool fileExists;

 public:
  FileInfo(const std::string & filePath);

  void setFile(const std::string & filePath);

  std::string getFileName() const;
  std::string getAbsolutePath();

  bool exists();
  bool isDir();
  bool isFile();

};

} // namespace sombrero

#endif//__FILEINFO_H__
