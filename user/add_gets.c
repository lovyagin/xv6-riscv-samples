#include "kernel/types.h"
#include "user/user.h"

void errorMessage(const char* msg) {
  write(2, msg, strlen(msg));
  exit(0);
}

int readInt(char* buf, int bufSize, char* dst) {
  int i;
  i = 0;
  while (i < bufSize && buf[i] >= '0' && buf[i] <= '9') {
    dst[i] = buf[i];
    ++i;
  }
  if (i == 0)
    errorMessage("Incorrect input\n");
  dst[i] = '\0';
  return i;
}

void readToEnd() {
  char c;
  int cc;
  for(;;) {
    cc = read(0, &c, 1);
    if (cc < 1 || c == '\n') break;
  }
}

int main(int argc, char *argv[])
{
  if (argc != 1)
    errorMessage("Incorrect number of arguments, expected 0\n");
  const int BUF_SIZE = 30;
  char buf[BUF_SIZE];
  gets(buf, BUF_SIZE);
  if (strlen(buf) == BUF_SIZE - 1) {
    if (buf[BUF_SIZE - 1] != '\n') 
      readToEnd();
    errorMessage("Error: too large input\n");
  }
  char first_num[BUF_SIZE];
  int n, first_sz;

  n = strlen(buf);
  first_sz = 0;

  first_sz = readInt(buf, n, first_num);

  if (first_sz == n)
    errorMessage("Expected two numbers, but given only one\n");

  if (buf[first_sz] != ' ')
    errorMessage("Incorrect input\n");

  char second_num[BUF_SIZE];
  int second_sz;

  second_sz = readInt(buf + first_sz + 1, n - first_sz - 1, second_num);

  int readSz = first_sz + second_sz + 1;
  if (readSz != n - 1 || buf[readSz] != '\n')
    errorMessage("Incorrect input\n");

  int x = atoi(first_num), y = atoi(second_num);
  add(x, y);
  exit(0);
}
