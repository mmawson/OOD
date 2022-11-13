//
// Created by mitchell on 11/12/22.
// THis class acts as an abstract base class for Map and Reduce
// Idea is to allow access to shared libraries via virtual functions

#ifndef OOD1_FUNCTIONS_H
#define OOD1_FUNCTIONS_H

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace MapReduce {
    class FileIOManager;

    class Functions {

    public:
        Functions();

        virtual void reduceFile(std::map<std::string, std::vector<int>>) = 0;

        virtual void writeReduce() = 0;

        virtual void mapToOutputFile(const std::vector<std::string> &) = 0;

        std::shared_ptr<MapReduce::FileIOManager> rFileIOMgr;
        std::shared_ptr<FileIOManager> mFileIOMgr;
        std::map<std::string, int> reduceTemp;
        std::string mOutputFile;

        virtual std::vector<std::string> tokenizeLine(const std::string &) = 0;

//        virtual Functions* createMap() {
//            return nullptr;
//        }

        virtual Functions* createReduce() {
            return nullptr;
        }

        void destroyMap(Functions*){};
        void destroyReduce(Functions*){};


        //class factories
        typedef Functions *create_t();

        typedef void destroy_t(Functions *);


    private:

    };

}
#endif //OOD1_FUNCTIONS_H


