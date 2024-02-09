#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("Hello, world (printf)\n");      // Common printf-based Hello, world
  
  char msg[] = "Hello, world (write)\n";  // Buffer to write
  int len = sizeof(msg) - 1;              // Message length excluding trailing '\0'

  int ret = write(1, msg, len);           // Write to stdout
  
  if (ret == len)                               // Check for write return
      exit (0);                                 // Exit success
  else            
  {
      write (2, "Write failure (stdout)\n", 23); // Write to stderr (bad: 23 is a magic number here)
      exit (1);                                  // Exit failure
  }
}
