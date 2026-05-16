#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(2, "Usage: freeze <pid> [reason]\n");
    exit(1);
  }

  int pid = atoi(argv[1]);
  char *reason = (argc >= 3) ? argv[2] : "manual";

  if (freeze(pid, reason) < 0)
  {
    fprintf(2, "freeze: failed for pid %d\n", pid);
    exit(1);
  }

  printf("Frozen pid %d (reason: %s)\n", pid, reason);
  exit(0);
}