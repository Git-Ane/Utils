#ifndef CLASSES_HPP
#define CLASSES_HPP


#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "../Console/commands.hpp"
#include "../SHA-warma/shawarma.hpp"
#include <openssl/sha.h>
#include <zlib.h>
#include <unordered_map>
#include <set>
#include <regex>
namespace fs = std::filesystem;

using namespace std;


namespace GitAne{



    class GitRepo
    {
    private:
        fs::path worktree; // URL de l'espace de travail
        fs::path gitdir; // worktree + .git
        fs::path conf; // dossier de config
    public:
        GitRepo(fs::path path, bool force=false); // force=true => le créer.
        fs::path get_gitdir();
    };

    GitRepo create_repo(fs::path);
    GitRepo repo_find(fs::path);
    void track_file(vector<string> args);
    void untrack_file(vector<string> args);
    vector<string> get_tracked_files(GitRepo repo);
    void checkoutcommit(vector<string> args);
    string get_head(GitRepo repo, bool ignore_temporary);
    void set_head(GitRepo repo, string sha);
    string get_parent(GitRepo repo, string sha);
    unordered_map<string,string> get_branches(GitRepo repo);
    string get_file_content(fs::path path);
    string get_active_branch(GitRepo repo);
    void set_active_branch(GitRepo repo, string branch);
    void create_branch(vector<string> args);
    string sha_of_position(GitRepo repo,string pos);
    unordered_map<string,string> open_commit(GitRepo repo, string sha);
    void show_history(vector<string> args);
    void status(vector<string>);
    bool made_changes(GitRepo repo);

    string get_hash_of_branch(string ,bool ignore_temporary);

    void create_dir(fs::path);
    ofstream create_file(fs::path);

    bool matchPattern(const std::string& filePath, const std::string& pattern);
    bool shouldIgnoreFile(const std::string& filePath, const std::vector<std::string>& patterns);
    std::vector<std::string> parseGitIgnore(const std::string& gitIgnorePath);



    class GitObject
    {
    public:

        string fmt="";

        GitObject(string data="");

        virtual string serialize(GitRepo repo);

        virtual void deserialize(string data);
        virtual string get_format();

        void init();
    };

    class GitBlob : public GitObject
    {
    public:

        GitBlob(const std::string& data) : GitObject(data) {}

        string fmt = "blob";
        virtual string get_format() override;
        virtual string serialize(GitRepo repo) override;     //pas sur de si il faut mieux mettre ou ne pas mettre d'argument (telle est la question mdr)

        virtual void deserialize(string data) override;

    private:
        string blobdata;
    };

    unordered_map<string, string> kvlm_parse(const string& raw);

    string kvlm_serialize(const unordered_map<string, string>& kvlm);

    class GitCommit : public GitObject      // 1/10 the man the myth the legend !
    {
    public:

        string fmt = "commit";

        virtual string serialize(GitRepo repo);
        virtual string get_format() override;
        virtual void deserialize(string data);

        void init();

        unordered_map<string, string> kvlm;
    };
  





    string write_to_git_object(GitRepo repo, GitObject&, string forcesha);
    void write_commit(string name, bool temporary,string second_parent, bool isempty);
    string read_object(GitRepo repo, string sha, bool write = false);

    bool isHidden(const fs::path& path);
    vector<string> listFiles();

    void move_branch();
    void delete_branch(string branch_name);
    void clean();


    /* Idée:
    - Le serveur stocke tout le contenu du fichier
    - Le client stocke que le path, ça sert à rien de stocker 2x l'informations.
    - Quand le client pull, gitane modifie le fichier à l'url indiqué par le path
    - Quand le client commit, gitane envoie le contenu du fichier à l'url indiqué par le path
    */

}

#endif

