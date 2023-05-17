#include "graph.h"

// TODO: ADICIONAR INTERAÇÕES CALCULAR DISTANCIA DE CLIENTE A VEICULOS. APOS VIAGEM ATUALIZAR GRAPH. VALIDAR TODAS AS LOCALIZACOES ATRAVES DE CURL? DIJKSTRA ALGORITHM

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
        fprintf(graphFile, "%s %.6f, %.6f\n", location->name, location->coordinates.latitude, location->coordinates.longitude);

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

        // Write the client info
        ClientInfo* clientInfo = location->clientInfo;
        while (clientInfo != NULL)
        {
            fprintf(graphFile, "%s\n", clientInfo->username);
            clientInfo = clientInfo->next;
        }

        fprintf(graphFile, "-1\n");  // To mark the end of the client info list for this location

        // Write the vehicle info
        VehicleInfo* vehicleInfo = location->vehicleInfo;
        while (vehicleInfo != NULL)
        {
            fprintf(graphFile, "%d %s\n", vehicleInfo->code, vehicleInfo->type);
            vehicleInfo = vehicleInfo->next;
        }

        fprintf(graphFile, "-1\n");  // To mark the end of the vehicle info list for this location

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

    printf("Location count: %d\n", locationCount);

    // Read each location and its adjacent locations
    for (int i = 0; i < locationCount; i++)
    {
        // Read the location name
        char locationName[100];
        double latitude, longitude;
        if (fscanf(graphFile, "%s %lf, %lf", locationName, &latitude, &longitude) != 3)
        {
            printf("Failed to read location name and coordinates.\n");
            fclose(graphFile);
            freeGraph(graph);
            return NULL;
        }

        printf("Location name: %s %.6f, %.6f\n", locationName, latitude, longitude);

        // Check if the location already exists in the graph
        Location* location = findLocationByGeolocation(graph, locationName);
        if (location == NULL)
        {
            // Create a new location and add it to the graph
            location = createLocation(locationName, latitude, longitude);
            addLocation(graph, location);
        }

        // Read the number of adjacent locations
        int adjacentCount;
        if (fscanf(graphFile, "%d", &adjacentCount) != 1)
        {
            printf("Failed to read adjacent location count.\n");
            fclose(graphFile);
            freeGraph(graph);
            return NULL;
        }

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

            // Check if the adjacent location already exists in the graph
            Location* adjacentLocation = findLocationByGeolocation(graph, adjacentLocationName);
            if (adjacentLocation == NULL)
            {
                // If the adjacent location doesn't exist, create it and add it to the graph
                adjacentLocation = createLocation(adjacentLocationName, 0.0, 0.0);
                addLocation(graph, adjacentLocation);
            }

            // Add the adjacent location and weight to the current location
            addAdjacentLocation(location, adjacentLocation, weight);
        }

        while (1)
        {
            char clientUsername[50];
            if (fscanf(graphFile, "%s", clientUsername) != 1 || strcmp(clientUsername, "-1") == 0)
            {
                break;  // End of the client info list for this location
            }

            ClientInfo* clientInfo = createClientInfo(clientUsername);
            addClientInfo(location, clientInfo);
        }

        // Read the vehicle info
        while (1)
        {
            int vehicleCode;
            char vehicleType[50];
            if (fscanf(graphFile, "%d", &vehicleCode) != 1 || vehicleCode == -1)
            {
                break;  // End of the vehicle info list for this location
            }

            if (fscanf(graphFile, "%s", vehicleType) != 1)
            {
                printf("Failed to read vehicle type.\n");
                fclose(graphFile);
                freeGraph(graph);
                return NULL;
            }

            VehicleInfo* vehicleInfo = createVehicleInfo(vehicleCode, vehicleType);
            addVehicleInfo(location, vehicleInfo);
        }
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
Location* createLocation(const char* name, double latitude, double longitude)
{
    Location* location = (Location*)malloc(sizeof(Location));
    strncpy(location->name, name, sizeof(location->name) - 1);
    location->name[sizeof(location->name) - 1] = '\0';  // Ensure null-termination

    if (latitude == 0.0 && longitude == 0.0)
    {
        Coordinates coordinates = geolocationToCoordinates(name);
        location->coordinates.latitude = coordinates.latitude;
        location->coordinates.longitude = coordinates.longitude;
    }
    else
    {
        location->coordinates.latitude = latitude;
        location->coordinates.longitude = longitude;
    }
    
    location->adjacentLocations = NULL;
    location->clientInfo = NULL;
    location->vehicleInfo = NULL;
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
void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight)
{
    AdjacentLocation* adjLocation = (AdjacentLocation*)malloc(sizeof(AdjacentLocation));
    adjLocation->location = adjacentLocation;
    adjLocation->weight = weight;

    ListElem elem = createListElem(adjLocation);
    elem->next = location->adjacentLocations;
    location->adjacentLocations = elem;
}

// Function to create a new client info
ClientInfo* createClientInfo(const char* username)
{
    ClientInfo* clientInfo = (ClientInfo*)malloc(sizeof(ClientInfo));
    strncpy(clientInfo->username, username, sizeof(clientInfo->username) - 1);
    clientInfo->username[sizeof(clientInfo->username) - 1] = '\0';  // Ensure null-termination
    clientInfo->next = NULL;
    return clientInfo;
}

// Function to add a client info to a location
void addClientInfo(Location* location, ClientInfo* clientInfo)
{
    if (location->clientInfo == NULL)
    {
        location->clientInfo = clientInfo;
    }
    else
    {
        ClientInfo* currClientInfo = location->clientInfo;
        while (currClientInfo->next != NULL)
        {
            currClientInfo = currClientInfo->next;
        }
        currClientInfo->next = clientInfo;
    }
}

VehicleInfo* createVehicleInfo(int code, const char* type)
{
    VehicleInfo* vehicleInfo = (VehicleInfo*)malloc(sizeof(VehicleInfo));
    vehicleInfo->code = code;
    strncpy(vehicleInfo->type, type, sizeof(vehicleInfo->type) - 1);
    vehicleInfo->type[sizeof(vehicleInfo->type) - 1] = '\0';  // Ensure null-termination
    vehicleInfo->next = NULL;
    return vehicleInfo;
}

void addVehicleInfo(Location* location, VehicleInfo* vehicleInfo)
{
    if (location->vehicleInfo == NULL)
    {
        location->vehicleInfo = vehicleInfo;
    }
    else
    {
        VehicleInfo* currVehicleInfo = location->vehicleInfo;
        while (currVehicleInfo->next != NULL)
        {
            currVehicleInfo = currVehicleInfo->next;
        }
        currVehicleInfo->next = vehicleInfo;
    }
}

void createLocationsFromClients(Graph* graph, ListElem listC)
{
    ListElem currC = listC;
    while (currC != NULL)
    {
        Client client = (Client)currC->data;

        // Check if the geolocation is already added as a location in the graph
        Location* existingLocation = findLocationByGeolocation(graph, client->geolocation);
        if (existingLocation == NULL)
        {
            // Geolocation not found, create a new location and add it to the graph
            Location* newLocation = createLocation(client->geolocation, 0.0, 0.0);
            addLocation(graph, newLocation);
            existingLocation = newLocation; // Set the existingLocation to the newly created location
        }

        // Register the client id and name in the location's clientInfo list
        ClientInfo* newClientInfo = createClientInfo(client->username);
        addClientInfo(existingLocation, newClientInfo);

        currC = currC->next;
    }
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
            Location* newLocation = createLocation(vehicle->geolocation, 0.0, 0.0);
            addLocation(graph, newLocation);
            existingLocation = newLocation; // Set the existingLocation to the newly created location
        }

        // Register the vehicle type and code in the location's vehicleInfo list
        VehicleInfo* newVehicleInfo = createVehicleInfo(vehicle->code, vehicle->type);
        addVehicleInfo(existingLocation, newVehicleInfo);

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


/*
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

            double weight = calculateDistance(location->coordinates.latitude, location->coordinates.longitude, adjacentLocation->coordinates.latitude, adjacentLocation->coordinates.longitude);

            addAdjacentLocation(location, adjacentLocation, weight);
        }

        currLocation = currLocation->next;
    }
}
*/


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

int locationHasAdjacency(Location* location, Location* otherLocation)
{
    ListElem currAdjacency = location->adjacentLocations;
    while (currAdjacency != NULL)
    {
        AdjacentLocation* adjLoc = (AdjacentLocation*)currAdjacency->data;
        if (adjLoc->location == otherLocation)
            return 1;

        currAdjacency = currAdjacency->next;
    }

    return 0;
}

void connectAdjacentLocations(Graph* graph)
{
    ListElem currLocation = graph->locations;

    // Ensure every location has at least one outgoing connection
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;
        Location* adjacentLocation;

        // Ensure the location has at least one outgoing connection and is not the same location
        do
        {
            adjacentLocation = getRandomLocation(graph, location);
        } while (locationHasAdjacency(location, adjacentLocation) || adjacentLocation == location);

        double weight = calculateDistance(location->coordinates.latitude,
            location->coordinates.longitude,
            adjacentLocation->coordinates.latitude,
            adjacentLocation->coordinates.longitude);

        addAdjacentLocation(location, adjacentLocation, weight);

        currLocation = currLocation->next;
    }

    // Second pass to ensure that every location has at least one incoming connection
    currLocation = graph->locations;
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;

        // Check if location has incoming connections
        int hasIncoming = 0;
        ListElem otherLocation = graph->locations;
        while (otherLocation != NULL)
        {
            Location* other = (Location*)otherLocation->data;
            if (locationHasAdjacency(other, location))
            {
                hasIncoming = 1;
                break;
            }
            otherLocation = otherLocation->next;
        }

        // If location doesn't have incoming connections, add one from a random location
        if (!hasIncoming)
        {
            Location* incomingLocation;
            do
            {
                incomingLocation = getRandomLocation(graph, location);
            } while (locationHasAdjacency(incomingLocation, location) || incomingLocation == location);

            double weight = calculateDistance(location->coordinates.latitude,
                location->coordinates.longitude,
                incomingLocation->coordinates.latitude,
                incomingLocation->coordinates.longitude);

            addAdjacentLocation(incomingLocation, location, weight);
        }

        currLocation = currLocation->next;
    }
}


void printGraph(Graph* graph)
{
    printf("\n\n");
    ListElem currLocation = graph->locations;
    while (currLocation != NULL)
    {
        Location* location = (Location*)currLocation->data;
        printf("Location: %s Coordinates: %.6f, %.6f (lat, lon)\n", location->name, location->coordinates.latitude, location->coordinates.longitude);

        ListElem currAdjacentLocation = location->adjacentLocations;
        while (currAdjacentLocation != NULL)
        {
            AdjacentLocation* adjLocation = (AdjacentLocation*)currAdjacentLocation->data;

            // Find the adjacent location in the graph's location list
            Location* adjacentLocation = findLocationByGeolocation(graph, adjLocation->location->name);
            if (adjacentLocation != NULL)
            {
                printf("Adjacent Location: %s (Weight: %.3f)\n", adjacentLocation->name, adjLocation->weight);
            }
            else
            {
                printf("Error: Adjacent location not found.\n");
            }

            currAdjacentLocation = currAdjacentLocation->next;
        }

        // Print the client info
        ClientInfo* currClientInfo = location->clientInfo;
        while (currClientInfo != NULL)
        {
            printf("Client Username: %s\n", currClientInfo->username);
            currClientInfo = currClientInfo->next;
        }

        // Print the vehicle info
        VehicleInfo* currVehicleInfo = location->vehicleInfo;
        while (currVehicleInfo != NULL)
        {
            printf("Vehicle Type_Code: %s_%d\n", currVehicleInfo->type, currVehicleInfo->code);
            currVehicleInfo = currVehicleInfo->next;
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

        // Free the vehicle info
        ClientInfo* currClientInfo = location->clientInfo;
        while (currClientInfo != NULL)
        {
            ClientInfo* nextClientInfo = currClientInfo->next;
            free(currClientInfo);
            currClientInfo = nextClientInfo;
        }

        // Free the vehicle info
        VehicleInfo* currVehicleInfo = location->vehicleInfo;
        while (currVehicleInfo != NULL)
        {
            VehicleInfo* nextVehicleInfo = currVehicleInfo->next;
            free(currVehicleInfo);
            currVehicleInfo = nextVehicleInfo;
        }

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
