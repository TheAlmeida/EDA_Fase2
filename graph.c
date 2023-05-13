#include "graph.h"

Graph* createGraph()
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->locations = NULL;
    return graph;
}

// Function to create a new location
Location* createLocation(const char* name)
{
    Location* location = (Location*)malloc(sizeof(Location));
    strcpy(location->name, name);
    location->adjacentLocations = NULL;
    return location;
}

// Function to add a location to the graph
void addLocation(Graph* graph, Location* location)
{
    ListElem elem = createListElem(location);
    elem->next = graph->locations;
    graph->locations = elem;
}

// Function to add an adjacent location to a location
void addAdjacentLocation(Location* location, Location* adjacentLocation, int weight)
{
    AdjacentLocation* adjLocation = (AdjacentLocation*)malloc(sizeof(AdjacentLocation));
    adjLocation->location = adjacentLocation;
    //adjLocation->weight = calculateDistance(geolocationToCoordinates(location), geolocationToCoordinates(adjacentLocation));
    adjLocation->weight = weight;

    ListElem elem = createListElem(adjLocation);
    elem->next = location->adjacentLocations;
    location->adjacentLocations = elem;
}

// Function to create locations based on unique geolocations found in vehicles and add them to the graph
void createLocationsFromVehicles(Graph* graph, ListElem listV)
{
    ListElem currV = listV;
    while (currV != NULL)
    {
        Vehicle vehicle = (Vehicle)currV->data;

        // Check if the geolocation is already added as a location in the graph
        Location* existingLocation = findLocationByGeolocation(graph, vehicle->geolocation);
        if (existingLocation == NULL)
        {
            // Geolocation not found, create a new location and add it to the graph
            Location* newLocation = createLocation(vehicle->geolocation);
            addLocation(graph, newLocation);
        }

        currV = currV->next;
    }
}

// Function to find a location in the graph by geolocation
Location* findLocationByGeolocation(Graph* graph, const char* geolocation)
{
    ListElem currLocation = graph->locations;
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;
        if (strcmp(location->name, geolocation) == 0)
        {
            return location;
        }
        currLocation = currLocation->next;
    }
    return NULL;
}


// Function to print the graph
void printGraph(Graph* graph)
{
    ListElem currLocation = graph->locations;
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;
        printf("Location: %s\n", location->name);

        ListElem currAdjacentLocation = location->adjacentLocations;
        while (currAdjacentLocation != NULL)
        {
            AdjacentLocation* adjLocation = (AdjacentLocation*)currAdjacentLocation->data;
            printf("Adjacent Location: %s (Weight: %d)\n", adjLocation->location->name, adjLocation->weight);
            currAdjacentLocation = currAdjacentLocation->next;
        }

        printf("\n");
        currLocation = currLocation->next;
    }
}

// Function to free the memory allocated for the graph
void freeGraph(Graph* graph)
{
    ListElem currLocation = graph->locations;
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;

        // Free the adjacent locations
        ListElem currAdjacentLocation = location->adjacentLocations;
        while (currAdjacentLocation != NULL)
        {
            ListElem nextAdjacentLocation = currAdjacentLocation->next;
            free(currAdjacentLocation);
            currAdjacentLocation = nextAdjacentLocation;
        }

        ListElem nextLocation = currLocation->next;
        free(location);
        free(currLocation);
        currLocation = nextLocation;
    }

    free(graph);
}
