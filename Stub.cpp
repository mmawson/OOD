#include "Stub.hpp"

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <dlfcn.h>
#include "Messages.hpp"
#include "Functions.h"
#include "FileIO.hpp"

/* Socket Programming Libaries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
/* End Socket Programing Libraries */

namespace MapReduce
{

  Stub::Stub(const size_t port)
  {
    if (!ListenForConnection(port)) { std::cout << "Error when listening for connection" << std::endl; }

    //Keep listening until the controller sends a termination message
    bool terminate = false;
    while (terminate == false)
    {
      terminate = ListenForMessages();
    }
  }

  Stub::~Stub()
  {
  }

  bool Stub::ListenForConnection(const size_t port)
  {
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    //Create socket file descriptor
    if ((mListeningSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("socket failed");
      return false;
    }

    //Forcefully attahing socket to the port
    if (setsockopt(mListeningSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
      perror("setsockopt"); 
      return false;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(mListeningSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
      perror("bind failed");
      return false;
    }
    if (listen(mListeningSocket, 3) < 0) 
    {
      perror("listen");
      return false;
    }

    if ((mConnectedSocket = accept(mListeningSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
      perror("accept");
      return false;
    }

    std::cout << "Created socket on port " << port << std::endl;

    return true;
  }

  bool Stub::ListenForMessages()
  {
    char buffer[1024] = { 0 };
    int valread = read(mConnectedSocket, buffer, 1024); 

    if (valread == 0)
    {
      return false;
    }

    //TODO: Take the appropriate actions after receiving these messages

    ControllerMessage message;
    message.Deserialize(buffer);

    if (message.type == ControllerMessageType::CREATE_MAP_INSTANCE)
    {
      std::cout << "Got a message to create a map instance" << std::endl;
      mapProcess();
      return false;
    }
    else if (message.type == ControllerMessageType::CREATE_REDUCE_INSTANCE)
    {
      std::cout << "Got a message to create a reduce instance" << std::endl;
      reduceProcess();
      return false;
    }
    else if (message.type == ControllerMessageType::TERMINATE_STUB)
    {
      std::cout << "Got a message to terminate this stub!" << std::endl;
      return true;
    }

    return false;
  }

  maker_t* Stub::accessMapLib() {
      std::string pathToMapLib = "./lib/libmapNew.so";
        void* mapPtr = dlopen(pathToMapLib.c_str(), RTLD_LAZY);
        if(!mapPtr){
            cerr <<"Cannot load library: "<< dlerror() <<'\n';
        }
        dlerror();
//    maker_t defined in Functions class, declare as a pointer and typecast output of dlsym
//    maker function creates new instance of map
        maker_t *tempFuncMap = (maker_t *) dlsym(mapPtr, "maker");
        const char* dlsym_error = dlerror();
        if (dlsym_error) {
            cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        }
        return tempFuncMap;
    }


    auto mapFunctionThread = [] (auto fileIoPtr, string mapOutputFileName) { //  pointer to type Functions to allow access to all derived functions
        maker_t* FuncMap = Stub::accessMapLib();
        Functions* map1 = FuncMap(fileIoPtr, mapOutputFileName); // create a map object with a handel to the FileIOManager
        vector<string> mapOutput;
        mapOutput = map1->mapToOutputFile(fileIoPtr->getTempFileLines());  // the thread gets the files and maps the string tokens
        fileIoPtr->save(mapOutput,mapOutputFileName,fileIoPtr->getTempDir());  // saves the tokens in a temp file
    };

  void Stub::mapProcess(){
    for (int a = 1; a <= R; a++) {
        // The FileIOManager class handles FileIO
          // creates a FileIOManager object
        MapReduce::FileIOManager fileIO;
        fileIO.populateFiles(a, R);  // This iteratively opens all the files in batch "a" in the input directory and
        // saves the contents in a private string vector (tempFileLines) using a call to FileIOManager::read()
        // Now all the plays, poems, and sonnets are in one large string vector
        std::string mapOutputFile = "shakesTemp.txt" + to_string(a);
        // Map. whose job it is to take text lines passed to it and properly format them for the Reduce class to use
        auto filePtr = make_shared<MapReduce::FileIOManager>(fileIO);  // create shared pointer to pass to map object
        // Create a thread and pass it the
        std::thread mapThread(mapFunctionThread, filePtr, mapOutputFile);
        mapThread.join();
    }
}

    auto reduceFunctionThread = [] (auto fileIoPtr, string reduceInputFile, int b) { //  pointer to type Functions to allow access to all derived functions
      int R = 2;
      std::string pathToReduceLib = "./lib/libreduceNew.so";
        fileIoPtr->sortMap(reduceInputFile);
        auto holdingMap = fileIoPtr->getHoldingMap();

        // open .so file and assign to void pointer, RTLD_LAZY binds functions as called

        void* reducePtr = dlopen(pathToReduceLib.c_str(), RTLD_LAZY);
        if(!reducePtr){
            cerr <<"Cannot load library: "<< dlerror() <<'\n';
            return 1;
        }
        dlerror();
        maker_tRed *FuncReduce = (maker_tRed*) dlsym(reducePtr, "makerRed");
        const char* dlsym_error1 = dlerror();
        if (dlsym_error1) {
            cerr << "Cannot load symbol create: " << dlsym_error1 << '\n';
            return 1;
        }
        Functions* reduce1 = FuncReduce(fileIoPtr);
        vector<string> reduceOutput;
        reduce1->reduceFile(holdingMap);
        reduceOutput = reduce1->writeReduce();
        // If b == R we are processing our last temp file and saving SUCCESS.txt
        if (b == R) {
            fileIoPtr->save(reduceOutput,"SUCCESS.txt", fileIoPtr->getOutputDir());
        }
            // If b!=R we are saving bucket r to file
        else {
            fileIoPtr->save(reduceOutput,"reducedPiece" + to_string(b) + ".txt", fileIoPtr->getOutputDir());
        }
        return 0;
    };

    void Stub::reduceProcess(){
    MapReduce::FileIOManager fileIO;  // creates a FileIOManager object
    auto filePtr = make_shared<MapReduce::FileIOManager>(fileIO);  // create shared pointer to pass to map object
    for (int b = 1; b <= R; b++) {
                // The FileIOManager class handles FileIO
                std::string reduceInputFile = "shakesTemp.txt" + to_string(b);
                std::thread reduceThread(reduceFunctionThread, filePtr, reduceInputFile, b);
                reduceThread.join();
            }
}


}//namespace MapReduce
