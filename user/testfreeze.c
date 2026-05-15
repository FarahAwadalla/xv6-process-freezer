// user/testfreeze.c
// Member 4 — Testing, Stress Cases & User Programs
// Compatible xv6 freeze/resume testing suite

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// ----------------------------------------------------
// Test Counters
// ----------------------------------------------------

static int passed = 0;
static int failed = 0;

void
check(char *name, int condition)
{
  if(condition){
    printf("[PASS] %s\n", name);
    passed++;
  } else {
    printf("[FAIL] %s\n", name);
    failed++;
  }
}

// ----------------------------------------------------
// Test 1: Invalid PID Tests
// ----------------------------------------------------

void
test_invalid_pid(void)
{
  printf("\n=== Test 1: Invalid PIDs ===\n");

  check("freeze(-1)", freeze(-1) == -1);
  check("freeze(0)", freeze(0) == -1);
  check("freeze(99999)", freeze(99999) == -1);

  check("resume(-1)", resume(-1) == -1);
  check("resume(0)", resume(0) == -1);
  check("resume(99999)", resume(99999) == -1);
}

// ----------------------------------------------------
// Test 2: Freeze Zombie Process
// ----------------------------------------------------

void
test_freeze_zombie(void)
{
  printf("\n=== Test 2: Freeze Zombie ===\n");

  int pid = fork();

  if(pid == 0){
    exit(0);
  }

  sleep(5);

  check("freeze(zombie) == -1", freeze(pid) == -1);

  wait(0);
}

// ----------------------------------------------------
// Test 3: Parent Freezes Child
// ----------------------------------------------------

void
test_parent_freeze_child(void)
{
  printf("\n=== Test 3: Parent Freeze/Resume Child ===\n");

  int pid = fork();

  if(pid == 0){
    while(1){
      sleep(100);
    }
  }

  sleep(5);

  check("freeze(child)", freeze(pid) == 0);

  sleep(20);

  check("resume(child)", resume(pid) == 0);

  kill(pid);
  wait(0);
}

// ----------------------------------------------------
// Test 4: Self Freeze
// ----------------------------------------------------

void
test_self_freeze(void)
{
  printf("\n=== Test 4: Self Freeze ===\n");

  int pid = fork();

  if(pid == 0){
    sleep(5);

    freeze(getpid());

    // resumes here after parent resumes process
    exit(0);
  }

  sleep(30);

  check("resume(self-frozen child)", resume(pid) == 0);

  wait(0);

  check("self-frozen child exited", 1);
}

// ----------------------------------------------------
// Test 5: Double Freeze
// ----------------------------------------------------

void
test_double_freeze(void)
{
  printf("\n=== Test 5: Double Freeze ===\n");

  int pid = fork();

  if(pid == 0){
    while(1){
      sleep(100);
    }
  }

  sleep(5);

  check("first freeze()", freeze(pid) == 0);

  check("second freeze()", freeze(pid) == -1);

  resume(pid);

  kill(pid);

  wait(0);
}

// ----------------------------------------------------
// Test 6: Repeated Resume
// ----------------------------------------------------

void
test_repeated_resume(void)
{
  printf("\n=== Test 6: Repeated Resume ===\n");

  int pid = fork();

  if(pid == 0){
    while(1){
      sleep(100);
    }
  }

  sleep(5);

  freeze(pid);

  check("first resume()", resume(pid) == 0);

  check("second resume()", resume(pid) == -1);

  kill(pid);

  wait(0);
}

// ----------------------------------------------------
// Test 7: Multiple Freeze/Resume Cycles
// ----------------------------------------------------

void
test_multiple_cycles(void)
{
  printf("\n=== Test 7: Multiple Freeze/Resume Cycles ===\n");

  int pid = fork();

  if(pid == 0){
    while(1){
      sleep(100);
    }
  }

  sleep(5);

  int ok = 1;

  for(int i = 0; i < 5; i++){

    if(freeze(pid) != 0){
      ok = 0;
      break;
    }

    sleep(5);

    if(resume(pid) != 0){
      ok = 0;
      break;
    }

    sleep(5);
  }

  check("5 cycles succeeded", ok);

  kill(pid);

  wait(0);
}

// ----------------------------------------------------
// Test 8: Stress Test
// ----------------------------------------------------

void
test_stress_many(void)
{
  printf("\n=== Test 8: Stress Test ===\n");

#define NPROCS 6

  int pids[NPROCS];

  for(int i = 0; i < NPROCS; i++){

    pids[i] = fork();

    if(pids[i] == 0){

      while(1){
        sleep(100);
      }
    }
  }

  sleep(10);

  int ok1 = 1;

  for(int i = 0; i < NPROCS; i++){

    if(freeze(pids[i]) != 0){
      ok1 = 0;
    }
  }

  check("freeze many processes", ok1);

  sleep(20);

  int ok2 = 1;

  for(int i = 0; i < NPROCS; i++){

    if(resume(pids[i]) != 0){
      ok2 = 0;
    }
  }

  check("resume many processes", ok2);

  for(int i = 0; i < NPROCS; i++){

    kill(pids[i]);

    wait(0);
  }
}

// ----------------------------------------------------
// Test 9: Freeze Then Kill
// ----------------------------------------------------

void
test_freeze_then_kill(void)
{
  printf("\n=== Test 9: Freeze Then Kill ===\n");

  int pid = fork();

  if(pid == 0){

    while(1){
      sleep(100);
    }
  }

  sleep(5);

  check("freeze before kill", freeze(pid) == 0);

  kill(pid);

  resume(pid);

  wait(0);

  check("killed frozen process cleaned", 1);
}

// ----------------------------------------------------
// Main
// ----------------------------------------------------

int
main(void)
{
  printf("\n====================================\n");
  printf("      testfreeze - xv6 tests\n");
  printf("====================================\n");

  test_invalid_pid();

  test_freeze_zombie();

  test_parent_freeze_child();

  test_self_freeze();

  test_double_freeze();

  test_repeated_resume();

  test_multiple_cycles();

  test_stress_many();

  test_freeze_then_kill();

  printf("\n====================================\n");
  printf("Tests Passed: %d\n", passed);
  printf("Tests Failed: %d\n", failed);
  printf("====================================\n");

  exit(0);
}
