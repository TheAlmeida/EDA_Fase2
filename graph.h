#pragma once
#include "ll.h"

typedef struct sGraphNode
{
    void* data; ///< Pointer to the data stored in this node
    ListElem adj_list; ///< Pointer to the adjacency list for this node
} *GraphNode, SGraphNode;
