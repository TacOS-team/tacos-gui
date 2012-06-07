#include <fileinfo.h>

#include <cstring>
#include <stdlib.h>
#include <unistd.h>

namespace sombrero {

FileInfo::FileInfo(const std::string & filePath) {
  this->setFile(filePath);
}

void FileInfo::setFile(const std::string & filePath) {
  this->filePath = filePath;
  if(stat(filePath.c_str(), &this->fileStats) != 0) {
    this->fileExists = false;
    this->absoluteFilePath = "";
  } else {
    this->fileExists = true;
    this->absoluteFilePath = realpath(filePath.c_str(), NULL);
  }
}

std::string FileInfo::getFileName() const {
  char * currentBegining = (char*)this->absoluteFilePath.c_str();
  char * nextSlash;
  while((nextSlash = strstr(currentBegining, "/")) != NULL) {
    currentBegining = nextSlash+1;
  }
  return std::string(currentBegining);
}

std::string FileInfo::getAbsolutePath() {
  return this->absoluteFilePath;
}

bool FileInfo::exists() {
  return this->fileExists;
}

bool FileInfo::isDir() {
  return this->fileExists && S_ISDIR(this->fileStats.st_mode);
}

bool FileInfo::isFile() {
  return this->fileExists && S_ISREG(this->fileStats.st_mode);
}

} // namespace sombrero
