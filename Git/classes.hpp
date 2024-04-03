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
namespace fs = std::filesystem;

using namespace std;

void create_dir(fs::path);
ofstream create_file(fs::path);

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



    class GitObject
    {
    public:

        string fmt="";

        GitObject(string data="");

        virtual string serialize(GitRepo repo);

        virtual void deserialize(string data);

        void init();
    };

    class GitBlob : public GitObject
    {
    public:

        GitBlob(const std::string& data) : GitObject(data) {}

        string fmt = "blob";

        virtual string serialize(GitRepo repo) override;     //pas sur de si il faut mieux mettre ou ne pas mettre d'argument (telle est la question mdr)

        virtual void deserialize(string data) override;

    private:
        string blobdata;
    };

    class GitCommit : public GitObject      // 1/10 the man the myth the legend !
    {
    public:

        string fmt = "commit";

        virtual string serialize(GitRepo repo);

        virtual void deserialize(string data);

        void init();
    };

    class GitTag : public GitAne::GitCommit{
    public:
        string fmt = "tag";

        virtual string serialize(GitRepo repo);

        virtual void deserialize(string data);
    };      

    class GitTree : public GitObject{
        virtual string serialize(GitRepo repo);

        virtual void deserialize(string data);

        void init();
    };

    class GitTreeLeaf{
        public:
            GitTreeLeaf(string modearg, string patharg, string shaarg);
        private:
            string mode;
            string path;
            string sha;         //pas sur que les sha se stockent avec un string mais bon
    };

    class GitIndexEntry{
        public:
            GitIndexEntry(int ctimearg, int mtimearg, int devarg, int inoarg, int mode_typearg, int mode_permsarg, int uidarg,int gidarg,int fsizearg, string shaarg, string namearg);
        private:
            //derniere fois ou la metadata du fichier a change
            pair<int,int> ctime;
            //derniere fois ou la data du fichier a change
            pair<int,int> mtime;
            //The ID of device containing this file
            int dev;
            //The file's inode number
            int ino;
            //The object type, either b1000 (regular), b1010 (symlink), b1110 (gitlink).
            int mode_type;
            //les perms (devinable)
            int mode_perms;
            //id of user
            int uid;
            //group id of owner
            int gid;
            int fsize;
            string sha;
            string name;
    };

    class GitIndex{
        public:
            GitIndex(int versionarg, vector<GitIndexEntry> entriesarg);
        private:
            int version;
            vector<GitIndexEntry> entries;
    };

    class GitIgnore{
        private:
            vector<string> absolute;
            map<string,string> scoped;      //pas sur du type (ben j'avoue un type avec rien en public jsuis pas sur mdr)
    };

    /* Mes propositions */
    /*
    Le seul problème c'est les dossiers:
    - Si les dossiers sont des fichiers, il faut faire un GitIndexTree avec pour feuille les fichiers & pour noeud les dossiers
    - Si GitIndexEntry c'est uniquement les fichiers alors les types sont ok normalement.
    */
    class GitIndexEntryServer{
        public:
            GitIndexEntryServer(int ctimearg, int mtimearg, int uidarg,int gidarg,int fsizearg, string shaarg, string contenu, string namearg);
        private:
            //derniere fois ou la metadata du fichier a change
            pair<int,int> ctime;
            //derniere fois ou la data du fichier a change
            pair<int,int> mtime;
            
            //id of user
            int uid;
            //group id of owner
            int gid;
            int fsize;
            string sha;
            string contenu; // Seulmement si le contenu est pas retrouvable depuis le sha.
            string name;
    };

    class GitIndexEntryClient{
        public:
            GitIndexEntryClient(int ctimearg, int mtimearg, int uidarg,int gidarg,int fsizearg, string shaarg, string path, string namearg);
        private:
            //derniere fois ou la metadata du fichier a change
            pair<int,int> ctime;
            //derniere fois ou la data du fichier a change
            pair<int,int> mtime;
            
            //id of user
            int uid;
            //group id of owner
            int gid;
            int fsize;
            string sha;
            string path;
            string name;
    };

    bool write_to_git_object(GitRepo repo, GitBlob*);

    GitBlob* read_object(GitRepo repo, string sha);


    /* Idée:
    - Le serveur stocke tout le contenu du fichier
    - Le client stocke que le path, ça sert à rien de stocker 2x l'informations.
    - Quand le client pull, gitane modifie le fichier à l'url indiqué par le path
    - Quand le client commit, gitane envoie le contenu du fichier à l'url indiqué par le path
    */

}

#endif

