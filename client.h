#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "ll.h"
#include "admin.h"

typedef struct dataclient
{
    char username[20];
    char password[50];
    char name[50];
    char email[50];
    char address[50];
    int nif;
    int cellphone;
    int age;
    int totaltrips;
    float totalkms;
    float balance;
    //BankAccount bankaccount[5];
} *Client;

void showClient(void* data);

ListElem loadDataClients(ListElem listClients);

void storeDataClients(ListElem listC);

int compareNamesClients(void* data1, void* data2);

void changeCUsername(Client client, char* newUsername);

void changeCPassword(Client client, char* newPassword);

void changeCName(Client client, char* newName);

void changeCEmail(Client client, char* newEmail);

void changeCCellphone(Client client, int newCellphone);

void changeCAge(Client client, int newAge);

void changeCAddress(Client client, char* newAddress);

void changeCTotalKms(Client client, float newTotalKms);

void changeCBalance(Client client, float newBalance);

void changeCNif(Client client, int newNif);

void changeCTotalTrips(Client client, int newTotalTrips);

ListElem removeClient(ListElem listClient, int* modified);

ListElem sortByTrips(ListElem listClient);

void findClientWithMostTrips(ListElem listClient, char* username, int* totaltrips);

float calculateAverageAge(ListElem listClient);

ListElem editClient(ListElem listClient, int* modified);

int usernameExists(char* username, ListElem listClients, ListElem listAdmin);

ListElem registerClient(ListElem listClient, ListElem listAdmin, int* modified);

Client addBalance(Client c);

