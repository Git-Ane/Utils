#include "date.hpp"



namespace GitAne{
    namespace Date{

        string get_date(){
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t);
            return to_string(now->tm_year+1900)+"-"+to_string(now->tm_mon+1)+"-"+to_string(now->tm_mday)+"__"+to_string(now->tm_hour)+":"+to_string(now->tm_min)+":"+to_string(now->tm_sec);
        }

        unordered_map<string,string> add_date_to_commit(unordered_map<string,string> k){
            k["#date"] = get_date();
            return k;
        }


        void date_loader(){
            GitAne::Console::addCommitFun(add_date_to_commit);
        }
    }
}

