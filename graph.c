#include "graph.h"

//Binary storeDataGraph
/*
void storeDataGraph(const Graph* graph) {
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Failed to get current working directory.\n");
        return;
    }

    char graph_bin[500];
    strcpy(graph_bin, cwd);
    strcat(graph_bin, "\\graph.bin");

    FILE* graphFile = fopen(graph_bin, "wb");
    if (graphFile == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }

    // Write the number of locations in the graph
    int locationCount = 0;
    ListElem locationElem = graph->locations;
    while (locationElem != NULL) {
        locationCount++;
        locationElem = locationElem->next;
    }
    fwrite(&locationCount, sizeof(int), 1, graphFile);

    // Write each location and its adjacent locations
    locationElem = graph->locations;
    while (locationElem != NULL) {
        Location* location = (Location*)locationElem->data;

        // Write the location name
        int nameLength = strlen(location->name);
        fwrite(&nameLength, sizeof(int), 1, graphFile);
        fwrite(location->name, sizeof(char), nameLength, graphFile);

        // Write the number of adjacent locations
        int adjacentCount = 0;
        ListElem adjacentElem = location->adjacentLocations;
        while (adjacentElem != NULL) {
            adjacentCount++;
            adjacentElem = adjacentElem->next;
        }
        fwrite(&adjacentCount, sizeof(int), 1, graphFile);

        // Write each adjacent location and its weight
        adjacentElem = location->adjacentLocations;
        while (adjacentElem != NULL) {
            AdjacentLocation* adjacentLocation = (AdjacentLocation*)adjacentElem->data;

            // Write the adjacent location name and weight
            int adjacentNameLength = strlen(adjacentLocation->location->name);
            fwrite(&adjacentNameLength, sizeof(int), 1, graphFile);
            fwrite(adjacentLocation->location->name, sizeof(char), adjacentNameLength, graphFile);
            fwrite(&adjacentLocation->weight, sizeof(double), 1, graphFile);

            adjacentElem = adjacentElem->next;
        }

        locationElem = locationElem->next;
    }

    fclose(graphFile);
}
*/


//Binary loadDataGraph
/*
Graph* loadDataGraph(Graph* graph)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        error();

    char graph_bin[500];
    strcpy(graph_bin, cwd);
    strcat(graph_bin, "\\graph.bin");

    FILE* graphFile = fopen(graph_bin, "rb");
    if (graphFile == NULL) {
        printf("Failed to open file for reading.\n");
        return NULL;
    }

    // Read the number of locations in the graph
    int locationCount;
    fread(&locationCount, sizeof(int), 1, graphFile);

    // Read each location and its adjacent locations
    for (int i = 0; i < locationCount; i++) {
        // Read the location name
        char locationName[50];
        fread(locationName, sizeof(char), sizeof(locationName), graphFile);

        // Create a new location
        Location* location = createLocation(locationName);
        addLocation(graph, location);

        // Read the number of adjacent locations
        int adjacentCount;
        fread(&adjacentCount, sizeof(int), 1, graphFile);

        // Read each adjacent location and its weight
        for (int j = 0; j < adjacentCount; j++) {
            // Read the adjacent location name
            char adjacentName[50];
            fread(adjacentName, sizeof(char), sizeof(adjacentName), graphFile);

            // Read the weight
            double weight;
            fread(&weight, sizeof(double), 1, graphFile);

            // Find the adjacent location in the graph
            Location* adjacentLocation = findLocationByGeolocation(graph, adjacentName);

            // Create the adjacent location entry
            addAdjacentLocation(location, adjacentLocation, weight);
        }
    }

    fclose(graphFile);
    return graph;
}
*/

void storeDataGraph(const Graph* graph) {
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Failed to get current working directory.\n");
        return;
    }

    char graph_bin[500];
    strcpy(graph_bin, cwd);
    strcat(graph_bin, "\\graph.bin");

    FILE* graphFile = fopen(graph_bin, "wb");
    if (graphFile == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }

    // Write the number of locations in the graph
    int locationCount = 0;
    ListElem locationElem = graph->locations;
    while (locationElem != NULL) {
        locationCount++;
        locationElem = locationElem->next;
    }
    fwrite(&locationCount, sizeof(int), 1, graphFile);

    // Write each location and its adjacent locations
    locationElem = graph->locations;
    while (locationElem != NULL) {
        Location* location = (Location*)locationElem->data;

        // Write the location name
        fwrite(location->name, sizeof(location->name), 1, graphFile);

        // Write the number of adjacent locations
        int adjacentCount = 0;
        ListElem adjacentElem = location->adjacentLocations;
        while (adjacentElem != NULL) {
            adjacentCount++;
            adjacentElem = adjacentElem->next;
        }
        fwrite(&adjacentCount, sizeof(int), 1, graphFile);

        // Write each adjacent location and its weight
        adjacentElem = location->adjacentLocations;
        while (adjacentElem != NULL) {
            AdjacentLocation* adjacentLocation = (AdjacentLocation*)adjacentElem->data;

            // Write the adjacent location name and weight
            fwrite(adjacentLocation->location->name, sizeof(adjacentLocation->location->name), 1, graphFile);
            fwrite(&adjacentLocation->weight, sizeof(double), 1, graphFile);

            adjacentElem = adjacentElem->next;
        }

        locationElem = locationElem->next;
    }

    fclose(graphFile);
}


void storeDataGraphTxt(const Graph* graph)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Failed to get current working directory.\n");
        return;
    }

    char graph_txt[500];
    strcpy(graph_txt, cwd);
    strcat(graph_txt, "\\graph.txt");

    FILE* graphFile = fopen(graph_txt, "w");
    if (graphFile == NULL)
    {
        printf("Failed to open file for writing.\n");
        return;
    }

    // Write the number of locations in the graph
    int locationCount = 0;
    ListElem locationElem = graph->locations;
    while (locationElem != NULL)
    {
        locationCount++;
        locationElem = locationElem->next;
    }
    fprintf(graphFile, "%d\n", locationCount);

    // Write each location and its adjacent locations
    locationElem = graph->locations;
    while (locationElem != NULL)
    {
        Location* location = (Location*)locationElem->data;

        // Write the location name
        fprintf(graphFile, "%s\n", location->name);

        // Write the number of adjacent locations
        int adjacentCount = 0;
        ListElem adjacentElem = location->adjacentLocations;
        while (adjacentElem != NULL)
        {
            adjacentCount++;
            adjacentElem = adjacentElem->next;
        }
        fprintf(graphFile, "%d\n", adjacentCount);

        // Write each adjacent location and its weight
        adjacentElem = location->adjacentLocations;
        while (adjacentElem != NULL)
        {
            AdjacentLocation* adjacentLocation = (AdjacentLocation*)adjacentElem->data;

            // Write the adjacent location name and weight
            fprintf(graphFile, "%s %.3f\n", adjacentLocation->location->name, adjacentLocation->weight);

            adjacentElem = adjacentElem->next;
        }

        locationElem = locationElem->next;
    }

    fclose(graphFile);
}

Graph* loadDataGraphTxt(Graph* graph) {
    
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("Failed to get current working directory.\n");
        return;
    }

    char graph_txt[500];
    strcpy(graph_txt, cwd);
    strcat(graph_txt, "\\graph.txt");

    FILE* graphFile = fopen(graph_txt, "r");
    if (graphFile == NULL)
    {
        printf("Failed to open file for reading.\n");
        return;
    }

    // Read the number of locations in the graph
    int locationCount;
    fscanf(graphFile, "%d", &locationCount);

    // Read the locations and their adjacent locations
    for (int i = 0; i < locationCount; i++) {
        // Read the location name
        char locationName[50];
        fscanf(graphFile, "%s", locationName);

        // Create the location
        Location* location = createLocation(locationName);

        // Read the number of adjacent locations
        int adjacentCount;
        fscanf(graphFile, "%d", &adjacentCount);

        // Read the adjacent locations and their weights
        for (int j = 0; j < adjacentCount; j++) {
            char adjacentName[50];
            double weight;
            fscanf(graphFile, "%s %lf", adjacentName, &weight);

            // Create the adjacent location and add it to the current location
            Location* adjacentLocation = createLocation(adjacentName);
            addAdjacentLocation(location, adjacentLocation, weight);
        }

        // Add the location to the graph
        addLocation(graph, location);
    }

    fclose(graphFile);
    return graph;
}

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
    strncpy(location->name, name, sizeof(location->name) - 1);
    location->name[sizeof(location->name) - 1] = '\0';  // Ensure null-termination
    location->adjacentLocations = NULL;
    return location;
}

/*
Location* createLocation(const char* name)
{
    Location* location = (Location*)malloc(sizeof(Location));
    strcpy(location->name, name);
    location->adjacentLocations = NULL;
    return location;
}
*/

// Function to add a location to the graph
void addLocation(Graph* graph, Location* location)
{
    ListElem elem = createListElem(location);
    elem->next = graph->locations;
    graph->locations = elem;
}

// Function to add an adjacent location to a location
void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight)
{
    AdjacentLocation* adjLocation = (AdjacentLocation*)malloc(sizeof(AdjacentLocation));
    adjLocation->location = adjacentLocation;
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

void connectAdjacentLocations(Graph* graph)
{
    ListElem currLocation = graph->locations;
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;

        // Get the adjacent locations count (1 or 2)
        int adjacentCount = (rand() % 2) + 1;

        for (int i = 0; i < adjacentCount; i++)
        {
            // Select a random adjacent location
            Location* adjacentLocation = getRandomLocation(graph, location);

            // Calculate the weight using geolocation and distance
            Coordinates locationCoords = geolocationToCoordinates(location->name);
            Coordinates adjacentCoords = geolocationToCoordinates(adjacentLocation->name);
            double weight = calculateDistance(locationCoords.latitude, locationCoords.longitude, adjacentCoords.latitude, adjacentCoords.longitude);            

            addAdjacentLocation(location, adjacentLocation, weight);
        }

        currLocation = currLocation->next;
    }
}

Location* getRandomLocation(Graph* graph, Location* excludeLocation)
{
    ListElem currLocation = graph->locations;
    int numLocations = 0;

    // Count the number of available locations (excluding the excludeLocation)
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;
        if (location != excludeLocation)
            numLocations++;
        currLocation = currLocation->next;
    }

    // Select a random index within the available locations range
    int randomIndex = rand() % numLocations;

    // Get the location at the random index
    currLocation = graph->locations;
    int currentIndex = 0;

    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;

        if (location != excludeLocation)
        {
            if (currentIndex == randomIndex)
                return location;

            currentIndex++;
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
            printf("Adjacent Location: %s (Weight: %.3f)\n", adjLocation->location->name, adjLocation->weight);
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
