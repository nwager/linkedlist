#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

// tests
void test_int(void);
void test_string(void);
void test_large(void);
void test_iter(void);
void test_removeall(void);

// helpers
void print_data_int(void *data);
void print_data_string(void *data);
void free_data_string(void *data);
void print_list(struct linkedlist *ll, void (*print_data)(void*));

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  test_iter();
  test_int();
  test_string();
  test_large();
  test_removeall();

  return 0;
}

void test_int(void) {
  const size_t NUM_INT = 5;

  struct linkedlist *ll = linkedlist_alloc();
  for (size_t i = 0; i < NUM_INT; i++) {
    linkedlist_push(ll, (void *)i);
    assert((size_t)linkedlist_get(ll, i) == i);
  }

  const size_t MID_IDX = 2;

  size_t out = (size_t)linkedlist_remove(ll, MID_IDX);
  assert(out == MID_IDX);
  assert(linkedlist_size(ll) == NUM_INT-1);
  for (size_t i = 0; i < NUM_INT-1; i++) {
    size_t v = i + (i >= MID_IDX);
    assert((size_t)linkedlist_get(ll, i) == v);
  }

  out = (size_t)linkedlist_pop(ll);
  assert(linkedlist_size(ll) == NUM_INT-2);
  assert(out == NUM_INT-1);

  linkedlist_insert(ll, MID_IDX, (void *)MID_IDX);
  assert(linkedlist_size(ll) == NUM_INT-1);
  assert((size_t)linkedlist_get(ll, MID_IDX) == MID_IDX);

  linkedlist_free(ll, NULL);
}

void test_string(void) {
  const char *strings[] = { "zero", "one", "two", "three", "four", "five" };
  const size_t NUM_STR = sizeof(strings) / sizeof(*strings);

  struct linkedlist *ll = linkedlist_alloc();
  for (size_t i = 0; i < NUM_STR; i++) {
    char *s = malloc(sizeof(**strings) * (strlen(strings[i]) + 1));
    strcpy(s, strings[i]);
    linkedlist_push(ll, s);
    assert(strcmp((char *)linkedlist_get(ll, i), strings[i]) == 0);
  }
  assert(linkedlist_size(ll) == NUM_STR);

  const size_t MID_IDX = 2;

  char *out = linkedlist_remove(ll, MID_IDX);
  assert(linkedlist_size(ll) == NUM_STR-1);
  assert(strcmp(out, strings[MID_IDX]) == 0);

  free(out);

  linkedlist_free(ll, free);
}

void test_large(void) {
  const size_t SIZE = 10000;

  struct linkedlist *ll = linkedlist_alloc();

  for (size_t i = 0; i < SIZE; i++) {
    linkedlist_push(ll, (void *)(i*i));
  }
  assert(linkedlist_size(ll) == SIZE);

  for (size_t i = 1; i < SIZE; i <<= 1) {
    assert((size_t)linkedlist_get(ll, i) == (i*i));
  }

  linkedlist_free(ll, NULL);
}

void test_iter(void) {
  const size_t NUM_INT = 10;

  struct linkedlist *ll = linkedlist_alloc();

  // build list
  for (size_t i = 0; i < NUM_INT; i++) {
    linkedlist_push(ll, (void *)i);
  }

  // test gets
  lliter_init(ll);
  size_t i;
  for (i = 0; lliter_hasnext(ll); i++) {
    assert(i == (size_t)lliter_next(ll));
  }
  assert(i == NUM_INT);

  // do removes
  lliter_init(ll);
  for (i = 0; lliter_hasnext(ll); i++) {
    size_t v = (size_t)lliter_next(ll);
    if (i == 0 || i == NUM_INT/2 || i == NUM_INT-1) {
      v = (size_t)lliter_remove(ll); // should assign to same value
    }
    assert(v == i);
  }

  // verify removes
  lliter_init(ll);
  for (i = 0; i < NUM_INT; i++) {
    if (i == 0 || i == NUM_INT/2 || i == NUM_INT-1)
      continue;
    assert(lliter_hasnext(ll));
    assert(i == (size_t)lliter_next(ll));
  }
  assert(!lliter_hasnext(ll));

  linkedlist_free(ll, NULL);
}

void test_removeall(void) {
  const size_t NUM_INT = 5;

  // test regular removes
  struct linkedlist *ll = linkedlist_alloc();
  for (size_t i = 0; i < NUM_INT; i++) {
    linkedlist_push(ll, (void *)i);
  }
  assert(linkedlist_size(ll) == 5);
  for (size_t i = 0; i < NUM_INT; i++) {
    linkedlist_pop(ll);
  }
  assert(linkedlist_size(ll) == 0);
  linkedlist_free(ll, NULL);

  // test iterator removes
  ll = linkedlist_alloc();
  for (size_t i = 0; i < NUM_INT; i++) {
    linkedlist_push(ll, (void *)i);
  }
  assert(linkedlist_size(ll) == 5);
  int i = 0;
  for (lliter_init(ll); lliter_hasnext(ll); i++) {
    lliter_next(ll);
    lliter_remove(ll);
  }
  assert(i == NUM_INT);
  assert(linkedlist_size(ll) == 0);
  linkedlist_free(ll, NULL);
}

// helpers

void print_data_int(void *data) {
  int d = (size_t)data;
  printf("%d", d);
}

void print_data_string(void *data) {
  char *s = (char *)data;
  printf("'%s'", s);
}

void print_list(struct linkedlist *ll, void (*print_data)(void*)) {
  printf("ll of size %d: [", linkedlist_size(ll));
  bool started = false;
  for (lliter_init(ll); lliter_hasnext(ll);) {
    if (started) printf(", ");
    started = true;
    print_data(lliter_next(ll));
  }
  printf("]\n");
}
