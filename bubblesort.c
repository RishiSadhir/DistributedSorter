// 
// ARGS: sort Algo, file to sort, -o, file to print to, -k, col to sort on, filesize
//           0         1          2        3             4       5                6


#include "root.h"
#include <sys/times.h> /* times() */

int sort_column;

void swap(Record * a, Record * b)
{
  Record tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}


int main( int argc, char ** argv)
{
  char * to_sort = argv[1];
  char * out = argv[3];
FILE * time_out;
  char line[200];
  int sort_col;  sscanf (argv[5], "%d", &sort_col);
  int num_rows, i, ch, swaps;
  char attribute[10];
  Record * records;
  FILE * in_fd;
  FILE * out_fd;
  int rows;
  double t1, t2;
  struct tms tb1, tb2;
  double ticspersec;
  ticspersec = (double) sysconf(_SC_CLK_TCK);

  t1 = (double) times(&tb1);


  sscanf (argv[6], "%d", &rows);
  sscanf (argv[5], "%d", &sort_column);

  if((in_fd = fopen(argv[1], "rt")) == 0)
    {
      perror("Could not open file for sort\n");
    }

  records = (Record *) malloc(sizeof(Record) * rows);

  while (fscanf(in_fd, "%s %s %s %s",records[i].ssn,records[i].last, records[i].first, records[i].income) != EOF)
    {
      i++;
    }
  num_rows = i;

  swaps = 1;
  while (swaps != 0)
    {
      swaps = 0;
      for(i=0; i<(rows-1); i++)
	{
	  switch(sort_column)
	    {
	    case 1:
	      if( strcmp(records[i].ssn, records[i+1].ssn)>0 )
		{
		  swap(&records[i], &records[i+1]);
		  swaps++;
		}
	      break;
	    case 2:
	      if( strcmp(records[i].last, records[i+1].last)>0 )
		{
		  swap(&records[i], &records[i+1]);
		  swaps++;
		}
	      break;
	    case 3:
	      if( strcmp(records[i].first, records[i+1].first)>0 )
		{
		  swap(&records[i], &records[i+1]);
		  swaps++;
		}
	      break;
	    case 4:
	      if( strcmp(records[i].income, records[i+1].income)>0 )
		{
		  swap(&records[i], &records[i+1]);
		  swaps++;
		}
	      break;
	    }
	}
    }
  
  out_fd = fopen(argv[3], "wt");
  for(i=0; i<num_rows; i++)
    {
      fprintf(out_fd, "%s %s %s %s\n", records[i].ssn, records[i].last, records[i].first, records[i].income);
    }
  fclose(out_fd);

  free(records);

  t2 = (double) times(&tb2);
time_out = fopen("timeData.txt", "at");
fprintf(time_out, "Bubble sort took %lf seconds\n",(t2 - t1) / ticspersec);
fclose(time_out);
  return 0;
}




