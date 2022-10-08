/************************************************************************
** MapReduce_Phase1.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/6/2022 at 12:00 AM
**
** This file is the "executive" component
** 
** This component will handle all the command-line options
** and kick off the workflow component.
**
************************************************************************/
// include some library files
#include <iostream>
#include <stdio.h>
#include "FileIO.h"
// end libarary

using namespace FileIO;
using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("\nNo arguments were passed.\n");
    } else if (argc == 4) {
        FileIOManager fileIO(argv[1], argv[2], argv[3]);
        fileIO.read();
    } else {
        printf("\nWrong number of arguments\n");
    }
    return(0);
}
