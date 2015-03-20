//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Mon Dec 21 18:00:42 PST 1998
// Last Modified: Mon Dec 21 18:00:42 PST 1998
// Filename:      ...linuxmidi/output/method1.c
// Syntax:        C 
// $Smake:        gcc -O -o %b %f && strip %b
//

#include <sys/soundcard.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void usage(void)
{
	printf ("usage: simb device n_loop init_level breath\n");
	exit(0);
}

int main(int argc, char *argv[]) {
   char* device =  argv[1] ;
   int i,j;
   unsigned char data[3] = {0x90, 60, 100};
   if ( argc != 5 ) usage();

   // step 1: open the OSS device for writing
   int fd = open(device, O_WRONLY, 0);
   if (fd < 0) {
      printf("Error: cannot open %s\n", device);
      exit(1);
   }

   data[2]=atoi(argv[3]);

   // step 2: write the MIDI information to the OSS device
   printf ("Initial note level: %d\n", data[2]);
   write(fd, data, sizeof(data));

   data[0] = 0xB0; 
   data[1] = 2;
   data[2] = atoi(argv[4]);
   printf ("Continuous breath level: %d\n", data[2]);
   j=atoi(argv[2]);
   printf ("Looping for %d time(s)\n",j);
   for ( i=j; i> 0; i-- )
     {
        write(fd, data, sizeof(data));
	usleep(5000);
     }

   /* Envoi note Off */
   data[0] = 0x80; 
   data[1] = 60;
   data[2] = 0;
   write(fd, data, sizeof(data));
   /* Envoi souffle null */
   data[0] = 0xB0; 
   data[1] = 2;
   data[2] = 0;
   write(fd, data, sizeof(data));

  // close(fd);
   return 0;
}
   
