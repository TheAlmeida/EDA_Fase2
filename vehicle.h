#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "ll.h"

typedef struct datavehicle
{
    int code; // código do transport de mobilidade elétrica
    char type[50];
    char geolocation[50];
    float battery;
    float autonomy;
    float costhour;
    float costkm;
    int inUse;
    float totalkms;
} *Vehicle;


void showVehicle(void* data);

ListElem loadDataVehicles(ListElem listVehicles);

void storeDataVehicles(ListElem listV);

int compare(void* data1, void* data2);

int compareAutonomy(void* data1, void* data2);

int compareNamesVehicles(void* data1, void* data2);

int compareTotalKms(void* data1, void* data2);

int equal(void* data1, void* data2);

void changeVType(Vehicle vehicle, const char* newType);

void changeVCode(Vehicle vehicle, int code);

void changeVGeolocation(Vehicle vehicle, const char* newGeolocation);

void changeVBattery(Vehicle vehicle, float newBattery);

void changeVAutonomy(Vehicle vehicle, float newAutonomy);

void changeVCosthour(Vehicle vehicle, float newCosthour);

void changeVCostkm(Vehicle vehicle, float newCostkm);

void changeVstate(Vehicle vehicle, int inUse);

void changeVkms(Vehicle vehicle, float newTotalkms);

ListElem removeVehicle(ListElem listVehicle, int* modified);

ListElem sortByTotalKms(ListElem listVehicle);

float percentageInUse(ListElem listVehicle);

float averageAutonomy(ListElem listVehicle);

ListElem filterVehicleByBattery(ListElem listVehicle);

ListElem filterVehicleByGeo(ListElem listV, char* geolocation);

ListElem filterVehicleByInUse(ListElem listV);

Vehicle getVehicleByTypeAndCode(ListElem listVehicle, char* type, int code);

ListElem editVehicle(ListElem listVehicle, int* modified);

int vehicleExists(int code, char* type, ListElem listVehicles);

ListElem registerVehicle(ListElem listVehicle, int* modified);
