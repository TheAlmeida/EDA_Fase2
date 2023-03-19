#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// <summary>
/// 
/// </summary>
typedef struct sListElem
{
    void* data; // endereço de memória remetendo para o(s) dado(s) a representar
    struct sListElem* next;
} *ListElem, SListElem;

// List length
/// <summary>
/// 
/// </summary>
int listLength(ListElem head);//, ListElem* addresses, int maxAddresses);

// Removes element by index
/// <summary>
/// 
/// </summary>
ListElem removeElementByIndex(ListElem head, int index);

// Applies show function to all elements on the list, iteratively
/// <summary>
/// 
/// </summary>
void showListIterative(ListElem head, void (*show)(void* data));

// Insert a new element with *data in the head of the list
// *data must point to storage in the heap
/// <summary>
/// 
/// </summary>
ListElem addItemHead(ListElem head, void* data);

// Insert a new element with *data in last position of the list, iteratively
// *data must point to storage in the heap
/// <summary>
/// 
/// </summary>
ListElem addItemLastIterative(ListElem head, void* data);

// Insert a new element with *data in the list according to the order defined by function compare, iteratively
// *data must point to storage in the heap
/// <summary>
/// 
/// </summary>
ListElem addItemOrderedIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2));

// Removes the first item on the list that compares positively with *data through function compare, iteratively
/// <summary>
/// 
/// </summary>
ListElem removeItemIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2));

// Delete list
/// <summary>
/// 
/// </summary>
ListElem deleteList(ListElem head);

// Element on the list on specific position
/// <summary>
/// 
/// </summary>
ListElem obtainElementPosition(ListElem head, int pos);
