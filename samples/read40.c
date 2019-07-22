/*---------------------------------------------------------------------------*/
/* read40.c                                                                  */
/*                                                                           */
/* WIT application program to read a WIT data file in 4.0 format and display */
/* the result.                                                               */
/* The name of the data file should be given as the command line argument.   */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <wit.h>

void main (int argc, char * argv[])
   {
   WitRun *  theWitRun;

   /* Make sure a data file was specified on command line. */

   if (argc < 2)
      {
      printf ( "usage: %s wit_data_file.4.0_name\n", argv[0]);

      exit (1);
      }

   witNewRun     (&theWitRun);
   witInitialize  (theWitRun);
   witReadData40  (theWitRun, argv[1]);
   witDisplayData (theWitRun, "echo.40");
   witDeleteRun   (theWitRun);

   exit (0);
   }
