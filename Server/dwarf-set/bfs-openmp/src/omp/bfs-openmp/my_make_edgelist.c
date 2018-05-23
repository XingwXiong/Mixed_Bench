/******************************************************************
 *       File Name: my_make_edgelist.c
 *     Description: 
 *          Author: He Xiwen
 *           Email: hexiwen2000@163.com 
 *    Created Time: 2015-12-13 15:35:17
 *   Modified Time: 
 *          License: 
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include <assert.h>

#include <fcntl.h>
#include <unistd.h>

#include "generator/graph_generator.h"
#include "xalloc.h"


static struct packed_edge * restrict IJ;
static int64_t nedge;


int
main (int argc, char **argv)
{
  int i;
  int fd;
  const char *input_f;
  const char *output_f;
  const char *dumpname;
  FILE *ifp;
  uint64_t u, v;
  int64_t e_cnt;
  if (sizeof (int64_t) < 8) {
    fprintf (stderr, "No 64-bit support.\n");
    return EXIT_FAILURE;
  }

  // usage: exe nedge input_f output_f
  if( argc != 4 )
  {
    if( argc != 1 ) fprintf( stderr, "Wrong arguments!\n" );
    printf( "Usage: %s nedge input_f output_f\n", argv[0] );
    if( argc == 1 ) exit( EXIT_SUCCESS );
    else exit( EXIT_FAILURE );
  }
#ifdef DEBUG_TEST
  printf( "argc: %d\n", argc );
  printf( "argv: " );
  for( i=0; i<argc; ++i ) printf( "%s ", argv[i] );
  printf( "\n" );
#endif
  nedge = atoi( argv[1] );
  input_f = argv[2];
  output_f = argv[3];

  IJ = xmalloc_large_ext (nedge * sizeof (*IJ));

  ifp = fopen( input_f, "r" );
  if( ifp == NULL ) {
    perror( "Cannot open input_f" );
    exit( EXIT_FAILURE );
  }

  e_cnt = 0;
  for( i=0; i<nedge; ++i ) {
    if( fscanf( ifp, "%ld\t%ld", &u, &v ) == 2 ) ++e_cnt;
    else break;
    write_edge( IJ+i, u, v );
  }
  nedge = e_cnt;

  fclose( ifp );

  dumpname = output_f;
  if (dumpname)
    fd = open (dumpname, O_WRONLY|O_CREAT|O_TRUNC, 0666);
  else
    fd = 1;

  if (fd < 0) {
    fprintf (stderr, "Cannot open output file : %s\n",
	     (dumpname? dumpname : "stdout"));
    return EXIT_FAILURE;
  }

  //write (fd, IJ, 2 * nedge * sizeof (*IJ));
  write (fd, IJ, nedge * sizeof (*IJ));

  close (fd);
  xfree_large( IJ );

  return EXIT_SUCCESS;
}
