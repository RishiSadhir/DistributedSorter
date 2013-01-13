// 
// ARGS: sort Algo, file to sort, -o, file to print to, -k, col to sort on, rows to sort
//           0         1          2        3             4       5              6 

#include <sys/times.h> /* times() */
#include "root.h"

int sort_column;

void swap(Record * a, Record * b)
{
  Record tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void insertionsort(Record * records, int gap, int size)
{
  int i, j;
  int a, b;
  Record tmp;
  
  for (i=gap; i<size; i++)
    {
      for( j = i; j-gap >= 0; j -= gap )
	{
	  switch(sort_column)
	    {
	    case 1:
	      if(strcmp(records[j].ssn, records[j-gap].ssn) < 0 )
		{
		  swap (&records[j], &records[j-gap]);
		}
	      break;
	    case 2:
	      if(strcmp(records[j].last, records[j-gap].last) < 0 )
		{
		  swap (&records[j], &records[j-gap]);
		}
	      break;
	    case 3:
	      if(strcmp(records[j].first, records[j-gap].first) < 0 )
		{
		  swap (&records[j], &records[j-gap]);
		}
	      break;
	    case 4:
	      if ( strcmp(records[j].income, records[j-gap].income) < 0 )
		{
		  swap (&records[j], &records[j-gap]);
		}
	      break;
	    }
	}
    }
}


int main( int argc, char ** argv)
{
  int series[20] = {1747331, 776591, 345152, 153401, 68178, 30301, 13467, 5985, 2660, 1182, 525, 233, 103, 46, 30, 9, 4, 1};
  char * to_sort = argv[1];
  char * out = argv[3];
  char line[200];
  int sort_col;  sscanf (argv[5], "%d", &sort_col);
  int num_rows, i, j, ch, swaps, gap;
  char attribute[10];
  Record * records;
  FILE * in_fd;
  FILE * out_fd;
  FILE * time_out;
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

  for(i=0; i<rows; i++)
    {
      if(fscanf(in_fd, "%s %s %s %s", records[i].ssn,records[i].last, records[i].first, records[i].income) == 0 )
	{
	  perror("error reading shit in shellsort\n");
	}
    }
  fclose(in_fd);

  for (j=18; j>=0; j--)
    {
      if(series[j]>0 && series[j]<rows)
	{
	  insertionsort(records, series[j], i);
	}
    }

  out_fd = fopen(argv[3], "wt");
  for(i=0; i<rows; i++)
    {
      fprintf(out_fd, "%s %s %s %s\n", records[i].ssn, records[i].last, records[i].first, records[i].income);
    }
  fclose(out_fd);
  

  free(records);
  t2 = (double) times(&tb2);
  time_out = fopen("timeData.txt", "at");
  fprintf(time_out, "Shell sort took %lf seconds\n",(t2 - t1) / ticspersec);
  fclose(time_out);

  return 0;
}




