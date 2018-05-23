#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define CEILING 80      //maximum of iterations
#define P 0.85          //dumping factor, (1-P) is the possibility of a random jump

#define OK 1
#define ERROR 0

typedef int status;

typedef struct { 
  int name;
  double value;
}page;

typedef struct Edge {
  int x, y;
  double value;
  struct Edge *right, *down;
}Edge, *List;
typedef struct {
  List *RHead, *CHead;
}CrossList;


/*  FUNCTIONS  */
//call functions
status Pagerank(char *filename, int num_pages, char *dir, int threads);

//create probabilistic transfer matrix and initialize it
status CreateCL(CrossList *M, char *filename, int num_pages);
//insert in a row
status RInsert(CrossList *M, Edge *p, int i);
//insert in a column
status CInsert(CrossList *M, Edge *p, int j);
//destroy a cross list
status DestroyCL(CrossList *M);

//calulate probabilistic transfer matrix
//although two pages without an edge can be randomly accessed, they are ignored for now
status Cal_pt(CrossList *pt, int num_pages, double random);
//define multiplication of a matrix and an array
status Mul(CrossList *pt, page *pr1, page *pr2, int threads, int num_pages, double random);
//call function Mul
status Calculation(CrossList *pt, page *pr1, page *pr2, int threads, int num_pages, double threshold, double random);

//get the minimum and maximum of pagerank values
status MinMax(page *pr1, int *min_name, double *min_value, int *max_name, double *max_value, int num_pages);


/*  DETAILS  */
int main(int argc, char *argv[]) {
	char *filename, *dir;
	int threads, num_pages;

	filename = argv[1];
	num_pages = atoi(argv[2]);
	dir = argv[3];
	threads = atoi(argv[4]);

	Pagerank(filename, num_pages, dir, threads);

	return 0;
}

//call functions
status Pagerank(char *filename, int num_pages, char *dir, int threads) {
	CrossList pt;         //probabilistic transfer matrix
	page *pr1, *pr2;      //pagerank values
	double threshold;     //iteration threshold
	double random;        //(1-P)/num_pages, possibility of randomly jump to one of the pages
	
	FILE *fp;
	int i;

	int min_name, max_name;
	double min_value, max_value;

	printf("\n------------start------------\n");

	printf("number of threads: %d\n", threads);
	printf("number of nodes: %d\n", num_pages);
	printf("initializing pr1 and pr2........");
	pr1 = (page *)malloc(num_pages * sizeof(page));
	pr2 = (page *)malloc(num_pages * sizeof(page));
	for (i = 0; i<num_pages; i++) {  //initialize pr1 and pr2
		pr1[i].name = i;
		pr2[i].name = i;
		pr1[i].value = 1.0;
	}

	printf("completed.\ncreating matrix pt........");
	CreateCL(&pt, filename, num_pages);

	random = (1 - P) / num_pages;
	printf("conpleted.\ncalculating matrix pt........");
	Cal_pt(&pt, num_pages, random);

	threshold = 1.0 / num_pages / 10;
	printf("completed.\ncalculating pagerank values........     \n");
	Calculation(&pt, pr1, pr2, threads, num_pages, threshold, random);

	printf("comleted.\noutputing........");
	chdir(dir);
	fp = fopen("pr_value.txt", "w");
	for (i = 0; i < num_pages; i++)
		fprintf(fp, "%d	%e\n", pr2[i].name, pr2[i].value);
	fclose(fp);

	MinMax(pr2, &min_name, &min_value, &max_name, &max_value, num_pages);
	fp = fopen("pr_minmax.txt", "w");
	fprintf(fp, "%d	%e\n", min_name, min_value);
	fprintf(fp, "%d	%e\n", max_name, max_value);
	fclose(fp);

	printf("completed.\n");
	printf("-------------end-------------\n");

	return OK;
}


//create probabilistic transfer matrix and initialize it
status CreateCL(CrossList *M, char *filename, int num_pages) {
	int i, j, k;
	double e = 1.0;
	Edge *p;
	FILE *fp;

	char s[128], *s1, c;  //to ignore lines start with "#"

	if (M->RHead != NULL)
		DestroyCL(M);

	//allocate space
	M->RHead = (List *)malloc(num_pages * sizeof(List));
	M->CHead = (List *)malloc(num_pages * sizeof(List));
	if (M->RHead == NULL || M->CHead == NULL)
		exit(ERROR);
	for (k = 0; k < num_pages; k++) {
		M->RHead[k] = NULL;
		M->CHead[k] = NULL;
	}

	fp = fopen(filename, "r");

	if (fp == NULL) {    //if cannot open file
		printf("cannot open input file\n");
		exit(ERROR);
	}
	
	//ingnore lines start with "#"
	c = fgetc(fp);
	while (c == '#') {
		s1 = fgets(s, 128, fp);
		c = fgetc(fp);
	}
	fseek(fp, -1l, 1);    //move the "cursor" left

	//add an element to the matrix
	while (!feof(fp)) {
		fscanf(fp, "%d %d", &j, &i);
		p = (Edge *)malloc(sizeof(Edge));
		p->x = i;
		p->y = j;
		p->value = e;

		RInsert(M, p, i);
		CInsert(M, p, j);
	}

	fclose(fp);

	return OK;
}

//insert a node in a row
status RInsert(CrossList *M, Edge *p, int i) {
  Edge *q, *r;

  q = M->RHead[i];    //find the row
  if (q == NULL || q->y > p->y) {    //find the position
    p->right = q;
    M->RHead[i] = p;
  }
  else {    //correct position
    r = q->right;
    while (r != NULL && r->y < p->y) {
      q = r;
      r = q->right;
    }
    q->right = p;
    p->right = r;
  }

  return OK;
}

//insert a node in a column
status CInsert(CrossList *M, Edge *p, int j) {
  Edge *q, *r;

  q = M->CHead[j];    //find the column
  if (q == NULL || q->x > p->x) {    //find the position
    p->down = q;
    M->CHead[j] = p;
  }
  else {    //correct position
    r = q->down;
    while (r != NULL && r->x < p->x) {
      q = r;
      r = q->down;
    }
    q->down = p;
    p->down = r;
  }

  return OK;
}


//destroy a cross list
status DestroyCL(CrossList *M) {
  M->RHead = NULL;
  M->CHead = NULL;

  return OK;
}

//calulate probabilistic transfer matrix
//although two pages without an edge can be randomly accessed, they are ignored for now
status Cal_pt(CrossList *pt, int num_pages, double random) {
  Edge *p;
  int j, n;

  for (j = 0; j < num_pages; j++) {    //for every column
    n = 0;
    p = pt->CHead[j];

    while (p != NULL) {    //get the out-degree of every page j
      n++;
      p = p->down;
    }

    if (n != 0) {    //if the out-degree of page j isn't 0
      p = pt->CHead[j];
      while (p != NULL) {
        //there is a probability P that jump along edges and a possibility 1-P that jump randomly
        p->value = p->value * P / n + random;    //random = (1 - P) / num_pages
        p = p->down;
      }
    }
  }

  return OK;
}

//call function Mul
status Calculation(CrossList *pt, page *pr1, page *pr2, int threads, int num_pages, double threshold, double random) {
	int i, index;
	double x;
	page *temp;
	int static count;

	Mul(pt, pr1, pr2, threads, num_pages, random);

	index = 0;    //index of the comparison between the difference and the threshold
	for (i = 0; i < num_pages; i++) {
		x = (*(pr1 + i)).value - (*(pr2 + i)).value;
		if (x > threshold || x < (0 - threshold)) {
			index = 1;
			break;
		}
	}

	if (count >= CEILING)    //if the number of iterations reaches the maximum
		index = 0;
	
	if (index == 1) {    //the difference is larger than the threshold, continue iterating
		temp = pr1;
		pr1 = pr2;
		pr2 = temp;

		count++;
		printf("\b\b\b\b\b\b%3d, \n", count);    //update the iteration counter

		Calculation(pt, pr1, pr2, threads, num_pages, threshold, random);
	}
	return OK;
}

/* define the multiplication of a matrix and an array, pr2=pt*pr1 */
status Mul(CrossList *pt, page *pr1, page *pr2, int threads, int num_pages, double random) {
  int i;

#pragma omp parallel for default(none) private(i) shared(pt, pr1, pr2, num_pages, random) num_threads(threads)
  for (i = 0; i < num_pages; i++) {  //for every page i
    Edge *p;
    int j;
    double e;
    p = pt->RHead[i];
    e = 0.0;
    for (j = 0; j < num_pages; j++) {  //for every page j
      if (p == NULL || p->y > j)  //if there is no edge from j to i
        e = e + random * (*(pr1+j)).value;
      else if (p->y == j) {  //if there is an edge from j to i
        e = e + p->value * (*(pr1+j)).value;
        p = p->right;
      }
    }
    (*(pr2+i)).value = e;  //get the new pagerank value of page i
  }

  return OK;
}

//get the minimum and maximum of pagerank values
status MinMax(page *pr1, int *min_name, double *min_value, int *max_name, double *max_value, int num_pages) {
  int i, a, b;
  double min, max;

  a = 0;
  b = 0; 
  min = 1.0;
  max = 0.0;
  for (i = 0; i < num_pages; i++) { 
    a = min < pr1[i].value ? a : i;
    b = max < pr1[i].value ? i : b;
    min = min < pr1[i].value ? min : pr1[i].value;
    max = max < pr1[i].value ? pr1[i].value : max;
  }
  
  *min_name = a;
  *max_name = b;
  *min_value = min;
  *max_value = max;
}
