#include <directory.h>

#include <string.h>
#include <dirent.h>

namespace sombrero {

Directory::Directory(const std::string & dirPath) : fileInformations(dirPath) {

}

FileInfo Directory::getInformations() {
  return this->fileInformations;
}

bool Directory::cd(const std::string & dirPath) {
  this->fileInformations.setFile(dirPath);
  return this->fileInformations.exists();
}

bool Directory::cdUp() {
  return this->cd(this->fileInformations.getAbsolutePath()+"/..");
}

std::list<std::string> Directory::entryList() {
  std::list<std::string> res;
  if(this->fileInformations.isDir()) {
    struct dirent *dp;
    DIR *dirp = opendir(this->fileInformations.getAbsolutePath().c_str());
    while ((dp = readdir(dirp)) != NULL) {
      if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
        res.push_back(std::string(dp->d_name));
      }
    }
    closedir(dirp);
  }// TODO else throw exception
  return res;
}

std::list<FileInfo> Directory::entryInfoList() {
  std::list<FileInfo> res;
  std::list<std::string> files = this->entryList();
  for(std::list<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
    res.push_back(FileInfo(this->fileInformations.getAbsolutePath() + "/" + *it));
  }
  return res;
}

} // namespace sombrero