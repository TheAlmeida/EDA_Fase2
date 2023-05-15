#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "vehicle.h"
#include "what3words.h"

typedef struct sVehicleInfo
{
    int code;             ///< Code of the vehicle
    char type[50];        ///< Type of the vehicle
    struct sVehicleInfo* next;  ///< Pointer to the next vehicle info in the list
} VehicleInfo;

typedef struct sLocation
{
    char name[50];
    struct sListElem* adjacentLocations;
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

void storeDataGraphTxt(const Graph* graph);

Graph* loadDataGraphTxt(Graph* graph);

void storeDataGraphBin(const Graph* graph);

Graph* loadDataGraphBin(Graph* graph);

// Function to create a new graph
Graph* createGraph();

// Function to create a new location
Location* createLocation(const char* name);

// Function to add a location to the graph
void addLocation(Graph* graph, Location* location);

// Function to add an adjacent location to a location
void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight);

VehicleInfo* createVehicleInfo(int code, const char* type);

void addVehicleInfo(Location* location, VehicleInfo* vehicleInfo);

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
