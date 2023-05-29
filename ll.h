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

/// <summary>
/// Creates a new ListElem.
/// </summary>
/// <param name="data">Data pointer to the data of the new element.</param>
/// <returns></returns>
ListElem createListElem(void* data);

/// <summary>
/// Returns the length of the linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <returns>the length of the list.</returns>
int listLength(ListElem head);

/// <summary>
/// Reorders the content of the linked list.
/// </summary>
/// <param name="head">Pointer to the head of the list.</param>
/// <param name="compare">Function pointer to a function that compares the data of two nodes.</param>
void reorderList(ListElem* head, int (*compare)(void* data1, void* data2));

/// <summary>
/// Removes an element from the list by its index.
/// </summary>
/// <param name="head">Pointer to the head of the list.</param>
/// <param name="index">The index of the element to remove.</param>
/// <returns>Pointer to the head of the list after removal.</returns>
ListElem removeElementByIndex(ListElem head, int index);

/// <summary>
/// Iteratively shows the contents of a linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="show">Function pointer to a function that prints the data of a node.</param>
void showListIterative(ListElem head, void (*show)(void* data));

/// <summary>
/// Adds an element to the head of the list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the new node to add.</param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem addItemHead(ListElem head, void* data);

/// <summary>
/// Iteratively adds an element to the end of the list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the new node to add.</param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem addItemLastIterative(ListElem head, void* data);

/// <summary>
/// Iteratively adds a new item to a linked list in ascending order, based on the given compare function.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the new node to add.</param>
/// <param name="compare">Function pointer to a function that compares the data of two nodes.></param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem addItemOrderedIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2));

/// <summary>
/// Removes an item with the given data from a singly linked list, based on the given compare function. 
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="data">Data pointer to the data of the node to remove.</param>
/// <param name="compare">Function pointer to a function that compares the data of two nodes.</param>
/// <returns>Pointer to the head of the list after adding.</returns>
ListElem removeItemIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2));

/// <summary>
///  Deletes the entire linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <returns>Pointer to the head of the deleted linked list, which is NULL.</returns>
ListElem deleteList(ListElem head);

/// <summary>
/// Appends the head of the second linked list to the end of the first linked list.
/// </summary>
/// <param name="firstList">Pointer to the head of the first linked list.</param>
/// <param name="secondList">Pointer to the head of the second linked list.</param>
/// <returns></returns>
ListElem appendList(ListElem firstList, ListElem secondList);

/// <summary>
/// Obtains the element at the given position in a linked list.
/// </summary>
/// <param name="head">Head pointer to the head of the list.</param>
/// <param name="pos">The position of the element to obtain.</param>
/// <returns>The element at the given position, or NULL if the list is empty or the position is invalid.</returns>
ListElem obtainElementPosition(ListElem head, int pos);
