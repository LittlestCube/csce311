// Copyright 2023 LittleCube
//

/*
 * Count to 2000000 with two threads
 * and synchronizes them with a semaphore.
 * 
 * This is one of the simplest semaphore examples out there.
 * 
 */

#include <cubiphore.h>

#include <iostream>
#include <thread>  // NOLINT

void a_count_func(Cubiphore* cubiphore, int* count, const int kCountTo) {
  // count to half of our goal
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "a is finished! Counted to " << *count << "." << std::endl;

  // allow b_count to count the other half
  cubiphore->Post();
}

void b_count_func(Cubiphore* cubiphore, int* count, const int kCountTo) {
  // make sure we don't start counting before a finishes
  cubiphore->Wait();

  // a has finished and we can begin counting
  for (int i = 0; i < kCountTo/2; ++i) {
    ++*count;
  }

  std::cout << "b is finished! Counted to " << *count << "." << std::endl;
}

int main(/* int argc, char* argv[] */) {
  // initialize b_count_func's semaphore
  Cubiphore b_cubiphore = Cubiphore(0);

  // assign our goal
  int kCountTo = 2000000;
  int count = 0;

  // start thread a
  std::thread a_thread = std::thread(a_count_func,
                                     &b_cubiphore,
                                     &count,
                                     kCountTo);

  // start thread b
  std::thread b_thread = std::thread(b_count_func,
                                     &b_cubiphore,
                                     &count,
                                     kCountTo);

  // wait for threads to finish before moving on
  a_thread.join();
  b_thread.join();

  std::cout << std::endl << "The final count is " << count << "!" << std::endl;

  // return in main because we clean boys :D
  return 0;
}
