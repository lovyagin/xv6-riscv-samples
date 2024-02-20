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
  perror(-ret);
  printf("return code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf, str, BUF_SIZE - 10);       // Trying to copy (bad size)
  perror(-ret);
  printf("return code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf + 1000, str, BUF_SIZE);     // Trying to copy (bad dst address)
  perror(-ret);
  printf("return code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf, (char *) 98765, BUF_SIZE); // Trying to copy (bad src address)
  perror(-ret);
  printf("return code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy (buf, (char *) 0, BUF_SIZE); // Trying to copy (NULL src address)
  perror(-ret);
  printf("return code %d\tbuffer content: '%s'\n", ret, buf);

  buf[0] = '\0';
  ret = scpy ((char *) 0, str, BUF_SIZE); // Trying to copy (NULL dst address)
  perror(-ret);
  printf("return code %d\tbuffer content: '%s'\n", ret, buf);
}
