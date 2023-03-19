#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// <summary>
/// Struct for a node of a linked list.
/// </summary>
typedef struct sListElem
{
	void* data; ///< Pointer to the data stored in this node
	struct sListElem* next; ///< Pointer to the next node in the list
} *ListElem, SListElem; ///< Pointer to sListElem struct and alias for sListElem struct

// List length

/// <summary>
/// Returns the length of the linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <returns>the length of the list.</returns>
int listLength(ListElem head);//, ListElem* addresses, int maxAddresses);

// Removes element by index

/// <summary>
/// Removes an element from the list by its index.
/// </summary>
/// <param name="head">Pointer to the head of the list.</param>
/// <param name="index">The index of the element to remove.</param>
/// <returns>Pointer to the head of the list after removal.</returns>
ListElem removeElementByIndex(ListElem head, int index);

// Applies show function to all elements on the list, iteratively

/// <summary>
/// Iteratively shows the contents of a linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="show">Function pointer to a function that prints the data of a node.</param>
void showListIterative(ListElem head, void (*show)(void* data));

// Insert a new element with *data in the head of the list
// *data must point to storage in the heap

/// <summary>
/// Adds an element to the head of the list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the new node to add.</param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem addItemHead(ListElem head, void* data);

// Insert a new element with *data in last position of the list, iteratively
// *data must point to storage in the heap

/// <summary>
/// Iteratively adds an element to the end of the list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the new node to add.</param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem addItemLastIterative(ListElem head, void* data);

// Insert a new element with *data in the list according to the order defined by function compare, iteratively
// *data must point to storage in the heap

/// <summary>
/// Iteratively adds a new item to a linked list in ascending order, based on the given compare function.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the new node to add.</param>
/// <param name="compare"Function pointer to a function that compares the data of two nodes.></param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem addItemOrderedIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2));

// Removes the first item on the list that compares positively with *data through function compare, iteratively

/// <summary>
/// Removes an item with the given data from a singly linked list, based on the given compare function. 
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the node to remove.</param>
/// <param name="compare">Function pointer to a function that compares the data of two nodes.</param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem removeItemIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2));

// Delete list

/// <summary>
///  Deletes the entire linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <returns>Pointer to the head of the deleted linked list, which is NULL.</returns>
ListElem deleteList(ListElem head);

// Element on the list on specific position

/// <summary>
/// Obtains the element at the given position in a linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="pos">The position of the element to obtain.</param>
/// <returns>The element at the given position, or NULL if the list is empty or the position is invalid.</returns>
ListElem obtainElementPosition(ListElem head, int pos);
