#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
int
main (int argc, char **argv)
{
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c)
      {
      case 'a':
        aflag = 1;
        break;
      case 'b':
        bflag = 1;
        break;
      case 'c':
        cvalue = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }

  printf ("aflag = %d, bflag = %d, cvalue = %s\n",
          aflag, bflag, cvalue);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;
}
//Examples of arguments to add command line and the optput
/* 
% optsarg
aflag = 0, bflag = 0, cvalue = (null)

% optsarg -a -b
aflag = 1, bflag = 1, cvalue = (null)

% optsarg -ab
aflag = 1, bflag = 1, cvalue = (null)

% optsarg -c foo
aflag = 0, bflag = 0, cvalue = foo

% optsarg -cfoo
aflag = 0, bflag = 0, cvalue = foo

% optsarg arg1
aflag = 0, bflag = 0, cvalue = (null)
Non-option argument arg1

% optsarg -a arg1
aflag = 1, bflag = 0, cvalue = (null)
Non-option argument arg1

% optsarg -c foo arg1
aflag = 0, bflag = 0, cvalue = foo
Non-option argument arg1

% optsarg -a -- -b
aflag = 1, bflag = 0, cvalue = (null)
Non-option argument -b

% optsarg -a -
aflag = 1, bflag = 0, cvalue = (null)
Non-option argument -
 */