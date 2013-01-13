/*
  ARGUMENTS:
  depth, sort_coumn, node_num, lowbound, upbound, filepath, max_Depth, file_size
  1       2           3          4            5     6         7            8
*/

#include "root.h"

// destructively sets r to the concatenation of a and b
void concatenate(char * r, char * a, char * b)
{
  strcpy(r, a);
  strcat(r, b);

}

char * prefix(char * s)
{
  int i=0;
  char tmp[strlen(s)+5];

  tmp[0] = '/';
  tmp[1] = 't';
  tmp[2] = 'm';
  tmp[3] = 'p';
  tmp[4] = '/';  
  for(i=5; i<strlen(s)+5; i++)
    {
      tmp[i] = s[i-5];
    }
  tmp[strlen(s)+5] = '\0';

  s = tmp;
  return s;
}


void extract(char * filepath, char * sortcol, int lb, int ub, char * pipe_to_sort, char * sort_to_pipe, int sorter, int rows)
{
  // Declarations
  FILE * records;
  FILE * pipe_sort;
  int curr_row = 1;
  char one[50], two[50], three[50], four[50], size[50];
  char buffer[200];
  char message[200];
  rows = ub - lb;
  // size = number of rows to be working on ~
  sprintf(size, "%d", rows);



  // Dependent on the value of sorter, call a specific sorting executable
  switch(sorter)
    {
    case 0:
      if(fork() == 0) /* shell sort */
	{

	  execl("shell", "shell", pipe_to_sort, "-o", sort_to_pipe, "-k", sortcol, size, NULL); // Finally works
	  printf("Couldnt call sort routine for %d to %d which has size %s\n", lb , ub, size);
	  perror("Error at 0: ");
	}
      break;
    case 1:
      if(fork() == 0) /* quick sort */
	{
	  execl("quick", "quick", pipe_to_sort, "-o", sort_to_pipe, "-k", sortcol, size, NULL);  
	  printf("Couldnt call sort routine for %d to %d which has size %s\n", lb , ub, size);
	  perror("Error at 1: ");
	}
      break;
    case 2:
      if(fork() == 0)  /* unix sort */
	{
	  execlp("sort", "sort", pipe_to_sort, "-o", sort_to_pipe, "-k", sortcol, NULL);  //Like a charm
	  printf("Couldnt call sort routine for %d to %d which has size %s\n", lb , ub, size);
	  perror("Error at 2: ");
	}
      break;
    case 3:
      if(fork() == 0)  /* bubble sort */
	{
	  execl("bubble", "bubble", pipe_to_sort, "-o", sort_to_pipe, "-k", sortcol, size, NULL);  
	  printf("Couldnt call sort routine for %d to %d which has size %s\n", lb , ub, size);
	  perror("Error at 3: ");
	}
      break;
    }

  // Write data to sorter
  pipe_sort = fopen(pipe_to_sort, "wt");
  // open file to be sorted
  records = fopen(filepath, "rt");
  // Write lines between the lower and upper bound to the sorter
  while ( (fscanf(records, "%s %s %s %s", one, two, three, four) ) != EOF )
    {
      if(curr_row >= lb && curr_row <= ub)
	{
	  fprintf(pipe_sort, "%s %s %s %s\n", one, two, three, four);
	}
      curr_row++;
    }
  // Close files
  fclose(records);
  fclose(pipe_sort);
}

// Just a wrapper for strcmp wrapper. Compares two records by an attribute
int compare(char * left, char * right, int col)
{
  int a, b;
  char onel[50], twol[50], threel[50], fourl[50];
  char oner[50], twor[50], threer[50], fourr[50];
  
  sscanf(left, "%s %s %s %s", onel, twol, threel, fourl);
  sscanf(right, "%s %s %s %s", oner, twor, threer, fourr);

  switch(col)
    {
    case 1:
      return (strcmp(onel, oner)>0);
      break;
    case 2:
      return (strcmp(twol, twor)>0);
      break;
    case 3:
      return (strcmp(threel, threer)>0);
      break;
    case 4:
      return (strcmp(fourl, fourr)>0);
      break;
    }
  return -1;
}

void merge(char * left_path, char * right_path, char * parent_path, int col) 
{
  // Declarations
  FILE * parent_pipe;
  FILE * left_pipe;
  FILE *right_pipe;
  char one[100], two[100], three[100], four[100];
  char line_left[150], line_right[150];

  // open pipes
  parent_pipe = fopen(parent_path, "wt");  // pipe to merge to 
  left_pipe = fopen(left_path, "rt");      // pipe to merge from
  right_pipe = fopen(right_path, "rt");    // pipe to merge from


  
   // Read first line from the two children
  if((fscanf(left_pipe, "%s %s %s %s", one, two, three, four)) == EOF)    // All processes will initially block here
    {
      strcpy(line_left, "EOF");
    }
  else
    {
      sprintf(line_left, "%s %s %s %s\n", one, two, three, four);
    }
  if((fscanf(right_pipe, "%s %s %s %s", one, two, three, four)) == EOF)
    {
      strcpy(line_right, "EOF");
    }
  else
    {
      sprintf(line_right, "%s %s %s %s\n", one, two, three, four);
    }
  // merge (in accordance with mergesort algorithm)
  while( strcmp(line_left, "EOF") != 0 || strcmp(line_right, "EOF") != 0 )
    {
      if ( strcmp(line_left,"EOF") != 0 && strcmp(line_right, "EOF") != 0)
	{
	  if (compare(line_right, line_left, col) == 1) // if left is less
	    {
	      fprintf(parent_pipe, "%s", line_left);
	      if((fscanf(left_pipe, "%s %s %s %s", one, two, three, four)) == EOF)
		{
		  strcpy(line_left, "EOF");
		}
	      else
		{
		  sprintf(line_left, "%s %s %s %s\n", one, two, three, four);
		}
	    }
	  else  // else right is less than or equal to so write it and check next 
	    {
	      fprintf(parent_pipe, "%s",line_right) ;
	      if((fscanf(right_pipe, "%s %s %s %s", one, two, three, four)) == EOF)
		{
		  strcpy(line_right, "EOF");
		}
	      else
		{
		  sprintf(line_right, "%s %s %s %s\n", one, two, three, four);
		}
	    }
	}
      
      else if (strcmp(line_left, "EOF") != 0)
	{
	  fprintf(parent_pipe, "%s",line_left) ;
	  if((fscanf(left_pipe, "%s %s %s %s", one, two, three, four)) == EOF)
	    {
	      strcpy(line_left, "EOF");
	    }
	  else
	    {
	      sprintf(line_left, "%s %s %s %s\n", one, two, three, four);
	    }
	}
      
      else // right is uneq EOF
	{
	  fprintf(parent_pipe, "%s", line_right);
	  if((fscanf(right_pipe, "%s %s %s %s", one, two, three, four)) == EOF)
	    {
	      strcpy(line_right, "EOF");
	    }
	  else
	    {
	      sprintf(line_right, "%s %s %s %s\n", one, two, three, four);
	    }
	}
    }

  fclose(parent_pipe);
  fclose(left_pipe);
  fclose(right_pipe); 
}


main(int argc, char ** argv)
{
  // Declarations
  int node_num_left, node_num_right, lb_left, lb_right, ub_right, ub_left;
  char lb_left_str[10], ub_left_str[10], lb_right_str[10], ub_right_str[10];
  char node_num_left_str[15], node_num_right_str[15], node_num_str[15];
  char path_to_sort_left[20], path_to_sort_right[20];
  char path_left[20], path_right[20], parent_path[25];
  char temp1[20], temp2[20];
  char myid[15], my_parents_id[15];
  char new_depth[10];
  
  // Handle arguments - convert strings to useable integers ~
  int curr_depth, sort_col, node_num, low_bound, up_bound, max_depth, file_size;
  char *filepath = argv[6];
  sscanf (argv[1], "%d", &curr_depth);
  sscanf (argv[2], "%d", &sort_col);
  sscanf (argv[3], "%d", &node_num);
  sscanf (argv[4], "%d", &low_bound);
  sscanf (argv[5], "%d", &up_bound);
  sscanf (argv[7], "%d", &max_depth);
  sscanf (argv[8], "%d", &file_size);

  
  // update variables
  sprintf (myid, "%d", (int) getpid());    // into string
  sprintf(node_num_str, "%d", node_num);
  node_num_left = node_num * 2;
  sprintf (node_num_left_str, "%d", node_num_left);
  node_num_right = node_num * 2 + 1;
  sprintf (node_num_right_str, "%d", node_num_right);  
  lb_left = low_bound;
  ub_left = (up_bound + low_bound)/2;
  lb_right = ub_left + 1;
  ub_right = up_bound;


  concatenate(path_left, myid, node_num_left_str);
  concatenate(path_right, myid, node_num_right_str);
  strcpy(path_left,prefix(path_left));
  strcpy(path_right,prefix(path_right));  

  // Create pipes to two child processes -> myid+childnodenum
  mkfifo(path_left, 0666);                     
  mkfifo(path_right, 0666);

  
  if (curr_depth < (max_depth-1))
    {
      sprintf (new_depth, "%d", curr_depth+1);
      sprintf (lb_left_str, "%d", lb_left);
      sprintf (ub_left_str, "%d", ub_left);
      sprintf (lb_right_str, "%d", lb_right);
      sprintf (ub_right_str, "%d", ub_right);
      if (fork()==0)
	{
	  execl("splitmerge", "splitmerge", new_depth, argv[2], node_num_left_str,
		lb_left_str, ub_left_str, argv[6], argv[7], argv[8], NULL);
	  perror("First exec failure");
	}
      else
	{
	  if (fork() == 0)
	    {
	      execl("splitmerge", "splitmerge", new_depth, argv[2], node_num_right_str, lb_right_str,
		    ub_right_str, argv[6], argv[7], argv[8], NULL);
	      perror("Second exec failure");
	    }
	  else // both my children have spawned off 
	    {
	      if (curr_depth == 0)
		{
		  // If we are at sm0, the parents path is rootfifo
		  strcpy(parent_path, "/tmp/rootfifo");   
		}
	      else   
		{
		  // if we are not at sm0, the parents path is parentsid + my nodenum. This is the path to write to.
		  sprintf(my_parents_id, "%d", (int) getppid());
		  concatenate(parent_path, my_parents_id, node_num_str);
		  strcpy(parent_path, prefix(parent_path));
		}
	      merge(path_left, path_right, parent_path, sort_col);   
	    }
	}   
    }

  else     // Its time to spawn sorters, for now suppose they all use unix sort
    {
      if (curr_depth == 0)
	{
	  strcpy (parent_path, "/tmp/rootfifo");
	}
      else
	{
	  sprintf(my_parents_id, "%d", (int) getppid());
	  concatenate(parent_path, my_parents_id, node_num_str);
	  strcpy(parent_path, prefix(parent_path));
	}

      concatenate(temp1, myid, "0"); concatenate(path_to_sort_left, node_num_left_str, temp1);
      concatenate(temp2, myid, "0"); concatenate(path_to_sort_right, node_num_right_str, temp2);
      strcpy(path_to_sort_left,prefix(path_to_sort_left));
      strcpy(path_to_sort_right,prefix(path_to_sort_right));

      mkfifo(path_to_sort_left, 0666);  // fifo down left
      mkfifo(path_to_sort_right, 0666); // fifo down right

      //                               sort will read v       and write to v
      extract(argv[6], argv[2], lb_left, ub_left, path_to_sort_left, path_left, node_num_left % 4, file_size);
      extract(argv[6], argv[2], lb_right, ub_right, path_to_sort_right, path_right, node_num_right % 4, file_size);
      merge(path_left, path_right, parent_path, sort_col);
    }
  
  return 0;
}





