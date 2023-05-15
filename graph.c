#include "graph.h"

//Binary storeDataGraph
/*
void storeDataGraphBin(const Graph* graph)
{
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

        // Write the location name length and name
        size_t nameLength = strlen(location->name);
        fprintf(graphFile, "%zu\n", nameLength);
        fprintf(graphFile, "%s", location->name);

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

            // Write the adjacent location name length and name
            size_t adjacentNameLength = strlen(adjacentLocation->location->name);
            fprintf(graphFile, "%zu\n", adjacentNameLength);
            fprintf(graphFile, "%s", adjacentLocation->location->name);

            // Write the weight
            fprintf(graphFile, "%lf\n", adjacentLocation->weight);

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
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Failed to get current working directory.\n");
        return;
    }

    char graph_bin[500];
    strcpy(graph_bin, cwd);
    strcat(graph_bin, "\\graph.bin");

    FILE* graphFile = fopen(graph_bin, "rb");
    if (graphFile == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }

    // Read locations
    char locationName[50];
    while (fread(locationName, sizeof(char), sizeof(locationName), graphFile) == sizeof(locationName))
    {
        Location* location = createLocation(locationName);
        addLocation(graph, location);
    }

    // Read edges
    char startLocationName[50], endLocationName[50];
    double weight;
    while (fread(startLocationName, sizeof(char), sizeof(startLocationName), graphFile) == sizeof(startLocationName)
        && fread(endLocationName, sizeof(char), sizeof(endLocationName), graphFile) == sizeof(endLocationName)
        && fread(&weight, sizeof(double), 1, graphFile) == 1)
    {
        Location* startLocation = findLocationByGeolocation(graph, startLocationName);
        Location* endLocation = findLocationByGeolocation(graph, endLocationName);
        addAdjacentLocation(startLocation, endLocation, weight);
    }

    fclose(graphFile);

    return graph;
}
*/

void storeDataGraphBin(const Graph* graph)
{
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

Graph* loadDataGraphBin(Graph* graph)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Failed to get current working directory.\n");
        return NULL;
    }

    char graph_bin[500];
    strcpy(graph_bin, cwd);
    strcat(graph_bin, "\\graph.bin");

    FILE* graphFile = fopen(graph_bin, "rb");
    if (graphFile == NULL)
    {
        printf("Failed to open file for reading.\n");
        return NULL;
    }

    // Read the number of locations in the graph
    int locationCount;
    if (fscanf(graphFile, "%d", &locationCount) != 1)
    {
        printf("Failed to read location count.\n");
        fclose(graphFile);
        return NULL;
    }
    fgetc(graphFile); // Consume the newline character after the location count

    printf("Location count: %d\n", locationCount);

    // Read each location and its adjacent locations
    for (int i = 0; i < locationCount; i++)
    {
        // Read the location name
        char locationName[100];
        if (fscanf(graphFile, "%s", locationName) != 1)
        {
            printf("Failed to read location name.\n");
            fclose(graphFile);
            freeGraph(graph);
            return NULL;
        }

        printf("Location name: %s\n", locationName);

        // Create a new location and add it to the graph
        Location* location = createLocation(locationName);
        addLocation(graph, location);

        // Read the number of adjacent locations
        int adjacentCount;
        if (fscanf(graphFile, "%d", &adjacentCount) != 1)
        {
            printf("Failed to read adjacent location count.\n");
            fclose(graphFile);
            freeGraph(graph);
            return NULL;
        }
        fgetc(graphFile); // Consume the newline character after the adjacent location count

        printf("Adjacent location count: %d\n", adjacentCount);

        // Read each adjacent location and its weight
        for (int j = 0; j < adjacentCount; j++)
        {
            // Read the adjacent location name and weight
            char adjacentLocationName[100];
            double weight;
            if (fscanf(graphFile, "%s %lf", adjacentLocationName, &weight) != 2)
            {
                printf("Failed to read adjacent location and weight.\n");
                fclose(graphFile);
                freeGraph(graph);
                return NULL;
            }

            printf("Adjacent location name: %s, Weight: %.3f\n", adjacentLocationName, weight);

            // Find the adjacent location by name
            Location* adjacentLocation = findLocationByGeolocation(graph, adjacentLocationName);
            if (adjacentLocation == NULL)
            {
                printf("Error: Adjacent location not found. Location: %s, Adjacent Location: %s\n", location->name, adjacentLocationName);
                fclose(graphFile);
                freeGraph(graph);
                return NULL;
            }

            // Add the adjacent location and weight to the current location
            addAdjacentLocation(location, adjacentLocation, weight);
        }
    }

    fclose(graphFile);
    return graph;
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
