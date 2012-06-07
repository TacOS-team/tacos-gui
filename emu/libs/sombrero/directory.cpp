#include <directory.h>

#include <string.h>
#include <dirent.h>
#include <algorithm>

namespace sombrero {

/*
 * Return true if a < b
 */
bool stringCompare(const std::string & a, const std::string & b) {
  int maxSize = std::max(a.length(), b.length());
  for(int i = 0; i < maxSize; ++i) {
    if(a[i] < b[i]) {
      return true;
    } else if(b[i] < a[i]) {
      return false;
    }
  }
  return a.length() < b.length();
}

class FileInfoComparator {
 public:
  bool operator()(const FileInfo & a, const FileInfo & b) {
    return stringCompare(a.getFileName(), b.getFileName());
  }
};

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

std::vector<std::string> Directory::entryList(bool sorted /* = true */) {
  std::vector<std::string> res;
  std::vector<FileInfo> files = this->entryInfoList(sorted);
  for(std::vector<FileInfo>::iterator it = files.begin(); it != files.end(); ++it) {
    res.push_back(it->getFileName());
  }
  return res;

}

std::vector<FileInfo> Directory::entryInfoList(bool sorted /* = true */) {
  std::vector<FileInfo> res;
  if(this->fileInformations.isDir()) {
    struct dirent *dp;
    DIR *dirp = opendir(this->fileInformations.getAbsolutePath().c_str());
    while ((dp = readdir(dirp)) != NULL) {
      if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
        res.push_back(FileInfo(this->fileInformations.getAbsolutePath() + "/" +std::string(dp->d_name)));
      }
    }
    closedir(dirp);
  }// @TODO else throw exception
  if(sorted) {
    FileInfoComparator comparator;
    std::sort(res.begin(), res.end(), comparator);
  }
  return res;
}

} // namespace sombrero
