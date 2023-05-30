#include "graph.h"

const char garage[] = "chart.maximum.ahead";

void storeDataGraph(const Graph* graph)
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

		fprintf(graphFile, "%s %.6f, %.6f\n", location->name, location->coordinates.latitude, location->coordinates.longitude);

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

			fprintf(graphFile, "%s %.3f\n", adjacentLocation->location->name, adjacentLocation->weight);

			adjacentElem = adjacentElem->next;
		}

		// Write the client info
		ListElem currentC = location->clientList;
		while (currentC != NULL) {
			Client client = (Client)currentC->data;
			fprintf(graphFile, "%s\n", client->username);
			currentC = currentC->next;
		}


		fprintf(graphFile, "-1\n");

		// Write the vehicle info
		ListElem currentV = location->vehicleList;
		while (currentV != NULL) {
			Vehicle vehicle = (Vehicle)currentV->data;
			fprintf(graphFile, "%d %s\n", vehicle->code, vehicle->type);
			currentV = currentV->next;
		}

		fprintf(graphFile, "-1\n");

		locationElem = locationElem->next;
	}

	fclose(graphFile);
}

Graph* loadDataGraph(Graph* graph, ListElem listClients, ListElem listVehicles)
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

		// Read the client info
		Client c = NULL;
		while (1) {
			c = (Client)malloc(sizeof(struct dataclient));
			char clientUsername[50];
			if (fscanf(graphFile, "%s", clientUsername) != 1 || strcmp(clientUsername, "-1") == 0) {
				free(c);
				break;
			}
			c = getClientByUsername(listClients, clientUsername);
			location->clientList = addItemLastIterative(location->clientList, c);
		}

		// Read the vehicle info
		Vehicle v = NULL;
		while (1)
		{
			v = (Vehicle)malloc(sizeof(struct datavehicle));
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

			v = getVehicleByTypeAndCode(listVehicles, vehicleType, vehicleCode);
			location->vehicleList = addItemLastIterative(location->vehicleList, v);

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
	location->clientList = NULL;
	location->vehicleList = NULL;
	return location;
}

void addLocation(Graph* graph, Location* location)
{
	ListElem elem = createListElem(location);
	elem->next = graph->locations;
	graph->locations = elem;
}

void addAdjacentLocation(Location* location, Location* adjacentLocation, double weight)
{
	AdjacentLocation* adjLocation = (AdjacentLocation*)malloc(sizeof(AdjacentLocation));
	adjLocation->location = adjacentLocation;
	adjLocation->weight = weight;

	ListElem elem = createListElem(adjLocation);
	elem->next = location->adjacentLocations;
	location->adjacentLocations = elem;
}

void addClientToLocation(Location* location, Client client)
{
	location->clientList = addItemLastIterative(location->clientList, client);
}

void removeClientFromLocation(Location* location, const char* username)
{
	ListElem currClient = location->clientList;
	ListElem prevClient = NULL;

	while (currClient != NULL)
	{
		Client client = (Client)currClient->data;

		if (strcmp(client->username, username) == 0)
		{
			if (prevClient == NULL)
			{
				// Client is the first element in the list
				location->clientList = currClient->next;
			}
			else
			{
				prevClient->next = currClient->next;
			}
			free(currClient);
			break;
		}

		prevClient = currClient;
		currClient = currClient->next;
	}
}

void addVehicleToLocation(Location* location, Vehicle vehicle)
{
	location->vehicleList = addItemLastIterative(location->vehicleList, vehicle);
}

void removeVehicleFromLocation(Location* location, int code, const char* type)
{
	ListElem currVehicle = location->vehicleList;
	ListElem prevVehicle = NULL;

	while (currVehicle != NULL)
	{
		Vehicle vehicle = (Vehicle)currVehicle->data;

		if (vehicle->code == code && strcmp(vehicle->type, type) == 0)
		{
			if (prevVehicle == NULL)
			{
				// Vehicle is the first element in the list
				location->vehicleList = currVehicle->next;
			}
			else
			{
				prevVehicle->next = currVehicle->next;
			}
			free(currVehicle);
			break;
		}

		prevVehicle = currVehicle;
		currVehicle = currVehicle->next;
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

		addClientToLocation(existingLocation, client);

		currC = currC->next;
	}
}

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

		addVehicleToLocation(existingLocation, vehicle);

		currV = currV->next;
	}
}

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

Location* findLocationByVehicle(Graph* graph, Vehicle vehicle)
{
	ListElem currLocationElem = graph->locations;
	while (currLocationElem != NULL) {
		Location* location = (Location*)currLocationElem->data;
		ListElem vehicleElem = location->vehicleList;
		while (vehicleElem != NULL) {
			Vehicle currVehicle = (Vehicle)vehicleElem->data;
			if (currVehicle == vehicle) {
				return location;
			}
			vehicleElem = vehicleElem->next;
		}
		currLocationElem = currLocationElem->next;
	}
	return NULL;
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

int hasIncomingAdjacency(Graph* graph, Location* location)
{
	ListElem currLocation = graph->locations;

	while (currLocation != NULL)
	{
		Location* otherLocation = (Location*)currLocation->data;

		if (otherLocation != location && locationHasAdjacency(otherLocation, location))
		{
			return 1; // Found an incoming adjacency
		}

		currLocation = currLocation->next;
	}

	return 0; // No incoming adjacency found
}

void maintainConnectivity(Graph* graph)
{
	ListElem currLocation = graph->locations;

	while (currLocation != NULL)
	{
		Location* location = (Location*)currLocation->data;

		if (location->adjacentLocations == NULL)
		{
			Location* outgoingAdjacentLocation = getRandomLocation(graph, location);

			if (outgoingAdjacentLocation != NULL)
			{
				double outgoingWeight = calculateDistance(location->coordinates.latitude,
					location->coordinates.longitude,
					outgoingAdjacentLocation->coordinates.latitude,
					outgoingAdjacentLocation->coordinates.longitude);

				addAdjacentLocation(location, outgoingAdjacentLocation, outgoingWeight);
			}
		}

		if (!hasIncomingAdjacency(graph, location))
		{
			Location* incomingAdjacentLocation = getRandomLocation(graph, location);

			if (incomingAdjacentLocation != NULL)
			{
				double incomingWeight = calculateDistance(incomingAdjacentLocation->coordinates.latitude,
					incomingAdjacentLocation->coordinates.longitude,
					location->coordinates.latitude,
					location->coordinates.longitude);

				addAdjacentLocation(incomingAdjacentLocation, location, incomingWeight);
			}
		}

		currLocation = currLocation->next;
	}
}

void pseudoStronglyConnectLocations(Graph* graph)
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
		addAdjacentLocation(adjacentLocation, location, weight); // Add incoming connection

		currLocation = currLocation->next;
	}
}


Location* findMinDistanceLocation(ListElem unvisitedSet)
{
	Location* minLocation = NULL;
	double minDistance = DBL_MAX;

	ListElem curr = unvisitedSet;
	while (curr != NULL) {
		VisitedLocation* visitedLocation = (VisitedLocation*)curr->data;
		if (visitedLocation->distance < minDistance) {
			minDistance = visitedLocation->distance;
			minLocation = visitedLocation->location;
		}
		curr = curr->next;
	}

	return minLocation;
}

ListElem removeVisitedLocationByLocation(ListElem head, Location* location)
{
	ListElem current = head;
	ListElem prev = NULL;
	while (current != NULL) {
		VisitedLocation* visitedLocation = (VisitedLocation*)current->data;
		if (visitedLocation->location == location) {
			if (prev == NULL) {
				ListElem next = current->next;
				free(current);
				return next;
			}
			else {
				prev->next = current->next;
				free(current);
				return head;
			}
		}
		prev = current;
		current = current->next;
	}
	return head;
}

VisitedLocation* createVisitedLocation(Location* location, double distance, VisitedLocation* previous)
{
	VisitedLocation* visitedLocation = (VisitedLocation*)malloc(sizeof(VisitedLocation));
	visitedLocation->location = location;
	visitedLocation->distance = distance;
	visitedLocation->previous = previous;
	return visitedLocation;
}

void updateVisitedLocationDistance(VisitedLocation* visitedLocation, double distance)
{
	visitedLocation->distance = distance;
}

void updateVisitedLocationPrevious(VisitedLocation* visitedLocation, VisitedLocation* previous)
{
	visitedLocation->previous = previous;
}

VisitedLocation* findVisitedLocationByLocation(ListElem visitedSet, Location* location)
{
	ListElem curr = visitedSet;
	while (curr != NULL) {
		VisitedLocation* visitedLocation = (VisitedLocation*)curr->data;
		if (visitedLocation->location == location) {
			return visitedLocation;
		}
		curr = curr->next;
	}

	// Didn't find a VisitedLocation for this Location
	return NULL;
}

AdjacentLocation* findAdjacentLocation(Location* from, Location* to)
{
	ListElem currElem = from->adjacentLocations;
	while (currElem != NULL) {
		AdjacentLocation* adjLocation = (AdjacentLocation*)currElem->data;
		if (adjLocation->location == to) {
			return adjLocation;
		}
		currElem = currElem->next;
	}
	return NULL;  // Return NULL if no AdjacentLocation is found
}

ListElem getShortestPath(VisitedLocation* target)
{
	ListElem path = NULL;

	VisitedLocation* curr = target;
	while (curr != NULL) {
		path = addItemHead(path, curr->location);
		curr = curr->previous;
	}

	return path;
}

double calculatePathDistance(ListElem path)
{
	double distance = 0.0;
	ListElem currElem = path;
	while (currElem != NULL && currElem->next != NULL) {
		Location* currentLocation = (Location*)currElem->data;
		Location* nextLocation = (Location*)currElem->next->data;
		AdjacentLocation* adjLocation = findAdjacentLocation(currentLocation, nextLocation);
		if (adjLocation != NULL) {
			distance += adjLocation->weight;
		}
		currElem = currElem->next;
	}
	return distance;
}

ListElem calculateShortestPath(Graph* graph, Location* start, Location* target)
{
	ListElem unvisitedSet = NULL;
	ListElem visitedSet = NULL;
	double newDistance = 0.0;
	VisitedLocation* currentVisitedLocation = NULL;

	// Initialize all locations with initial distance infinity
	ListElem currLocationElem = graph->locations;
	while (currLocationElem != NULL) {
		Location* location = (Location*)currLocationElem->data;
		double distance = (location == start) ? 0.0 : DBL_MAX;
		VisitedLocation* visitedLocation = createVisitedLocation(location, distance, NULL);
		unvisitedSet = addItemHead(unvisitedSet, visitedLocation);
		currLocationElem = currLocationElem->next;
	}

	printf("Start Loc: %s\n", start->name);
	printf("Target Loc: %s\n", target->name);
	printf("\n");

	while (unvisitedSet != NULL) {
		Location* currentLocation = findMinDistanceLocation(unvisitedSet);

		if (currentLocation == NULL) {
			printf("Current location is NULL\n");
			break;
		}

		printf("Current Loc: %s\n", currentLocation->name);

		// Set currentVisitedLocation before traversing neighbors
		currentVisitedLocation = findVisitedLocationByLocation(unvisitedSet, currentLocation);
		if (currentVisitedLocation == NULL) {
			printf("Error: Current visited location is NULL\n");
			return NULL;
		}

		// Traverse the adjacent locations of the current location
		ListElem adjacentElem = currentLocation->adjacentLocations;
		while (adjacentElem != NULL) {
			AdjacentLocation* adjLocation = (AdjacentLocation*)adjacentElem->data;
			Location* adjacentLocation = adjLocation->location;
			double weight = adjLocation->weight;

			if (adjacentLocation != NULL) {
				VisitedLocation* visitedLocation = findVisitedLocationByLocation(unvisitedSet, adjacentLocation);

				if (visitedLocation != NULL) {
					newDistance = currentVisitedLocation->distance + weight;

					if (newDistance < visitedLocation->distance) {
						visitedLocation->distance = newDistance;
						visitedLocation->previous = currentVisitedLocation;
					}
				}
			}

			adjacentElem = adjacentElem->next;
		}

		// After all neighbours have been visited, move current location to visited set
		unvisitedSet = removeVisitedLocationByLocation(unvisitedSet, currentLocation);
		visitedSet = addItemHead(visitedSet, currentVisitedLocation);

		if (currentLocation == target) {
			// Building shortest path with Location pointers
			ListElem path = getShortestPath(currentVisitedLocation);
			printf("Shortest Path:\n");
			showPath(path);

			// Print the distance
			printf("Distance: %.3f\n", currentVisitedLocation->distance);

			return path;
		}
	}

	return NULL;
}

Location* calculateShortestPathWithVehicle(Graph* graph, Location* start)
{
	Location* closestLocation = NULL;
	ListElem path = NULL;
	double minDistance = DBL_MAX;

	ListElem locationElem = graph->locations;
	while (locationElem != NULL) {
		Location* targetLocation = (Location*)locationElem->data;

		// Ignore the start location and locations without vehicles to pick up
		if (targetLocation != start && hasVehicleToPickUp(targetLocation))
		{
			ListElem newPath = calculateShortestPath(graph, start, targetLocation);

			// Get the distance of the path
			VisitedLocation* visitedLocation = (VisitedLocation*)newPath->data;
			double distance = visitedLocation->distance;

			if (distance < minDistance)
			{
				if (path != NULL)
				{
					freePath(path);
				}
				minDistance = distance;
				path = newPath;
				closestLocation = targetLocation;
			}
			else
			{
				freePath(newPath);
			}
		}

		locationElem = locationElem->next;
	}

	return closestLocation;
}

void unloadVehicles(Location* start, ListElem pickedUpVehicles)
{
	ListElem vehicleElem = pickedUpVehicles;
	while (vehicleElem != NULL) {
		Vehicle vehicle = (Vehicle*)vehicleElem->data;
		strcpy(vehicle->geolocation, start->name);
		refillBatteryAndAutonomy(vehicle);
		start->vehicleList = addItemHead(start->vehicleList, vehicle);
		vehicleElem = vehicleElem->next;
	}
}

PickUpResult pickUpVehicles(Location* location, float maxWeight)
{
	ListElem previous = NULL;
	ListElem current = location->vehicleList;
	ListElem pickedUpVehicles = NULL;
	int vehicleTooHeavy = 0;

	while (current != NULL && maxWeight > 0) {
		Vehicle vehicle = (Vehicle*)current->data;
		if (vehicle->battery < 50) {
			if (vehicle->weight <= maxWeight) {
				// remove vehicle from location's vehicle list
				if (previous == NULL) {
					location->vehicleList = current->next;
				}
				else {
					previous->next = current->next;
				}
				// add vehicle to picked up vehicles
				current->next = NULL;
				pickedUpVehicles = appendList(pickedUpVehicles, current);
				maxWeight -= vehicle->weight;
				current = previous ? previous->next : location->vehicleList;
			}
			else {
				vehicleTooHeavy = 1;
				break;
			}
		}
		else {
			previous = current;
			current = current->next;
		}
	}

	PickUpResult result = { pickedUpVehicles, vehicleTooHeavy };
	return result;
}

int hasVehicleToPickUp(Location* location)
{
	ListElem vehicleElem = location->vehicleList;
	while (vehicleElem != NULL) {
		Vehicle vehicle = (Vehicle)vehicleElem->data;
		if (vehicle->battery < 50.0) {
			return 1;
		}
		vehicleElem = vehicleElem->next;
	}
	return 0;
}

void printVisitedSet(ListElem visitedSet)
{
	printf("Visited Set: ");
	ListElem current = visitedSet;
	while (current != NULL) {
		VisitedLocation* visitedLocation = (VisitedLocation*)current->data;
		printf("%s ", visitedLocation->location->name);
		current = current->next;
	}
	printf("\n");
}

void printUnvisitedSet(ListElem unvisitedSet)
{
	printf("Unvisited Set: ");
	ListElem current = unvisitedSet;
	while (current != NULL) {
		VisitedLocation* visitedLocation = (VisitedLocation*)current->data;
		printf("%s ", visitedLocation->location->name);
		current = current->next;
	}
	printf("\n");
}

void showPath(ListElem path)
{
	double cumulativeDistance = 0.0;
	ListElem currElem = path;
	while (currElem != NULL && currElem->next != NULL) {
		Location* currentLocation = (Location*)currElem->data;
		Location* nextLocation = (Location*)currElem->next->data;
		AdjacentLocation* adjLocation = findAdjacentLocation(currentLocation, nextLocation);
		if (adjLocation != NULL) {
			double distance = adjLocation->weight;
			cumulativeDistance += distance;
			printf("%s -> %s (Distance: %.3f, Cumulative Distance: %.3f)\n", currentLocation->name, nextLocation->name, distance, cumulativeDistance);
		}
		currElem = currElem->next;
	}
}

void showGraph(Graph* graph)
{
	int length = 0;
	printf("\n\n");
	ListElem currLocation = graph->locations;
	while (currLocation != NULL)
	{
		length++;
		printf(" %d.\n", length);
		Location* location = (Location*)currLocation->data;
		printf(" Location: %s Coordinates: %.6f, %.6f (lat, lon)\n", location->name, location->coordinates.latitude, location->coordinates.longitude);

		ListElem currAdjacentLocation = location->adjacentLocations;
		while (currAdjacentLocation != NULL)
		{
			AdjacentLocation* adjLocation = (AdjacentLocation*)currAdjacentLocation->data;

			// Find the adjacent location in the graph's location list
			Location* adjacentLocation = findLocationByGeolocation(graph, adjLocation->location->name);
			if (adjacentLocation != NULL)
			{
				printf("  Adjacent Location: %s (Weight: %.3f)\n", adjacentLocation->name, adjLocation->weight);
			}
			else
			{
				printf(" Error: Adjacent location not found.\n");
			}

			currAdjacentLocation = currAdjacentLocation->next;
		}

		// Print the client info
		ListElem currClient = location->clientList;
		while (currClient != NULL)
		{
			Client client = (Client)currClient->data;
			printf("  Client Username: %s\n", client->username);
			currClient = currClient->next;
		}

		// Print the vehicle info
		ListElem currVehicle = location->vehicleList;
		while (currVehicle != NULL)
		{
			Vehicle vehicle = (Vehicle)currVehicle->data;
			printf("  Vehicle Type_Code: %s_%d\n", vehicle->type, vehicle->code);
			currVehicle = currVehicle->next;
		}

		printf("\n");
		currLocation = currLocation->next;
	}
}

void freePath(ListElem path)
{
	ListElem currentElem = path;
	while (currentElem != NULL) {
		ListElem nextElem = currentElem->next;
		free(currentElem);
		currentElem = nextElem;
	}
}

void freeGraph(Graph* graph)
{
	ListElem currLocation = graph->locations;
	while (currLocation != NULL)
	{
		Location* location = (Location*)currLocation->data;

		// Free the client info
		ListElem currClient = location->clientList;
		while (currClient != NULL)
		{
			ListElem nextClient = currClient->next;
			free(currClient);
			currClient = nextClient;
		}

		// Free the vehicle info
		ListElem currVehicle = location->vehicleList;
		while (currVehicle != NULL)
		{
			ListElem nextVehicle = currVehicle->next;
			free(currVehicle);
			currVehicle = nextVehicle;
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

int compareAdjacentLocations(void* data1, void* data2)
{
	AdjacentLocation* adjLocation1 = (AdjacentLocation*)data1;
	Location* location2 = (Location*)data2;

	if (strcmp(adjLocation1->location->name, location2->name) == 0)
		return 1;
	else
		return 0;
}

int compareLocations(void* data1, void* data2)
{
	Location* location1 = (Location*)data1;
	Location* location2 = (Location*)data2;

	if (strcmp(location1->name, location2->name) == 0)
		return 1;
	else
		return 0;
}

void removeAdjacentLocation(Location* selectedLocation, int index)
{
	selectedLocation->adjacentLocations = removeElementByIndex(selectedLocation->adjacentLocations, index);
}

void removeLocation(Graph* graph, Location* selectedLocation)
{
	// Iteratively remove the selectedLocation from graph's locations
	graph->locations = removeItemIterative(graph->locations, selectedLocation, compareLocations);

	// Traverse all locations in the graph and remove any adjacency related to the removed location
	ListElem temp = graph->locations;
	while (temp != NULL) {
		Location* location = (Location*)temp->data;
		location->adjacentLocations = removeItemIterative(location->adjacentLocations, selectedLocation, compareAdjacentLocations);
		temp = temp->next;
	}

	// Find drop location (garage)
	Location* dropLocation = findLocationByGeolocation(graph, garage);
	if (!dropLocation) {
		printf("Garagem nao encontrada.\n");
		return;
	}

	// Handle vehicles
	ListElem vehicleTemp = selectedLocation->vehicleList;
	if (selectedLocation->vehicleList != NULL)
		unloadVehicles(dropLocation, selectedLocation->vehicleList);
	selectedLocation->vehicleList = NULL;
	while (vehicleTemp != NULL)
	{
		Vehicle vehicle = (Vehicle)vehicleTemp->data;
		removeVehicleFromLocation(selectedLocation, vehicle->code, vehicle->type);
		vehicleTemp = vehicleTemp->next;
	}

	// Handle clients
	ListElem clientTemp = selectedLocation->clientList;
	selectedLocation->clientList = NULL;
	while (clientTemp != NULL)
	{
		Client client = (Client)clientTemp->data;
		strcpy(client->geolocation, garage);
		removeClientFromLocation(selectedLocation, client->username);
		addClientToLocation(dropLocation, client);
		clientTemp = clientTemp->next;
	}
}

Graph* editGraph(Graph* graph, int* modified) {
	int choice, index;
	ListElem iterator;
	AdjacentLocation* adjLocation;
	int counter;

	showGraph(graph);

	printf(" Insira o indice da localizacao a editar: ");
	char auxIndex[10];
	scanf(" %[^\n]%*c", auxIndex);

	index = 0;
	if (isInt(auxIndex))
		index = stringToInt(auxIndex);

	if (index != 0)
	{
		// Finding the location in the graph
		Location* selectedLocation = NULL;
		ListElem temp = graph->locations;
		counter = 1;
		while (temp != NULL) {
			if (counter == index) {
				selectedLocation = (Location*)temp->data;
				break;
			}
			counter++;
			temp = temp->next;
		}

		if (selectedLocation == NULL) {
			return graph;
		}

		printf(" 1. Remover localizacao selecionada.\n");
		printf(" 2. Remover uma adjacencia da localizacao selecionada.\n");
		printf(" 3. Alterar o peso de uma adjacencia da localizacao selecionada.\n");
		printf(" Escolha o modo de edicao: ");
		char auxChoice[10];
		scanf(" %[^\n]%*c", auxChoice);

		choice = 0;
		if (isInt(auxChoice))
			choice = stringToInt(auxChoice);

		switch (choice) {
		case 1:
			// Removing the selected location from the graph
			removeLocation(graph, selectedLocation);
			*modified = 1;
			printf(" Localizacao removida com sucesso.\n");
			break;

		case 2:
			// Print adjacent locations with index
			iterator = selectedLocation->adjacentLocations;
			counter = 1;
			while (iterator != NULL) {
				adjLocation = (AdjacentLocation*)iterator->data;
				printf(" %d. %s\n", counter++, adjLocation->location->name);
				iterator = iterator->next;
			}

			printf(" Insira o indice da adjacencia a remover: ");
			char auxIndex[10];
			scanf(" %[^\n]%*c", auxIndex);

			index = 0;
			if (isInt(auxIndex))
				index = stringToInt(auxIndex);

			if (index < 1 || index >= counter) {
				return graph;
			}

			// Removing the adjacent location from the location
			removeAdjacentLocation(selectedLocation, index - 1);
			*modified = 1;
			printf(" Adjacencia removida com sucesso.\n");
			break;
		case 3:
			// Print adjacent locations with index
			iterator = selectedLocation->adjacentLocations;
			counter = 1;
			while (iterator != NULL) {
				adjLocation = (AdjacentLocation*)iterator->data;
				printf(" %d. %s %.3f kms\n", counter++, adjLocation->location->name, adjLocation->weight);
				iterator = iterator->next;
			}

			printf(" Insira o indice do peso da adjacencia a editar: ");
			char auxIndexEdit[10]; // Updated variable name
			scanf(" %[^\n]%*c", auxIndexEdit);

			index = 0;
			if (isInt(auxIndexEdit))
				index = stringToInt(auxIndexEdit);

			if (index < 1 || index >= counter) {
				return graph;
			}

			// Get the selected adjacent location
			iterator = selectedLocation->adjacentLocations;
			counter = 1;
			while (counter < index) {
				iterator = iterator->next;
				counter++;
			}
			adjLocation = (AdjacentLocation*)iterator->data;

			printf(" Insira o novo peso da adjacencia: ");
			char auxWeight[10];
			scanf(" %[^\n]%*c", auxWeight);

			double weight = 0.0;
			if (!isFloat(auxWeight) && !isInt(auxWeight))
				break;

			// Update the weight for the selected adjacent location
			weight = (double)stringToFloat(auxWeight);
			adjLocation->weight = weight;

			// Check if there is also an adjacency from adjLocation to selectedLocation
			AdjacentLocation* reverseAdjLocation = findAdjacentLocation(adjLocation->location, selectedLocation);
			if (reverseAdjLocation != NULL) {
				reverseAdjLocation->weight = weight;
			}

			*modified = 1;
			printf(" Peso da adjacencia alterado com sucesso.\n");
			break;
		default:
			break;
		}
		return graph;
	}
	return graph;
}
