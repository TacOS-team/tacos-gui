#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__
/**
 * @file directory.h
 * Directory class definition
 */

#include <string>
#include <fileinfo.h>
#include <list>


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

  std::list<std::string> entryList();
  std::list<FileInfo> entryInfoList();
};

} // namespace sombrero

#endif//__DIRECTORY_H__
