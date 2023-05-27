#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "ll.h"
#include "client.h"
#include "vehicle.h"
#include "what3words.h"

typedef struct {
    ListElem pickedUpVehicles;
    int vehicleTooHeavy;
} PickUpResult;

typedef struct sLocation
{
    char name[50];
    Coordinates coordinates;
    struct sListElem* adjacentLocations;
    ListElem clientList;   ///< List of client pointers associated with the location
    ListElem vehicleList;  ///< List of vehicle pointers associated with the location
} Location;

typedef struct sAdjacentLocation
{
    struct sLocation* location;
    double weight;
} AdjacentLocation;

typedef struct sVisitedLocation {
    Location* location;
    double distance;
    struct sVisitedLocation* previous;
} VisitedLocation;

typedef struct sGraph
{
    struct sListElem* locations;
} Graph;

void storeDataGraph(const Graph* graph);

Graph* loadDataGraph(Graph* graph, ListElem listClients, ListElem listVehicles);

// Function to create a new graph
Graph* createGraph();

// Function to create a new location
Location* createLocation(const char* name, double latitude, double longitude);

// Function to add a location to the graph
void addLocation(Graph* graph, Location* location);

// Function to add an adjacent location to a location
void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight);

// Function to add a client info to a location
void addClientToLocation(Location* location, Client client);

void removeClientFromLocation(Location* location, const char* username);

void addVehicleToLocation(Location* location, Vehicle vehicle);

void removeVehicleFromLocation(Location* location, int code, const char* type);

void createLocationsFromClients(Graph* graph, ListElem listC);

// Function to create locations based on unique geolocations found in vehicles and add them to the graph
void createLocationsFromVehicles(Graph* graph, ListElem listV);

// Function to find a location in the graph by geolocation
Location* findLocationByGeolocation(Graph* graph, const char* geolocation);

Location* findLocationByVehicle(Graph* graph, Vehicle vehicle);

Location* getRandomLocation(Graph* graph, Location* excludeLocation);

int locationHasAdjacency(Location* location, Location* otherLocation);

void connectAdjacentLocations(Graph* graph);

Location* findMinDistanceLocation(ListElem unvisitedSet);

VisitedLocation* createVisitedLocation(Location* location, double distance, VisitedLocation* previous);

void updateVisitedLocationDistance(VisitedLocation* visitedLocation, double distance);

void updateVisitedLocationPrevious(VisitedLocation* visitedLocation, VisitedLocation* previous);

VisitedLocation* findVisitedLocationByLocation(ListElem visitedSet, Location* location);

AdjacentLocation* findAdjacentLocation(Location* from, Location* to);

ListElem getShortestPath(VisitedLocation* target);

double calculatePathDistance(ListElem path);

ListElem calculateShortestPath(Graph* graph, Location* start, Location* target);

Location* calculateShortestPathWithVehicle(Graph* graph, Location* start);

void unloadVehicles(Location* start, ListElem vehicles);

PickUpResult pickUpVehicles(Location* location, float maxWeight);

int hasVehicleToPickUp(Location* location);

void printVisitedSet(ListElem visitedSet);

void printUnvisitedSet(ListElem unvisitedSet);

void showPath(ListElem path);

// Function to print the graph
void showGraph(Graph* graph);

// Frees the memory allocated for a path
void freePath(ListElem path);

// Function to free the memory allocated for the graph
void freeGraph(Graph* graph);
