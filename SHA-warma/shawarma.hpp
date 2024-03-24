#include<string>
#include<openssl/sha.h>
#include <map>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include<iomanip>
namespace GitAne{
std::string sha1(const std::string &input);
}