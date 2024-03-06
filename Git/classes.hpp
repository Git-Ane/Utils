#include <string>
#include <map>
#include <vector>

using namespace std;


namespace Gitane{
    class GitRepo
    {
    private:
        string worktree;
        string gitdir;
        string conf;
    public:
        GitRepo(string path, bool force=false);
    };

    class GitObject
    {
    public:
        GitObject(string data="");

        string serialize(GitRepo repo);

        void deserialize(string data);

        void init();
    };

    class GitBlob : public GitObject
    {
        string serialize();     //pas sur de si il faut mieux mettre ou ne pas mettre d'argument (telle est la question mdr)

        void deserialize(string data);
    };

    class GitCommit : public GitObject      //the man the myth the legend !
    {
        string serialize(GitRepo repo);

        void deserialize(string data);

        void init();
    };

    class GitTag : public GitCommit{};      //g pas compris a koi ca servait mdr

    class GitTree : public GitObject{
        string serialize(GitRepo repo);

        void deserialize(string data);

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
            map<string,string> scoped;      //pas sur du type
    };

}
