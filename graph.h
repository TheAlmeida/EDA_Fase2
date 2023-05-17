#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "client.h"
#include "vehicle.h"
#include "what3words.h"

typedef struct sClientInfo
{
    char username[50];        ///< Username of the client
    struct sClientInfo* next;  ///< Pointer to the next client info in the list
} ClientInfo;

typedef struct sVehicleInfo
{
    int code;             ///< Code of the vehicle
    char type[50];        ///< Type of the vehicle
    struct sVehicleInfo* next;  ///< Pointer to the next vehicle info in the list
} VehicleInfo;

typedef struct sLocation
{
    char name[50];
    Coordinates coordinates;
    struct sListElem* adjacentLocations;
    struct sClientInfo* clientInfo;   ///< Client information associated with the location
    struct sVehicleInfo* vehicleInfo;  ///< Vehicle information associated with the location
} Location;

typedef struct sAdjacentLocation
{
    struct sLocation* location;
    double weight;
} AdjacentLocation;

typedef struct sGraph
{
    struct sListElem* locations;
} Graph;

void storeDataGraphBin(const Graph* graph);

Graph* loadDataGraphBin(Graph* graph);

// Function to create a new graph
Graph* createGraph();

// Function to create a new location
Location* createLocation(const char* name, double latitude, double longitude);

// Function to add a location to the graph
void addLocation(Graph* graph, Location* location);

// Function to add an adjacent location to a location
void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight);

// Function to create a new client info
ClientInfo* createClientInfo(const char* username);

// Function to add a client info to a location
void addClientInfo(Location* location, ClientInfo* clientInfo);

VehicleInfo* createVehicleInfo(int code, const char* type);

void addVehicleInfo(Location* location, VehicleInfo* vehicleInfo);

void createLocationsFromClients(Graph* graph, ListElem listC);

// Function to create locations based on unique geolocations found in vehicles and add them to the graph
void createLocationsFromVehicles(Graph* graph, ListElem listV);

// Function to find a location in the graph by geolocation
Location* findLocationByGeolocation(Graph* graph, const char* geolocation);

void connectAdjacentLocations(Graph* graph);

Location* getRandomLocation(Graph* graph, Location* excludeLocation);

// Function to print the graph
void printGraph(Graph* graph);

// Function to free the memory allocated for the graph
void freeGraph(Graph* graph);
