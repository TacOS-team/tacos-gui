#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__
/**
 * @file directory.h
 * Directory class definition
 */

#include <string>
#include <fileinfo.h>
#include <vector>


namespace sombrero {

/**
 * Directory class definition
 * @brief The Directory class
 */ 
class Directory {

 protected:
  FileInfo fileInformations;

 public:
  Directory(const std::string & dirPath);

  FileInfo getInformations();

  bool cd(const std::string & dirPath);
  bool cdUp();

  std::vector<std::string> entryList(bool sorted = true);
  std::vector<FileInfo> entryInfoList(bool sorted = true);
};

} // namespace sombrero

#endif//__DIRECTORY_H__
