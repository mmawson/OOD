//
// Created by mitchell on 11/8/22.
//

#ifndef OOD_DYNAMICLOADER_H
#define OOD_DYNAMICLOADER_H


#include <string>
#include <filesystem>
#include <vector>
#include <map>

class DynamicLoader {

public:
public:

    static constexpr int ERROR_BUF_SIZE = 80;

    DynamicLoader(std::filesystem::path);

    DynamicLoader(const DynamicLoader& src) = delete;


    void assignFuncVec(const char *, std::vector<std::string>);
    void assignFuncMapVec(const char *funcStr, std::map<std::string, std::vector<int>> textFile);
    void assignFuncMap(const char *funcStr, std::map<std::string, int> textFile);
    void populateLibs();


private:
    const std::string m_filename;
    const int         m_flags = 0;
    void*             libHandle = nullptr;
    std::filesystem::path libDirectory;
    std::vector<const char*> libFiles;




    void assignFunc(const char *funcStr, std::map<std::string, std::vector<int>> textFile);


};


#endif //OOD_DYNAMICLOADER_H
