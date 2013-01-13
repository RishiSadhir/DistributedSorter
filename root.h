
#ifndef _root_h_
#define _root_h_


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define STATLENGTH 40
#define MAXINPUT 100000

typedef struct
{
  char ssn[STATLENGTH];
  char last[STATLENGTH];
  char first[STATLENGTH];  
  char income[STATLENGTH];
} Record;

#endif
