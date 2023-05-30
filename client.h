#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "ll.h"
#include "admin.h"
#include "what3words.h"

/// <summary>
/// Struct for the client data
/// </summary>
typedef struct dataclient
{
	char username[20]; ///< Username of the client
	char password[50]; ///< Password of the client
	char name[50]; ///< Name of the client
	char email[50]; ///< Email geolocation of the client
	char geolocation[50]; ///< Geolocation of the client
	int nif; ///< Tax identification number of the client
	int cellphone; ///< Cellphone number of the client
	int age; ///< Age of the client
	int totaltrips; ///< Total number of trips made by the client
	float totalkms; ///< Total distance traveled by the client in kilometers
	float balance; ///< Balance of the client
} *Client; ///<Pointer to dataclient struct

/// <summary>
/// Shows client data on the console.
/// </summary>
/// <param name="data">A pointer to client data.</param>
void showClient(void* data);

/// <summary>
/// Loads client data from a file into a linked list.
/// </summary>
/// <param name="listClients">A pointer to the head of the linked list of clients.</param>
/// <returns>A pointer to the linked list of clients with the loaded data.</returns>
ListElem loadDataClients(ListElem listClients);

/// <summary>
/// Stores client data from a linked list into a file.
/// </summary>
/// <param name="listC">A pointer to the head of the linked list of clients.</param>
void storeDataClients(ListElem listC);

/// <summary>
/// Compares two client data pointers by their username.
/// </summary>
/// <param name="data1">A pointer to the first client data.</param>
/// <param name="data2">A pointer to the first client data.</param>
/// <returns>An integer value indicating the result of the comparison (-1 if data1 is lexically smaller, 0 if they are equal, 1 if data1 is lexically greater).</returns>
int compareNamesClients(void* data1, void* data2);

/// <summary>
/// Compares two client data pointers by their number of trips
/// </summary>
/// <param name="data1">A pointer to the first client data.</param>
/// <param name="data2">A pointer to the first client data.</param>
/// <returns>An integer value indicating the result of the comparison (-1 if data1 is greater, 0 if they are equal, 1 if data1 is smaller).</returns>
int compareTrips(void* data1, void* data2);

void changeCUsername(Client client, char* newUsername);

void changeCPassword(Client client, char* newPassword);

void changeCName(Client client, char* newName);

void changeCEmail(Client client, char* newEmail);

void changeCCellphone(Client client, int newCellphone);

void changeCAge(Client client, int newAge);

void changeCGeolocation(Client client, char* newGeolocation);

void changeCTotalKms(Client client, float newTotalKms);

void changeCBalance(Client client, float newBalance);

void changeCNif(Client client, int newNif);

void changeCTotalTrips(Client client, int newTotalTrips);

/// <summary>
/// Removes a client from the list of clients and returns the new list.
/// </summary>
/// <param name="listClient">A pointer to the head of the linked list of clients.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a client is removed, 0 otherwise.</param>
/// <returns>The modified list of clients.</returns>
ListElem removeClient(ListElem listClient, int* modified, Client* c);

/// <summary>
/// Sorts the list of clients by number of trips (by descending order).
/// </summary>
/// <param name="listClient">A pointer to the head of the linked list of clients.</param>
/// <returns>The sorted list of clients.</returns>
ListElem sortByTrips(ListElem listClient);

/// <summary>
/// Iterates the client list to find the max value for totaltrips and stores the totaltrips value and the corresponding username of the client in pointers passed as arguments.
/// </summary>
/// <param name="listClient">A pointer to the head of the linked list of clients.</param>
/// <param name="username">A pointer to the username of the client with the most trips.</param>
/// <param name="totaltrips">A pointer to the int that contains the max value of trips.</param>
void findClientWithMostTrips(ListElem listClient, char* username, int* totaltrips);

/// <summary>
/// Returns the average age of the clients in the list.
/// </summary>
/// <param name="listClient">A pointer to the head of the linked list of clients.</param>
/// <returns>The average age of the clients.</returns>
float calculateAverageAge(ListElem listClient);

/// <summary>
/// Edits the data of a client in the list.
/// </summary>
/// <param name="listClient">A pointer to the head of the linked list of clients.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new client is edited, 0 otherwise.</param>
/// <returns>The modified list of clients.</returns>
ListElem editClient(ListElem listClient, int* modified, Client* c, char* oldGeolocation);

/// <summary>
/// Checks if the username passed as argument exists in either of the lists.
/// </summary>
/// <param name="username">A pointer to the string to compare to the usernames in the lists.</param>
/// <param name="listClient">A pointer to the head of the linked list of clients.</param>
/// <param name="listAdmin">A pointer to the head of the linked list of admins.</param>
/// <returns>1 if the username passed as argument exists in either listClients or listAdmin, 0 otherwise.</returns>
int usernameExists(char* username, ListElem listClients, ListElem listAdmin);

/// <summary>
/// Retrieves a client with the given username from a linked list of clients.
/// </summary>
/// <param name="listClient">Head pointer to the head of the client list.</param>
/// <param name="username">Username of the client to retrieve.</param>
/// <returns>The client with the given username, or NULL if not found.</returns>
Client getClientByUsername(ListElem listClient, char* username);

/// <summary>
/// Registers a new client and adds them to the client list.
/// </summary>
/// <param name="listClient">Head pointer to the head of the client list.</param>
/// <param name="listAdmin">Head pointer to the head of the admin list.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new client is added, 0 otherwise.</param>
/// <param name="c">Client to be registered and added to the list.</param>
/// <returns>The updated client list.</returns>
ListElem registerClient(ListElem listClient, ListElem listAdmin, int* modified, Client c);

/// <summary>
/// Allows the user to attemp to add balance to their account.
/// </summary>
/// <param name="c">A pointer to client data.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new vehicle is added, 0 otherwise.</param>
/// <returns>The pointer of the client.</returns>
Client addBalance(Client c, int* modified);

