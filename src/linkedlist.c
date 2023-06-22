#include "linkedlist.h"
#include <stdlib.h>
#include <assert.h>

struct linkedlist_node {
  struct linkedlist_node *next;
  struct linkedlist_node *prev;
  void *data;
};

struct linkedlist_iter {
  struct linkedlist *ll;
  struct linkedlist_node *next;
  struct linkedlist_node *last;
};

struct linkedlist {
  struct linkedlist_node *head;
  struct linkedlist_node *tail;
  unsigned int size;
};

// Gets node at index i, given nonempty list and 0 <= i < ll->size.
static struct linkedlist_node *get_node(struct linkedlist *ll, unsigned int i);
// Directly removes node n from linkedlist ll, instead of using an index number.
static void *remove_node(struct linkedlist *ll, struct linkedlist_node *n);

struct linkedlist *linkedlist_alloc(void) {
  struct linkedlist *ll;
  ll = (struct linkedlist*) malloc(sizeof(*ll));
  ll->head = NULL;
  ll->tail = NULL;
  ll->size = 0;

  return ll;
}

void linkedlist_free(struct linkedlist *ll, data_free_fn fn) {
  struct linkedlist_node *n;
  struct linkedlist_node *n_next;

  // free each node
  for (n = ll->head; n != NULL; n = n_next) {
    n_next = n->next;
    if (fn != NULL)
      fn(n->data);
    free(n);
  }

  // free head
  free(ll);
}

unsigned int linkedlist_size(struct linkedlist *ll) {
  return ll->size;
}

void linkedlist_insert(struct linkedlist *ll, unsigned int index, void *data) {
  assert(index <= ll->size);

  struct linkedlist_node *n;
  struct linkedlist_node *curr;

  n = (struct linkedlist_node*) malloc(sizeof(*n));
  assert(n != NULL);
  n->data = data;

  if (ll->size == 0) {
    // list is empty
    ll->head = ll->tail = n;
    n->next = NULL;
    n->prev = NULL;
  } else if (index == 0) {
    // insert at head
    n->next = ll->head;
    n->prev = NULL;
    ll->head->prev = n;
    ll->head = n;
  } else if (index == ll->size) {
    // insert at tail
    n->prev = ll->tail;
    n->next = NULL;
    ll->tail->next = n;
    ll->tail = n;
  } else {
    // find slot to displace
    curr = get_node(ll, index);
    // insert node at index
    n->next = curr;
    n->prev = curr->prev;
    n->prev->next = n;
    curr->prev = n;
  }

  ll->size++;
}

void *linkedlist_remove(struct linkedlist *ll, unsigned int index) {
  assert(ll->size != 0 && index < ll->size);

  struct linkedlist_node *n;

  if (ll->size == 1) {
    // list has one item
    n = ll->head;
  } else if (index == 0) {
    // remove head
    n = ll->head;
  } else if (index == ll->size-1) {
    // remove tail
    n = ll->tail;
  } else {
    n = get_node(ll, index);
  }
  
  void *data = remove_node(ll, n);
  return data;
}

void *linkedlist_get(struct linkedlist *ll, unsigned int index) {
  assert(ll->size != 0 && index < ll->size);

  struct linkedlist_node *n = get_node(ll, index);
  return n->data;
}

void linkedlist_push(struct linkedlist *ll, void *data) {
  linkedlist_insert(ll, ll->size, data);
}

void *linkedlist_pop(struct linkedlist *ll) {
  return linkedlist_remove(ll, ll->size-1);
}

// iterator

struct linkedlist_iter *lliter_alloc(struct linkedlist *ll) {
  struct linkedlist_iter *it = malloc(sizeof(struct linkedlist_iter));
  it->ll = ll;
  it->next = it->ll->head;
  it->last = NULL;
  return it;
}

void lliter_free(struct linkedlist_iter *it) {
  free(it);
}

bool lliter_hasnext(struct linkedlist_iter *it) {
  return it->next != NULL;
}

void *lliter_next(struct linkedlist_iter *it) {
  assert(lliter_hasnext(it));
  it->last = it->next;
  it->next = it->next->next;
  return it->last->data;
}

void *lliter_remove(struct linkedlist_iter *it) {
  assert(it->last != NULL && "Must be preceded by lliter_next()");
  struct linkedlist_node *n = it->last;
  it->last = NULL;
  return remove_node(it->ll, n);
}

// helpers
static struct linkedlist_node *get_node(struct linkedlist *ll, unsigned int i) {
  struct linkedlist_node *curr;
  if (i < ll->size / 2) {
    for (curr = ll->head; i > 0; curr = curr->next, i--);
  } else {
    for (curr = ll->tail, i = ll->size-1 - i; i > 0; curr = curr->prev, i--);
  }
  return curr;
}

static void *remove_node(struct linkedlist *ll, struct linkedlist_node *n) {
  if (n == ll->head)
    ll->head = n->next;
  if (n == ll->tail)
    ll->tail = n->prev;
  if (n->next != NULL)
    n->next->prev = n->prev;
  if (n->prev != NULL)
    n->prev->next = n->next;

  ll->size--;

  void *data = n->data;
  free(n);
  return data;
}
