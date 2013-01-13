
#include "root.h"

int main(int argc, char ** argv)
{
  FILE * fd;
  FILE * fifo_fd;
  int ch, status;
  int num_rows = 0;
  pid_t sm0, child;
  char upbound[10];
  char returnbuf1[20];
  char returnbuf2[20];
  char returnbuf3[20];
  char returnbuf4[20];

  // Open file in arg4
  if((fd = fopen(argv[4], "rt")) == 0)
    {
      perror("Could not open file\n");
    }  
  
  // Count the number of records, then close file
  while ((ch = fgetc(fd)) != EOF)
    {
      if (ch == '\n') ++num_rows;        
    }
  fclose(fd);

  // make fifo in current path
  if((mkfifo("/tmp/rootfifo", 0666)) == -1)
    {
      perror("Could not create fifo");
    }

  // fork off sm0
  if (fork() == 0)       // if child
    {
      // numrows in to a string
      sprintf (upbound, "%d", num_rows);
      // execute splitmerge
      execl("./splitmerge", "splitmerge", "0", argv[6], "0", "1", upbound, argv[4], argv[2], upbound,  NULL);
      perror("Could not execute splitmerge, check for files existence in current directory.\n");  //error check
    }
  
  else               // if adult
    {
      if( ( fifo_fd = fopen("/tmp/rootfifo", "rt") ) == 0 )
	{
	  perror("Error reading fifo @ root\n");
	}
      
      // Now read the pipe
      while ((fscanf(fifo_fd, "%s %s %s %s", returnbuf1, returnbuf2, returnbuf3, returnbuf4)) != EOF)
	{
	  printf("%s %s %s %s\n", returnbuf1, returnbuf2, returnbuf3, returnbuf4);  
	}
      fclose(fifo_fd);      
    }

  if ( fork() == 0 )
    {
      execl("./remove.sh", "./remove.sh", NULL);
      perror("Error removing extraneous files\n");
    }


  wait(&status);  

  printf("=====================================================\n");
  if (fork() == 0)
    {
      execlp("cat", "cat", "timeData.txt", NULL);
      perror("Error reading time-data file\n");
    }

  wait(&status);
  if (fork()==0)
    {
      execlp("rm", "rm", "-f", "timeData.txt", NULL);
      perror("Error removing time-data file");
    }


  return 0;
  
}
