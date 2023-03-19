#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "ll.h"
#include "client.h"

/// <summary>
/// Struct for the admin data.
/// </summary>
typedef struct dataadmin
{
	char username[20]; ///< Username of the administrator
	char password[50]; ///< Password of the administrator
	char name[50]; ///< Name of the administrator
	char email[50]; ///< Email address of the administrator
} *Admin; ///<Pointer to dataadmin struct

/// <summary>
/// Shows admin data on the console.
/// </summary>
/// <param name="data">A pointer to admin data.</param>
void showAdmin(void* data);

/// <summary>
/// Loads admin data from a file into a linked list.
/// </summary>
/// <param name="listAdmins">A pointer to the head of the linked list of admins.</param>
/// <returns>A pointer to the linked list of admins with the loaded data.</returns>
ListElem loadDataAdmins(ListElem listAdmins);

/// <summary>
/// Stores admin data from a linked list into a file.
/// </summary>
/// <param name="listA">A pointer to the head of the linked list of admins.</param>
void storeDataAdmins(ListElem listA);

/// <summary>
/// Compares lexically two admin data pointers by their username.
/// </summary>
/// <param name="data1">A pointer to the first admin data.</param>
/// <param name="data2">A pointer to the first admin data.</param>
/// <returns>An integer value indicating the result of the comparison (-1 if data1 is lexically smaller, 0 if they are equal, 1 if data1 is lexically greater).</returns>
int compareNamesAdmins(void* data1, void* data2);

void changeAUsername(Admin admin, char* newUsername);

void changeAPassword(Admin admin, char* newPassword);

void changeAName(Admin admin, char* newName);

void changeAEmail(Admin admin, char* newEmail);

/// <summary>
/// Removes an admin from the list of admins and returns the new list.
/// </summary>
/// <param name="listAdmin">A pointer to the linked list of admins.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new admin is added, 0 otherwise.</param>
/// <returns>The modified list of admins.</returns>
ListElem removeAdmin(ListElem listAdmin, int* modified);

/// <summary>
/// Edits the data of an admin in the list.
/// </summary>
/// <param name="listAdmin">A pointer to the linked list of admins.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new admin is added, 0 otherwise.</param>
/// <returns>The modified list of admins.</returns>
ListElem editAdmin(ListElem listAdmin, int* modified);

/// <summary>
/// Registers a new admin in the list.
/// </summary>
/// <param name="listAdmin">A pointer to the head of the linked list of admins.</param>
/// <param name="listClient"A pointer to the head of the linked list of clients, (needed to check if an username that is alredy registed with a client is not used by an admin).></param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new admin is added, 0 otherwise.</param>
/// <returns>The modified list of admins.</returns>
ListElem registerAdmin(ListElem listAdmin, ListElem listClient, int* modified);

