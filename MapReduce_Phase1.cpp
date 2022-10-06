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

#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("No arguments were passed.\n");
    } else {
        printf( "Arguments: " );
            for ( int i = 1; i < argc; ++i ) {
                    printf( "  %d. %s\n", i, argv[i] );
                }
    }
    return 0;
}
