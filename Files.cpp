#include "headers/Methan0l.h"

std::string Files::readFile(std::string path) {
  std::ifstream input(path.c_str());
  std::string str, result;
  while (std::getline(input, str)) {
    result += str;
  }
  return result;
}

bool Files::writeFile(std::string filename, std::string arg) {
  std::ofstream fout(filename.c_str());
  fout << arg;
  fout.close();
  return true;
}
