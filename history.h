#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"

/// <summary>
/// Struct for the history (of trips) data
/// </summary>
typedef struct datahistory
{
	char user[50]; ///< Username of the client that made the trip
	char transport[50]; ///< Type of the electric mobility transport used for the trip
	char start[50]; ///< Starting geolocation of the trip
	char finish[50]; ///< Finishing geolocation of the trip
	float distance; ///< Distance of the trip in kilometers
	float duration; ///< Duration of the trip in minutes
	float cost; ///< Cost of the trip
} *History; ///<Pointer to datavehicle struct

/// <summary>
/// Shows history data on the console.
/// </summary>
/// <param name="data">A pointer to the history data.</param>
void showHistory(void* data);

/// <summary>
/// Loads history data from a file into a linked list.
/// </summary>
/// <param name="listHistory">A pointer to the head of the linked list of histories.</param>
/// <returns>A pointer to the linked list of histories with the loaded data.</returns>
ListElem loadDataHistory(ListElem listHistory);

/// <summary>
/// Stores history data from a linked list into a file.
/// </summary>
/// <param name="listH">A pointer to the linked list of histories.</param>
void storeDataHistory(ListElem listH);

/// <summary>
/// Returns a new list of histories that have a the client in common.
/// </summary>
/// <param name="originalList">A pointer to the linked list of histories.</param>
/// <param name="userToFilter">String passed as the user to compare to.</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterHistoryByUser(ListElem originalList, char* userToFilter);

/// <summary>
/// Returns a new list of histories that have a the vehicle used in common.
/// </summary>
/// <param name="originalList">A pointer to the linked list of histories.</param>
/// <param name="transportToFilter">String passed as the vehicle to compare to.</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterHistoryByTransport(ListElem originalList, char* transportToFilter);

/// <summary>
/// Returns a new list of histories that have a type in common.
/// </summary>
/// <param name="originalList">A pointer to the linked list of histories.</param>
/// <param name="transportToFilter">String passed as the type to compare to.</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterHistoryByType(ListElem originalList, char* transportToFilter);

/// <summary>
/// Returns a new list of histories after being passed down to one of the other 3 filterHistory functios.
/// </summary>
/// <param name="listHistory">A pointer to the linked list of histories.</param>
/// <param name="by">Determines which of the other filterHistory functions to call (0 is by type, 1 is by vehicle, 2 is by user).</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterHistory(ListElem listHistory, int by);

/// <summary>
/// Returns the average distance of all the trips recorded in the histories list.
/// </summary>
/// <param name="listHistory">A pointer to the linked list of histories.</param>
/// <returns>The average distance of the trips recorded in all histories.</returns>
float averageDistance(ListElem listHistory);

/// <summary>
/// Returns the average duration of all the trips recorded in the histories list.
/// </summary>
/// <param name="listHistory">A pointer to the linked list of histories.</param>
/// <returns>The average duration of the trips recorded in all histories.</returns>
float averageDuration(ListElem listHistory);

/// <summary>
/// Returns the average cost of all the trips recorded in the histories list.
/// </summary>
/// <param name="listHistory">A pointer to the linked list of histories.</param>
/// <returns>The average duration of the trips recorded in all histories.</returns>
float averageCost(ListElem listHistory);

/// <summary>
/// Returns the most common vehicle used by a client.
/// </summary>
/// <param name="list">A pointer to the linked list of histories of only a client.</param>
/// <returns></returns>
char* mostCommonTransport(ListElem list);

/// <summary>
/// Prints the averages of a user as well as their full history.
/// </summary>
/// <param name="listHistory">A pointer to the linked list of histories of only a client.</param>
/// <param name="username"></param>
void historyClientStats(ListElem listHistory, char* username);
