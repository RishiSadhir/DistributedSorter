// 
// ARGS: sort Algo, file to sort, -o, file to print to, -k, col to sort on, filesize
//           0         1          2        3             4       5                6


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

int partition(Record * records, int left, int right, int pivot)
{
  int tmp_index, i, a, b;
  Record pivotval = records[pivot];
  swap(&records[pivot], &records[right]);
  tmp_index = left;
  for (i=left; i<right; i++)
    {
      switch(sort_column)
	{
	case 1:
	  if( strcmp(pivotval.ssn, records[i].ssn) > 0 )
	    {
	      swap(&records[i], &records[tmp_index]);
	      tmp_index++;
	    }
	  break;
	case 2:
	  if( strcmp(pivotval.last, records[i].last) > 0 )
	    {
	      swap(&records[i], &records[tmp_index]);
	      tmp_index++;
	    }
	  break;
	case 3:
	  if( strcmp(pivotval.first, records[i].first) > 0 )
	    {
	      swap(&records[i], &records[tmp_index]);
	      tmp_index++;
	    }
	  break;
	case 4:
	  if( strcmp(pivotval.income, records[i].income) > 0)
	    {
	      swap(&records[i], &records[tmp_index]);
	      tmp_index++;
	    }
	  break;	  
	}
    }
  swap(&records[tmp_index], &records[right]);
  return tmp_index;
}


void sort(Record * records, int left, int right)
{
  int pivot;
  if (left<right)
    {
      pivot = (right + left)/2;
      pivot = partition(records, left, right, pivot);
      sort(records, left, pivot-1);
      sort(records, pivot+1, right);
    }
}


int main( int argc, char ** argv)
{
  char * to_sort = argv[1];
  char * out = argv[3];
  char line[200];
  int sort_col;  sscanf (argv[5], "%d", &sort_col);
  int num_rows, i, ch, swaps;
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

  while (fscanf(in_fd, "%s %s %s %s",records[i].ssn,records[i].last, records[i].first, records[i].income) != EOF)
    {
      i++;
    }
  num_rows = i;
  sort(records,0, i-1);
  
  out_fd = fopen(argv[3], "wt");
  for(i=0; i<num_rows; i++)
    {
      fprintf(out_fd, "%s %s %s %s\n", records[i].ssn, records[i].last, records[i].first, records[i].income);
    }
  fclose(out_fd);

  free(records);
  t2 = (double) times(&tb2);
  time_out = fopen("timeData.txt", "at");
  fprintf(time_out, "Quick sort took %lf seconds\n",(t2 - t1) / ticspersec);
  fclose(time_out);
  
  return 0;
}




