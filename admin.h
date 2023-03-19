#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "ll.h"
#include "client.h"

typedef struct dataadmin
{
    char username[20];
    char password[50];
    char name[50];
    char email[50];
} *Admin;

void showAdmin(void* data);

ListElem loadDataAdmins(ListElem listAdmins);

void storeDataAdmins(ListElem listA);

int compareNamesAdmins(void* data1, void* data2);

void changeAUsername(Admin admin, char* newUsername);

void changeAPassword(Admin admin, char* newPassword);

void changeAName(Admin admin, char* newName);

void changeAEmail(Admin admin, char* newEmail);

ListElem removeAdmin(ListElem listAdmin, int* modified);

ListElem editAdmin(ListElem listAdmin, int* modified);

ListElem registerAdmin(ListElem listAdmin, ListElem listClient, int* modified);

