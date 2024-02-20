#include "kernel/types.h"
#include "user/user.h"

#define BUF_SIZE 15

int
main(int argc, char *argv[])
{

  char str[] = "Hello, world";
  char buf[BUF_SIZE];
  int ret;

  buf[0] = '\0';
  ret = scpy (buf, str, BUF_SIZE);            // Trying to copy (good)
  printf(ret == 0 ? "Test 1: success" : "Test 1: copy error");
  printf("\treturn code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf, str, BUF_SIZE - 10);       // Trying to copy (bad size)
  printf(ret == 0 ? "Test 2: success" : "Test 2: copy error");
  printf("\treturn code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf + 1000, str, BUF_SIZE);     // Trying to copy (bad dst address)
  printf(ret == 0 ? "Test 3: success" : "Test 3: copy error");
  printf("\treturn code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf, (char *) 98765, BUF_SIZE); // Trying to copy (bad src address)
  printf(ret == 0 ? "Test 4: success" : "Test 4: copy error");
  printf("\treturn code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf, (char *) 0, BUF_SIZE); // Trying to copy (NULL src address)
  printf(ret == 0 ? "Test 5: success" : "Test 5: copy error");
  printf("\treturn code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy ((char *) 0, str, BUF_SIZE); // Trying to copy (NULL dst address)
  printf(ret == 0 ? "Test 6: success" : "Test 6: copy error");
  printf("\treturn code %d\tbuffer content: '%s'\n", ret, buf);
}
