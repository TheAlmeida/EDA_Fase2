#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "ll.h"

/// <summary>
/// Struct for the vehicle data
/// </summary>
typedef struct datavehicle
{
	int code; ///< Code of the electric mobility transport
	char type[50]; ///< Type of the vehicle
	char geolocation[50]; ///< Location of the vehicle (e.g. just.like.this)
	float weight; ///< Weight of the vehicle (in Kg)
	float battery; ///< Battery percentage of the vehicle
	float autonomy; ///< Autonomy in kilometers of the vehicle
	float costhour; ///< Cost per hour of the vehicle
	float costkm; ///< Cost per kilometer of the vehicle
	int inUse; ///< 1 if the vehicle is currently in use, 0 otherwise
	float totalkms; ///< Total kilometers driven by the vehicle
} *Vehicle; ///<Pointer to datavehicle struct

/// <summary>
/// Shows vehicle data on the console.
/// </summary>
/// <param name="data">A pointer to the vehicle data.</param>
void showVehicle(void* data);

/// <summary>
/// Shows vehicle data on the console to the client (reduced info).
/// </summary>
/// <param name="data">A pointer to the vehicle data.</param>
void showVehicleRent(void* data);

/// <summary>
/// Loads vehicle data from a file into a linked list.
/// </summary>
/// <param name="listVehicles">A pointer to the head of the linked list of vehicles.</param>
/// <returns>A pointer to the linked list of vehicles with the loaded data.</returns>
ListElem loadDataVehicles(ListElem listVehicles);

/// <summary>
/// Stores vehicle data from a linked list into a file.
/// </summary>
/// <param name="listV">A pointer to the head of the linked list of vehicles.</param>
void storeDataVehicles(ListElem listV);

/// <summary>
/// Compares two vehicle data pointers by their codes.
/// </summary>
/// <param name="data1">A pointer to the first vehicle data.</param>
/// <param name="data2">A pointer to the second vehicle data.</param>
/// <returns>An integer value indicating the result of the comparison (1 if data1 is smaller, 0 if they are equal, -1 if data1 is greater).</returns>
int compare(void* data1, void* data2);

/// <summary>
/// Compares two vehicle data pointers by their autonomy in descending order.
/// </summary>
/// <param name="data1">A pointer to the first vehicle data.</param>
/// <param name="data2">A pointer to the second vehicle data.</param>
/// <returns>An integer value indicating the result of the comparison (1 if data1 is greater, 0 if they are equal, -1 if data1 is smaller).</returns>
int compareAutonomy(void* data1, void* data2);

/// <summary>
/// Compares two vehicle data pointers by their type and then by their code.
/// </summary>
/// <param name="data1">A pointer to the first vehicle data.</param>
/// <param name="data2">A pointer to the second vehicle data.</param>
/// <returns>An integer value indicating the result of the comparison (-1 if data1 is lexically smaller, 0 if they are equal, 1 if data1 is lexically greater).</returns>
int compareNamesVehicles(void* data1, void* data2);

/// <summary>
/// Compares two vehicle data pointers by their total kilometers driven in descending order.
/// </summary>
/// <param name="data1">A pointer to the first vehicle data.</param>
/// <param name="data2">A pointer to the second vehicle data.</param>
/// <returns>An integer value indicating the result of the comparison (1 if data1 is greater, 0 if they are equal, -1 if data1 is smaller).</returns>
int compareTotalKms(void* data1, void* data2);

void changeVType(Vehicle vehicle, const char* newType);

void changeVCode(Vehicle vehicle, int code);

void changeVGeolocation(Vehicle vehicle, const char* newGeolocation);

void changeVWeight(Vehicle vehicle, float newWeight);

void changeVBattery(Vehicle vehicle, float newBattery);

void changeVAutonomy(Vehicle vehicle, float newAutonomy);

void changeVCosthour(Vehicle vehicle, float newCosthour);

void changeVCostkm(Vehicle vehicle, float newCostkm);

void changeVstate(Vehicle vehicle, int inUse);

void changeVkms(Vehicle vehicle, float newTotalkms);

/// <summary>
/// Removes a vehicle from the list of vehicles and returns the new list.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <param name="modified">A pointer to a variable that will be set to 1 if the list is modified, 0 otherwise</param>
/// <returns>The new list of vehicles.</returns>
ListElem removeVehicle(ListElem listVehicle, int* modified, Vehicle* v);

/// <summary>
/// Sorts the list of vehicles by the total kilometers driven by each vehicle, in descending order.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <returns>The sorted list of vehicles</returns>
ListElem sortByTotalKms(ListElem listVehicle);

/// <summary>
/// Returns the percentage of vehicles that are currently in use.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <returns>The percentage of vehicles in use.</returns>
float percentageInUse(ListElem listVehicle);

/// <summary>
/// Returns the average autonomy of the vehicles in the list.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <returns>The average autonomy of the vehicles.</returns>
float averageAutonomy(ListElem listVehicle);

/// <summary>
/// Refill the battery level and autonomy of a vehicle.
/// </summary>
/// <param name="vehicle">Vehicle whose battery level and autonomy are to be updated.</param>
void refillBatteryAndAutonomy(Vehicle vehicle);

/// <summary>
/// Returns a new list of vehicles that have a battery percentage lesser than 50%.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterVehicleByBattery(ListElem listVehicle);

/// <summary>
/// Returns a new list of vehicles that are located in the specified geolocation.
/// </summary>
/// <param name="listV">A pointer to the head of the linked list of vehicles.</param>
/// <param name="geolocation">The geolocation to filter by.</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterVehicleByGeo(ListElem listV, char* geolocation);

/// <summary>
/// Returns a new list of vehicles that are currently not in use
/// </summary>
/// <param name="listV">A pointer to the head of the linked list of vehicles.</param>
/// <returns>The new list of vehicles.</returns>
ListElem filterVehicleByInUse(ListElem listV);

/// <summary>
/// Returns the vehicle of the linked list with the specified type and code.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <param name="type">The type of the vehicle.</param>
/// <param name="code">The code of the vehicle.</param>
/// <returns>The pointer of the vehicle with the specified type and code, or NULL if it does not exist in the list.</returns>
Vehicle getVehicleByTypeAndCode(ListElem listVehicle, char* type, int code);

/// <summary>
/// Retrieves a vehicle from the vehicle list based on its type.
/// </summary>
/// <param name="listVehicle">Head pointer to the head of the vehicle list.</param>
/// <param name="type">Type of the vehicle to retrieve.</param>
/// <returns>The vehicle with the specified type, or NULL if not found.</returns>
Vehicle getVehicleByType(ListElem listVehicle, char* type);

/// <summary>
/// Edits the data of a vehicle in the list.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a modification is made, 0 otherwise.</param>
/// <returns>The modified list of vehicles.</returns>
ListElem editVehicle(ListElem listVehicle, int* modified, Vehicle* v, char* oldGeolocation);

/// <summary>
/// Checks if a vehicle with the given code and type already exists in the list of vehicles.
/// </summary>
/// <param name="code">The code of the vehicle.</param>
/// <param name="type">The type of the vehicle.</param>
/// <param name="listVehicles">A pointer to the head of the linked list of vehicles.</param>
/// <returns>1 if the vehicle exists, 0 otherwise.</returns>
int vehicleExists(int code, char* type, ListElem listVehicles);

/// <summary>
/// Checks if the given geolocation is valid (three words separated by two dots, e.g. just.like.this).
/// </summary>
/// <param name="geolocation">The geolocation string to validate.</param>
/// <returns>1 if the geolocation is valid, 0 otherwise.</returns>
int validGeolocation(char* geolocation);

/// <summary>
/// Registers a new vehicle in the list.
/// </summary>
/// <param name="listVehicle">A pointer to the head of the linked list of vehicles.</param>
/// <param name="modified">Pointer to an integer that will be set to 1 if a new vehicle is added, 0 otherwise.</param>
/// <returns>The modified list of vehicles.</returns>
ListElem registerVehicle(ListElem listVehicle, int* modified, Vehicle v);
