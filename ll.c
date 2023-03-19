#include "ll.h"

int listLength(ListElem head)
{
    int i = 0;

    while (head != NULL)
    {
        head = head->next;
        i++;
    }

    return i;
}

ListElem removeElementByIndex(ListElem head, int index) {
    // Handle empty list
    if (head == NULL) {
        return NULL;
    }

    // Special case: remove head node
    if (index == 0) {
        ListElem tempList = head;
        head = head->next;
        free(tempList);
        return head;
    }

    // Find the node before the one to remove
    ListElem currentList = head;
    for (int i = 0; currentList != NULL && i < index - 1; i++) {
        currentList = currentList->next;
    }

    // If index is out of range
    if (currentList == NULL || currentList->next == NULL) {
        return head;
    }

    // Remove the node at the specified index
    ListElem tempList = currentList->next;
    currentList->next = tempList->next;
    free(tempList);

    return head;
}

void showListIterative(ListElem head, void (*show)(void* data))
{
    int index = 1;
    while (head != NULL)
    {
        printf(" %d.\n", index);
        show(head->data);
        head = head->next;
        index++;
    }
}

ListElem addItemHead(ListElem head, void* data)
{
    ListElem aux;

    if ((aux = (ListElem)malloc(sizeof(SListElem))) == NULL)
        return head;

    aux->data = data;
    aux->next = head;

    return aux;
}

ListElem addItemLastIterative(ListElem head, void* data)
{
    ListElem aux, prev;

    aux = (ListElem)malloc(sizeof(SListElem));
    aux->data = data;
    aux->next = NULL;

    if (head == NULL)
        return aux;

    for (prev = head; prev->next != NULL; prev = prev->next)
        ;

    prev->next = aux;

    return head;
}

ListElem addItemOrderedIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2))
{
    ListElem n, aux;

    n = (ListElem)malloc(sizeof(SListElem));
    n->data = data;
    n->next = NULL;

    if (head == NULL || compare(data, head->data) < 0)
    {
        n->next = head;
        return n;
    }

    for (aux = head; aux->next != NULL; aux = aux->next)
    {
        if (compare(data, aux->next->data) < 0)
        {
            n->next = aux->next; // Inserção
            aux->next = n;		 // Inserção
            return head;
        }
    }

    aux->next = n;
    return head;
}

ListElem removeItemIterative(ListElem head, void* data, int (*compare)(void* data1, void* data2))
{
    ListElem aux, prev;

    // List is empty
    if (head == NULL)
        return NULL;

    // Element to remove is on the head of the list
    if (compare(head->data, data) == 1)
    {
        aux = head->next;
        free(head);
        return aux;
    }

    // Element to remove is in the tail of the list
    for (prev = head; prev->next != NULL; prev = prev->next)
    {
        if (compare(prev->next->data, data) == 1)
        {
            aux = prev->next;
            prev->next = prev->next->next;
            free(aux);
            break;
        }
    }

    return head;
}

ListElem obtainElementPosition(ListElem head, int pos)
{
    // Handle empty list
    if (head == NULL) {
        return NULL;
    }

    // Find the node
    ListElem currentElem = head;
    for (int i = 0; currentElem != NULL && i < pos; i++) {
        currentElem = currentElem->next;
    }
    
    return currentElem;
}

ListElem deleteList(ListElem head) {
    ListElem current = head;
    while (current != NULL) {
        ListElem next = current->next;
        free(current->data); // assumes that each element's data field was dynamically allocated
        free(current);
        current = next;
    }
    return NULL;
}

/*
ListElem addItemLastRecursive(ListElem head, void *data) {
    ListElem aux;

    if(head == NULL) {
        aux = (ListElem) malloc(sizeof(SListElem));
        aux->data = data;
        aux->next = NULL;
        return aux;
    }

    head->next = addItemLastRecursive(head->next, data);

    return head;
}
*/
/*
void showListRecursive(ListElem head, void (*show)(void *data)) {
    if(head != NULL) {
        show(head->data);
        showListRecursive(head->next, show);
    }
}
*/
/*
ListElem removeItemRecursive(ListElem head, void *data, int (*compare)(void *data1, void *data2)) {
    ListElem aux;

    if(head == NULL)
        return NULL;

    if(compare(head->data, data) == 1) {
        aux = head->next;
        free(head);
        return aux;
    } else {
        head->next = removeItemRecursive(head->next, data, compare);
        return head;
    }
}
*/
/*
ListElem addItemOrderedRecursive(ListElem head, void *data, int (*compare)(void *data1, void *data2)) {
    ListElem aux;

    if(head == NULL || compare(data, head->data) < 0) {
        aux = (ListElem) malloc(sizeof(SListElem));
        aux->data = data;
        aux->next = head;
        return aux;
    }

    head->next = addItemOrderedRecursive(head->next, data, compare);
    return head;
}
*/