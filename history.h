#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"

typedef struct datahistory
{
    char user[50];
    char transport[50];
    char start[50];
    char finish[50];
    float distance;
    float duration;
    float cost;
} *History;

void showHistory(void* data);

ListElem loadDataHistory(ListElem listHistory);

void storeDataHistory(ListElem listH);

ListElem filterHistoryByUser(ListElem originalList, char* userToFilter);

ListElem filterHistoryByTransport(ListElem originalList, char* transportToFilter);

ListElem filterHistoryByType(ListElem originalList, char* transportToFilter);

ListElem filterHistory(ListElem listHistory, int by);

float averageDistance(ListElem listHistory);

float averageDuration(ListElem listHistory);

float averageCost(ListElem listHistory);

char* mostCommonTransport(ListElem list);

void historyClientStats(ListElem listHistory, char* username);
