#include "ll.h"

// Function to create a new list element
ListElem createListElem(void* data)
{
    ListElem elem = (ListElem)malloc(sizeof(SListElem));
    elem->data = data;
    elem->next = NULL;
    return elem;
}

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

void reorderList(ListElem* head, int (*compare)(void* data1, void* data2)) {
    ListElem newHead = NULL;

    // Iterate through the original list
    ListElem current = *head;
    while (current != NULL) {
        ListElem next = current->next;

        // Remove the current node from the original list
        current->next = NULL;

        // Insert the current node into the new list in the correct order
        newHead = addItemOrderedIterative(newHead, current->data, compare);

        // Move to the next node in the original list
        current = next;
    }

    // Update the head of the original list to point to the reordered list
    *head = newHead;
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

ListElem appendList(ListElem list1, ListElem list2) {
    if (list1 == NULL) {
        return list2;
    }
    else if (list2 == NULL) {
        return list1;
    }
    else {
        // Both lists are not NULL, append list2 at the end of list1
        ListElem temp = list1;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = list2;
        return list1;
    }
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
