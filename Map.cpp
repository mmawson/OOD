/************************************************************************
** Map.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/16/2022 at 8:36 PM
**
** This file is the source file for the Map class, whose job it is to take
** text lines passed to it and properly format them for the Reduce class to use
************************************************************************/

#include "Map.hpp"
#include "FileIO.hpp"
#include <cctype>
#include <vector>
#include <iostream>

namespace MapReduce
{

Map::Map(std::shared_ptr<FileIOManager> fileIOMgr, const std::string& outputFile) : mFileIOMgr(fileIOMgr), mOutputFile(outputFile)
{
  
}

void Map::mapToOutputFile(const std::vector<std::string>& inputLines)
{
  std::vector<std::string> tokenizedData;

  for (const std::string& line : inputLines)
  {
    for (const std::string& token : tokenizeLine(line))
    {
      tokenizedData.push_back(token);
    }
  }

  mFileIOMgr->save(tokenizedData, mOutputFile, mFileIOMgr->getTempDir());
}

std::vector<std::string> Map::tokenizeLine(const std::string& lineStr)
{
  //Remove punctuation, whitespace, and capitalization
  //And format as ("word", 1)
  std::vector<std::string> tokenizedLine;
  std::string token = "";
  for (int i = 0; i < lineStr.size(); ++i)
  {
    if (std::isspace(lineStr[i]) && token != "") 
    {
      tokenizedLine.push_back("(\"" + token + "\", 1)");
      token = "";
    }
    else if (!std::ispunct(lineStr[i]) && !std::isspace(lineStr[i]))
    {
      token += std::tolower(lineStr[i]);
    }
  }

  //Push last word
  if (token != "")
  {
      tokenizedLine.push_back("(\"" + token + "\", 1)");
  }
 
  return tokenizedLine;
}

} //namespace MapReduce
