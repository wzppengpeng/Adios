#ifndef WZP_PATH_HPP
#define WZP_PATH_HPP

#include <cstdio>

#include <string>
#include <vector>
#include <utility>

#include <unistd.h>
#include <sys/stat.h>

namespace wzp
{

class Path
{

public:
    /**
     * join the path, get the whole path
     * @param  paths [description]
     * @return       [description]
     */
    inline static std::string Join(const std::vector<std::string>& paths) {
        //wash the strs
        std::vector<std::string> paths_(paths);
        for(auto& str : paths_) {
            if(str.back() == '/') str.pop_back();
        }
        std::string whole_path;
        for(size_t i = 0; i < paths_.size() - 1; ++i) {
            whole_path.append(paths_[i]);
            whole_path.push_back('/');
        }
        whole_path.append(paths_.back());
        return std::move(whole_path);
    }

    /**
     * get the base name of a path
     * @param  path [description]
     * @return      [description]
     */
    inline static std::string Basename(const std::string& path) {
        std::string filename;
        size_t pos = path.find_last_of('/');
        if(pos != std::string::npos) filename = std::move(path.substr(pos+1));
        else filename = path;
        return std::move(filename);
    }

    /**
     * get the dir of a filename
     * @param  path the raw path of the file
     * @return      the dir name
     */
    inline static std::string Dirname(const std::string& path) {
        std::string dir;
        size_t pos = path.find_last_of('/');
        if(pos != std::string::npos) dir = std::move(path.substr(0, pos));
        else dir = path;
        return std::move(dir);
    }


    /**
     * split the ext of a filename
     */
    inline static std::pair<std::string, std::string> Splitext(const std::string& filename) {
        size_t pos = filename.find_last_of('.');
        return std::make_pair(filename.substr(0, pos), filename.substr(pos + 1));
    }


    /**
     * check if exists one file
     * @param  file string format file
     * @return      true or false
     */
    inline static bool Exists(const std::string& file) {
        return access(file.c_str(), F_OK) == 0;
    }


    /**
     * check if the file is executable
     * @param  file check file
     * @return      true or false
     */
    inline static bool IsExecutable(const std::string& file) {
        return access(file.c_str(), X_OK) == 0;
    }


    /**
     * check if a path is dir
     * @param  path the need to check path
     * @return      true or false
     */
    inline static bool IsDir(const std::string& path) {
        if(!Exists(path))
            return false;
        struct stat statbuf;
        lstat(path.c_str(), &statbuf);
        return S_ISDIR(statbuf.st_mode);
    }

};


} //wzp



#endif //WZP_PATH_HPP