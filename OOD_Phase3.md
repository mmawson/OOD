# OOD MapReduce Phase 3


# Background

MapReduce Phase 3, takes files provide as input and calls and concurrently splits them into smaller chunks. This phase builds on the previous iterations development and begins to incorporate concurrency into both Map and Reduce functions.

# Architecture

```mermaid
flowchart LR
input --> bucket1
input --> bucket2
input --> bucket3
bucket1 --> map1
bucket2 --> map2
bucket3 --> map3
map1 --> holdingMap1
map2 --> holdingMap1
map3 --> holdingMap1
map1 --> holdingMap2
map2 --> holdingMap2
map3 --> holdingMap2
map1 --> holdingMap3
map2 --> holdingMap3
map3 --> holdingMap3
holdingMap1 --> Reduce1
holdingMap2 --> Reduce2
holdingMap3 --> Reduce3
Reduce1 --> Ouput
Reduce2 --> Ouput
Reduce3 --> Ouput
```

Initially data is taken into the program and split amongst n buckets, these files are then passed to a mapper function. This mapper function is a .so file that is called by the executive. The output from the mapper function is then stored in a corresponding holdingMap file.
Once the map function has been completed the temp output is passed to the reduce function, for aggregating keys, and finally the output is saved to file.

```mermaid
sequenceDiagram
    participant User
    participant FileIO
    participant Map
    participant Reduce
    
    User->>+FileIO: Pass source/temp/output directories
    FileIO->>FileIO: Extract data from input files
    FileIO->>+Map: Pass file name and lines 
    Map->>Map: tokenize data 
    Map->>Map: Export function to write to disk
    FileIO->>FileIO: Sort Map output
    FileIO->>+Reduce: Open temp file and pass data
    Reduce->>Reduce: Iterate over values and sum 
    Reduce->>Reduce: Export to write to disk
     
```


# Design

## Class Diagram

```mermaid
classDiagram
    class MapReduce{
        auto mapFunctionThread
        auto reduceFunctionThread
    }
    class FileIO{
        std::filesystem::path sourceDir
        std::filesystem::path tempDir
        std::filesystem::path outputDir
        std::vector<std::filesystem::path> inputFiles
        std::vector<std::string> tempFileLines
        void read(std::filesystem::path&)
        std::vector<std::string> getTempFileLines()
        void populateFiles()
        void saveTemp(std::vector<std::string> & tokenizedTempVector)
        std::filesystem::path getSourceDir()
        std::filesystem::path getTempDir()
        std::filesystem::path getOutputDir()
        int toString()         
    }
    Functions <|-- Map
    Functions <|-- Reduce
    Functions: typedef Functions *maker_t
    Functions: typedef Functions *maker_tRed
    class Map{
        std::shared_ptr<FileIOManager> mFileIOMgr
        std::string mOutputFileName
        void mapToOutputFile(const std::vector<std::string>& inputLines)
        std::vector<std::string> tokenizeLine(const std::string& lineStr)
        extern "C" Functions *maker(std::shared_ptr<FileIOManager> fileIOMgr, const std::string& outputFile) 
    }
    class Reduce{
        std::vector<std::string> tokenizeLine(const std::string& lineStr)
        std::shared_ptr<FileIOManager> mFileIOMgr
        std::string mOutputFileName
        void sortMap(MapReduce::FileIOManager, std::filesystem::path)
        void reduceFile()
        void writeReduce(filesystem::path outputFile)
        extern "C" Functions *makerRed(std::shared_ptr<FileIOManager> fileIOMgr)
    }
```

In order to implement the above architecture, there are a number of classes implemented to deliver the required functionality:

- MapReduce - This is the executive that calls classes and controls the creation and destruction of Map and Reduce Threads
- FileIO - This class handles all File operations
- Functions - This is an abstract base class, for Map And Reduce
- Map - Shared library, that is a derived class of Functions
- Reduce - hared library, that is a derived class of Functions

What is key in Phase 3, is the inclusion of threads within the MapReduce executive. These allow concurrent calling of Map functions, and once complete the concurrency is continued through the creation of multiple threads to allow reduce to operate concurrently.  

