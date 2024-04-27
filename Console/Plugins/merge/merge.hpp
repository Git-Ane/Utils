
#include "../../commands.hpp"
#include "../../../SHA-warma/shawarma.hpp"
#include "../../../Git/classes.hpp"
#include<iostream>
#include <vector>
#include<string>
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <openssl/sha.h>
#include <zlib.h>
#include <unordered_map>
#include <set>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

namespace GitAne{

    namespace Merge{
        void remove_merge(string hash);
        void validate_and_merge(const string& target_branch);
        void check_merge_conflicts(const string&);
        void merge_plugin_loader();
    }


}