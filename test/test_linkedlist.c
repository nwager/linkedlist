#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

typedef struct linkedlist_iter iter;

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
  iter *it = lliter_alloc(ll);
  size_t i;
  for (i = 0; lliter_hasnext(it); i++) {
    assert(i == (size_t)lliter_next(it));
  }
  assert(i == NUM_INT);

  lliter_free(it);

  // do removes
  it = lliter_alloc(ll);
  for (i = 0; lliter_hasnext(it); i++) {
    size_t v = (size_t)lliter_next(it);
    if (i == 0 || i == NUM_INT/2 || i == NUM_INT-1) {
      v = (size_t)lliter_remove(it); // should assign to same value
    }
    assert(v == i);
  }
  lliter_free(it);

  // verify removes
  it = lliter_alloc(ll);
  for (i = 0; i < NUM_INT; i++) {
    if (i == 0 || i == NUM_INT/2 || i == NUM_INT-1)
      continue;
    assert(lliter_hasnext(it));
    assert(i == (size_t)lliter_next(it));
  }
  assert(!lliter_hasnext(it));
  lliter_free(it);

  linkedlist_free(ll, NULL);

  // test nested iter
  ll = linkedlist_alloc();
  for (size_t i = 0; i < NUM_INT; i++) {
    linkedlist_push(ll, (void *)i);
  }
  iter *it1;
  iter *it2;
  it1 = lliter_alloc(ll);
  for (i = 0; lliter_hasnext(it1); i++) {
    size_t data = (size_t)lliter_next(it1);
    assert(data == i);
    it2 = lliter_alloc(ll);
    size_t j;
    for (j = 0; lliter_hasnext(it2); j++) {
      data = (size_t)lliter_next(it2);
      assert(data == j);
    }
    assert(j == NUM_INT);
    lliter_free(it2);
  }
  assert(i == NUM_INT);
  lliter_free(it1);
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
  int i;
  iter *it = lliter_alloc(ll);
  for (i = 0; lliter_hasnext(it); i++) {
    lliter_next(it);
    lliter_remove(it);
  }
  assert(i == NUM_INT);
  assert(linkedlist_size(ll) == 0);
  lliter_free(it);
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
  iter *it = lliter_alloc(ll);
  while (lliter_hasnext(it)) {
    if (started) printf(", ");
    started = true;
    print_data(lliter_next(it));
  }
  lliter_free(it);
  printf("]\n");
}
