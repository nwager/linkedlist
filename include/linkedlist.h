#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

/**
 * @brief Function to free an entry in a list.
 */
typedef void (*data_free_fn)(void *);

/**
 * @brief Linked list object.
 */
struct linkedlist;

/**
 * @brief Linked list iterator object.
 */
struct linkedlist_iter;

/**
 * @brief Create an empty linked list.
 * 
 * @return struct linkedlist* Pointer to the allocated list.
 */
extern struct linkedlist *linkedlist_alloc(void);

/**
 * @brief Frees a linked list.
 * 
 * @param ll Pointer to linked list to free.
 * @param fn Function to free an entry, or NULL for no-op.
 */
extern void linkedlist_free(struct linkedlist *ll, data_free_fn fn);

/**
 * @brief Get size of linked list.
 * 
 * @param ll Pointer to linked list.
 * @return unsigned int Number of entries in list.
 */
extern unsigned int linkedlist_size(struct linkedlist *ll);

/**
 * @brief Insert item into list at index.
 * 
 * @param ll Pointer to linked list.
 * @param index Index to insert item, 0 <= index <= size.
 * @param data Item to insert.
 */
extern void linkedlist_insert(struct linkedlist *ll, unsigned int index, void *data);

/**
 * @brief Remove item from list at index.
 * 
 * @param ll Pointer to linked list.
 * @param index Index to remove item, 0 <= index < size.
 * @return void* Removed item.
 */
extern void *linkedlist_remove(struct linkedlist *ll, unsigned int index);

/**
 * @brief Get item from list at index.
 * 
 * @param ll Pointer to linked list.
 * @param index Index to get item, 0 <= index < size.
 * @return void* List item.
 */
extern void *linkedlist_get(struct linkedlist *ll, unsigned int index);

/**
 * @brief Append item to end of list. Equivalent to linkedlist_insert(ll, size, data).
 * 
 * @param ll Pointer to linked list.
 * @param data Item to insert.
 */
extern void linkedlist_push(struct linkedlist *ll, void *data);

/**
 * @brief Get and remove item from end of list. Equivalent to linkedlist_remove(ll, size-1).
 * 
 * @param ll Pointer to linked list.
 * @return void* Removed list item.
 */
extern void *linkedlist_pop(struct linkedlist *ll);

/**
 * @brief Creates a new linkedlist iterator positioned at the list start.
 * 
 * @param ll Pointer to linked list.
 */
extern struct linkedlist_iter *lliter_alloc(struct linkedlist *ll);

/**
 * @brief Frees the iterator.
 * 
 * @param it Pointer to iterator.
 */
extern void lliter_free(struct linkedlist_iter *it);

/**
 * @brief Check if the iterator can proceed to the next item, or if it has reached the end.
 * 
 * @param it Pointer to iterator.
 * @return true if there is another item.
 * @return false if the iterator has reached the end of the list.
 */
extern bool lliter_hasnext(struct linkedlist_iter *it);

/**
 * @brief Get the next item of the iterator. The item must exist.
 * 
 * @param it Pointer to iterator.
 * @return void* List item.
 */
extern void *lliter_next(struct linkedlist_iter *it);

/**
 * @brief Remove the current item of the iterator. Must be preceded by lliter_next(it).
 * 
 * @param it Pointer to iterator.
 * @return void* Removed list item.
 */
extern void *lliter_remove(struct linkedlist_iter *it);

#endif // LINKEDLIST_H
