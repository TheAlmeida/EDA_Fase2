#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "ll.h"
#include "client.h"
#include "vehicle.h"
#include "what3words.h"

/// <summary>
/// Struct for the result of picking up vehicles.
/// </summary>
typedef struct {
    ListElem pickedUpVehicles;   ///< Pointer to the head of the list of picked up vehicles
    int vehicleTooHeavy;         ///< Flag indicating if a vehicle was too heavy to be picked up
} PickUpResult;

/// <summary>
/// Struct representing a location.
/// </summary>
typedef struct sLocation
{
    char name[50];                       ///< Name of the location
    Coordinates coordinates;             ///< Coordinates of the location
    struct sListElem* adjacentLocations; ///< Pointer to the head of the list of adjacent locations
    ListElem clientList;                 ///< Pointer to the head of the list of clients associated with the location
    ListElem vehicleList;                ///< Pointer to the head of the list of vehicles associated with the location
} Location;

/// <summary>
/// Struct representing an adjacent location.
/// </summary>
typedef struct sAdjacentLocation
{
    struct sLocation* location; ///< Pointer to the adjacent location
    double weight;              ///< Weight of the adjacency between the locations
} AdjacentLocation;

/// <summary>
/// Struct representing a visited location.
/// </summary>
typedef struct sVisitedLocation {
    Location* location;                     ///< Pointer to the visited location
    double distance;                        ///< Distance to the visited location
    struct sVisitedLocation* previous;      ///< Pointer to the previous visited location
} VisitedLocation;

/// <summary>
/// Struct representing a graph.
/// </summary>
typedef struct sGraph
{
    struct sListElem* locations; ///< Pointer to the head of the list of locations in the graph
} Graph;

extern const char garage[]; ///< External declaration of the garage location

/// <summary>
/// Stores the data of the graph.
/// </summary>
/// <param name="graph">Pointer to the graph to be stored.</param>
void storeDataGraph(const Graph* graph);

/// <summary>
/// Loads the data into the graph.
/// </summary>
/// <param name="graph">Pointer to the graph to be loaded.</param>
/// <param name="listClients">Pointer to the head of the list of clients to be loaded.</param>
/// <param name="listVehicles">Pointer to the head of the list of vehicles to be loaded.</param>
/// <returns>Pointer to the loaded graph.</returns>
Graph* loadDataGraph(Graph* graph, ListElem listClients, ListElem listVehicles);

/// <summary>
/// Creates a new graph.
/// </summary>
/// <returns>Pointer to the newly created graph.</returns>
Graph* createGraph();

/// <summary>
/// Creates a new location with the given name, latitude, and longitude. In case latitude and longitude is not known (= 0.0) the location is converted to coordinates and then created.
/// </summary>
/// <param name="name">Name (geolocation) of the location.</param>
/// <param name="latitude">Latitude of the location.</param>
/// <param name="longitude">Longitude of the location.</param>
/// <returns>Pointer to the newly created location.</returns>
Location* createLocation(const char* name, double latitude, double longitude);

/// <summary>
/// Adds a location to the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="location">Pointer to the location to be added.</param>
void addLocation(Graph* graph, Location* location);

/// <summary>
/// Adds an adjacent location to a location with the given weight.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="adjacentLocation">Pointer to the location to be added as an adjacent location.</param>
/// <param name="weight">Weight of the adjacency.</param>
void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight);

/// <summary>
/// Adds a client to a location.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="client">Client to be added.</param>
void addClientToLocation(Location* location, Client client);

/// <summary>
/// Removes a client from a location by it's username.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="username">Username of the client to be removed.</param>
void removeClientFromLocation(Location* location, const char* username);

/// <summary>
/// Adds a vehicle to a location.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="vehicle">Vehicle to be added.</param>
void addVehicleToLocation(Location* location, Vehicle vehicle);

/// <summary>
/// Removes a vehicle from a location by it's code and type.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="code">Code of the vehicle to be removed.</param>
/// <param name="type">Type of the vehicle to be removed.</param>
void removeVehicleFromLocation(Location* location, int code, const char* type);

/// <summary>
/// Creates locations based on unique geolocations found in clients and adds them to the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="listC">Pointer to the head of the list of clients.</param>
void createLocationsFromClients(Graph* graph, ListElem listC);

/// <summary>
/// Creates locations based on unique geolocations found in vehicles and adds them to the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="listV">Pointer to the head of the list of vehicles.</param>
void createLocationsFromVehicles(Graph* graph, ListElem listV);

/// <summary>
/// Finds a location in the graph by geolocation.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="geolocation">Geolocation of the location to be found.</param>
/// <returns>Pointer to the found location, or NULL if not found.</returns>
Location* findLocationByGeolocation(Graph* graph, const char* geolocation);

/// <summary>
/// Finds a location in the graph by vehicle.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="vehicle">Vehicle to be found.</param>
/// <returns>Pointer to the found location, or NULL if not found.</returns>
Location* findLocationByVehicle(Graph* graph, Vehicle vehicle);

/// <summary>
/// Gets a random location from the graph, excluding a specific location.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="excludeLocation">Pointer to the location to be excluded.</param>
/// <returns>Pointer to the random location, or NULL if not found.</returns>
Location* getRandomLocation(Graph* graph, Location* excludeLocation);

/// <summary>
/// Checks if a location has adjacency with another location.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="otherLocation">Pointer to the other location.</param>
/// <returns>1 if the locations are adjacent, 0 otherwise.</returns>
int locationHasAdjacency(Location* location, Location* otherLocation);

/// <summary>
/// Connects the adjacent locations in the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
void connectAdjacentLocations(Graph* graph);

/// <summary>
/// Finds the location with the minimum distance in the unvisited set of locations.
/// </summary>
/// <param name="unvisitedSet">Pointer to the head of the unvisited set.</param>
/// <returns>Pointer to the location with the minimum distance, or NULL if the set is empty.</returns>
Location* findMinDistanceLocation(ListElem unvisitedSet);

/// <summary>
/// Creates a visited location with distance and previous location information.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="distance">Distance to the location.</param>
/// <param name="previous">Pointer to the previous visited location.</param>
/// <returns>Pointer to the created visited location.</returns>
VisitedLocation* createVisitedLocation(Location* location, double distance, VisitedLocation* previous);

/// <summary>
/// Updates the distance of a visited location.
/// </summary>
/// <param name="visitedLocation">Pointer to the visited location.</param>
/// <param name="distance">New distance value.</param>
void updateVisitedLocationDistance(VisitedLocation* visitedLocation, double distance);

/// <summary>
/// Updates the previous location of a visited location.
/// </summary>
/// <param name="visitedLocation">Pointer to the visited location.</param>
/// <param name="previous">Pointer to the new previous visited location.</param>
void updateVisitedLocationPrevious(VisitedLocation* visitedLocation, VisitedLocation* previous);

/// <summary>
/// Finds a visited location in the visited set by location.
/// </summary>
/// <param name="visitedSet">Pointer to the head of the visited set.</param>
/// <param name="location">Pointer to the location to be found.</param>
/// <returns>Pointer to the found visited location, or NULL if not found.</returns>
VisitedLocation* findVisitedLocationByLocation(ListElem visitedSet, Location* location);

/// <summary>
/// Finds the adjacent location between two locations.
/// </summary>
/// <param name="from">Pointer to the "from" location.</param>
/// <param name="to">Pointer to the "to" location.</param>
/// <returns>Pointer to the adjacent location, or NULL if not found.</returns>
AdjacentLocation* findAdjacentLocation(Location* from, Location* to);

/// <summary>
/// Gets the shortest path from the visited location to the target.
/// </summary>
/// <param name="target">Pointer to the target visited location.</param>
/// <returns>Pointer to the head of the shortest path.</returns>
ListElem getShortestPath(VisitedLocation* target);

/// <summary>
/// Calculates the total distance of a path.
/// </summary>
/// <param name="path">Pointer to the head of the path.</param>
/// <returns>Total distance of the path.</returns>
double calculatePathDistance(ListElem path);

/// <summary>
/// Dijkstra implementation. Calculates the shortest path between two locations in the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="start">Pointer to the start location.</param>
/// <param name="target">Pointer to the target location.</param>
/// <returns>Pointer to the head of the shortest path, or NULL if not found.</returns>
ListElem calculateShortestPath(Graph* graph, Location* start, Location* target);

/// <summary>
/// Calculates the shortest path from a location with a vehicle.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="start">Pointer to the start location.</param>
/// <returns>Pointer to the target location found on the shortest path, or NULL if not found.</returns>
Location* calculateShortestPathWithVehicle(Graph* graph, Location* start);

/// <summary>
/// Unloads vehicles at the start location from the given list of vehicles.
/// </summary>
/// <param name="start">Pointer to the start location.</param>
/// <param name="vehicles">Pointer to the head of the list of vehicles.</param>
void unloadVehicles(Location* start, ListElem vehicles);

/// <summary>
/// Picks up vehicles from the location up to the maximum weight.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <param name="maxWeight">Maximum weight allowed.</param>
/// <returns>PickUpResult struct containing the picked up vehicles and vehicleTooHeavy flag.</returns>
PickUpResult pickUpVehicles(Location* location, float maxWeight);

/// <summary>
/// Checks if a location has vehicles to pick up.
/// </summary>
/// <param name="location">Pointer to the location.</param>
/// <returns>1 if the location has vehicles to pick up, 0 otherwise.</returns>
int hasVehicleToPickUp(Location* location);

/// <summary>
/// Prints the visited set.
/// </summary>
/// <param name="visitedSet">Pointer to the head of the visited set.</param>
void printVisitedSet(ListElem visitedSet);

/// <summary>
/// Prints the unvisited set.
/// </summary>
/// <param name="unvisitedSet">Pointer to the head of the unvisited set.</param>
void printUnvisitedSet(ListElem unvisitedSet);

/// <summary>
/// Prints the path.
/// </summary>
/// <param name="path">Pointer to the head of the path.</param>
void showPath(ListElem path);

/// <summary>
/// Prints the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
void showGraph(Graph* graph);

/// <summary>
/// Frees the memory allocated for a path.
/// </summary>
/// <param name="path">Pointer to the head of the path.</param>
void freePath(ListElem path);

/// <summary>
/// Frees the memory allocated for the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
void freeGraph(Graph* graph);

/// <summary>
/// Compares two adjacent locations for reordering.
/// </summary>
/// <param name="data1">Pointer to the first adjacent location.</param>
/// <param name="data2">Pointer to the second adjacent location.</param>
/// <returns>Result of the comparison.</returns>
int compareAdjacentLocations(void* data1, void* data2);

/// <summary>
/// Compares two locations for reordering.
/// </summary>
/// <param name="data1">Pointer to the first location.</param>
/// <param name="data2">Pointer to the second location.</param>
/// <returns>Result of the comparison.</returns>
int compareLocations(void* data1, void* data2);

/// <summary>
/// Removes an adjacent location from a location by index.
/// </summary>
/// <param name="selectedLocation">Pointer to the selected location.</param>
/// <param name="index">Index of the adjacent location to be removed.</param>
void removeAdjacentLocation(Location* selectedLocation, int index);
/// <summary>
/// Removes a location from the graph.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="selectedLocation">Pointer to the selected location.</param>
void removeLocation(Graph* graph, Location* selectedLocation);

/// <summary>
/// Edits removing whole locations or specific adjacencies.
/// </summary>
/// <param name="graph">Pointer to the graph.</param>
/// <param name="modified">Pointer to the modified flag.</param>
/// <returns>Pointer to the edited graph.</returns>
Graph* editGraph(Graph* graph, int* modified);