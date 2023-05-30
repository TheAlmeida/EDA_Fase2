#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "ll.h"
#include "menu.h"
#include "admin.h"
#include "client.h"
#include "vehicle.h"
#include "history.h"
#include "what3words.h"
#include "graph.h"

//Global vars

/// <summary>
/// List of clients.
/// </summary>
ListElem listC;
/// <summary>
/// List of admins.
/// </summary>
ListElem listA;
/// <summary>
/// List of vehicles.
/// </summary>
ListElem listV;
/// <summary>
/// List of histories.
/// </summary>
ListElem listH;
/// <summary>
/// Graph containing locations.
/// </summary>
Graph* graph;

/// <summary>
/// Current logged in client.
/// </summary>
Client currentClient;
/// <summary>
/// Current vehicle in simulation.
/// </summary>
Vehicle currentVehicle;

// Prototypes

/// <summary>
/// Loads the linked lists from binary files.
/// Allows the user to register, login, check the physical stores information or kill the program.
/// </summary>
/// <returns>0 if exited correctly.</returns>
int main();

/// <summary>
/// Iterates over the admin and client lists until it finds a corresponding username, then checks the password.
/// </summary>
/// <returns>0 if login failed, 1 if login successfull as admin, 2 if login successfull for client</returns>
int login();

/// <summary>
/// Allows the user to rent a vehicle by vehicles available(/in a specified geocode), consult and charge the user's account balance and to see some stats.
/// </summary>
void modeClient();

/// <summary>
/// Allows the user to choose a vehicle for a trip based on the provided graph and start location.
/// </summary>
/// <param name="graph">Pointer to the graph containing locations and vehicles.</param>
/// <param name="start">Pointer to the user location.</param>
/// <param name="mode">Mode indicating how the vehicles should be filtered (1 for range and type of vehicle, 2 for geolocation)</param>
/// <param name="geolocation">Geolocation used for filtering vehicles in mode 2.</param>
/// <returns>1 if a vehicle was successfully chosen, 0 otherwise.</returns>
int choosingVehicle(Graph* graph, Location* start, int mode, char* geolocation);

/// <summary>
/// Simulates a trip for a client using a vehicle.
/// </summary>
/// <param name="c">Client object representing the client.</param>
/// <param name="v">Vehicle object representing the vehicle.</param>
/// <param name="graph">Pointer to the graph.</param>
/// <returns>1 if the trip was successfully simulated, 0 otherwise.</returns>
int simulateTrip(Client c, Vehicle v, Graph* graph);

/// <summary>
/// Allows the user to add/edit/remove any client/admin/vehicle, to see the histories filtered by vehicle(type+code)/type/client and to see some stats for clients/vehicles.
/// Updates the files with every change.
/// </summary>
void modeAdmin();

/// <summary>
/// Returns vehicles to the start location if their battery is under 50% (battery field < 50), unloading vehicles and refueling along the way if necessary.
/// </summary>
/// <param name="graph">Pointer to the graph containing locations and vehicles.</param>
/// <param name="start">Pointer to the starting location where vehicles are also returned to.</param>
/// <param name="totalWeight">Total weight allowed to carry at once.</param>
/// <returns>1 if any vehicles were successfully returned, 0 otherwise.</returns>
int returnVehicles(Graph* graph, Location* start, float totalWeight);

/// <summary>
/// Prints the percentage of vehicles in use off the linked list of vehicles.
/// Prints the average autonomy of the field autonomy in all the elements of the linked list of vehicles.
/// Prints the filtered linked list of vehicles where each vehicle must have the battery field below 50.
/// Prints the sorted linked list of vehicles by decreasing order of total kms.
/// </summary>
/// <param name="listVehicles">A pointer to the head of the linked list of vehicles.</param>
void vehicleStats(ListElem listVehicles);

/// <summary>
/// Prints the highest value of the field totalTrips in all the elements of the linked list of clients and the corresponding client's username.
/// Prints the average age of the field age in all the elements of the linked list of clients.
/// Prints the sorted linked list of clients by decreasing order of total Trips.
/// </summary>
/// <param name="listClients">A pointer to the head of the linked list of clients.</param>
void clientStats(ListElem listClients);

// Implementations

void clientStats(ListElem listClients)
{
	admincstats();

	char clientMaxTrips[21];
	int maxTrips = 0;
	findClientWithMostTrips(listClients, clientMaxTrips, &maxTrips);
	printf(" O cliente com mais viagens e: %s com %d viagens.\n\n", clientMaxTrips, maxTrips);

	float avgAge = calculateAverageAge(listClients);
	printf(" A idade media dos utilizadores e: %.1f anos\n\n", avgAge);

	admincstatsbytrips();

	ListElem sortListC = sortByTrips(listClients);
	showListIterative(sortListC, &showClient);

	wait();
}

void vehicleStats(ListElem listVehicles)
{
	adminvstats();

	float percentInUse = percentageInUse(listVehicles);
	printf(" A percentagem de veiculos em uso e de: %.2f%%\n\n", percentInUse);

	float avgAutonomy = averageAutonomy(listVehicles);
	printf(" A autonomia media de todos os veiculos registados e de: %.2fkms\n\n", avgAutonomy);

	adminvstatsbatteryunder50();

	ListElem auxListV = filterVehicleByBattery(listVehicles);
	showListIterative(auxListV, &showVehicle);

	adminvstatstotalkms();

	ListElem sortListV = sortByTotalKms(listVehicles);
	showListIterative(sortListV, &showVehicle);

	wait();
}

int returnVehicles(Graph* graph, Location* start, float totalWeight) {
	Location* targetLocation = calculateShortestPathWithVehicle(graph, start);
	int pickedUp = 0; // Flag to indicate if any vehicles were picked up

	while (targetLocation != NULL) {
		// Calculate the shortest path from start to the target location
		ListElem path = calculateShortestPath(graph, start, targetLocation);

		// Traverse the path
		ListElem pathElem = path;
		Location* currentLocation = NULL;
		float remainingWeight = totalWeight;
		ListElem pickedUpVehicles = NULL; // List of vehicles picked up along the path
		PickUpResult result = { NULL, 0 }; // Initialize result outside the loop

		while (pathElem != NULL) {
			currentLocation = (Location*)pathElem->data;
			result = pickUpVehicles(currentLocation, remainingWeight);
			if (result.pickedUpVehicles != NULL) {
				pickedUp = 1;
			}
			// Update remaining weight
			ListElem vehiclesElem = result.pickedUpVehicles;
			while (vehiclesElem != NULL) {
				Vehicle vehicle = (Vehicle*)vehiclesElem->data;
				remainingWeight -= vehicle->weight;
				vehiclesElem = vehiclesElem->next;
			}
			printf("Picking up vehicles at %s, remaining weight: %.2f\n", currentLocation->name, remainingWeight);
			// Add vehicles to the list of picked up vehicles
			pickedUpVehicles = appendList(pickedUpVehicles, result.pickedUpVehicles);
			if (remainingWeight <= 0 || result.vehicleTooHeavy) {
				break;
			}
			pathElem = pathElem->next;
		}

		// After the truck can't pick up more vehicles, or encounters a vehicle too heavy to lift, it returns to the start location
		if (remainingWeight <= totalWeight * 0.25 || result.vehicleTooHeavy || calculateShortestPathWithVehicle(graph, start) == NULL) {
			// Calculate the shortest path from current location to the start location
			ListElem returnPath = calculateShortestPath(graph, currentLocation, start);
			printf("Shortest path from %s to %s:\n", currentLocation->name, start->name);
			showPath(returnPath);
			printf("\n");

			// Traverse the return path (without picking up vehicles)
			ListElem returnPathElem = returnPath;
			while (returnPathElem != NULL) {
				returnPathElem = returnPathElem->next;
			}

			// Unload vehicles, refuel, etc. at the start location
			unloadVehicles(start, pickedUpVehicles);
			reorderList(&listV, compareAutonomy);

			// Free the return path and the list of picked up vehicles
			freePath(returnPath);
			freePath(pickedUpVehicles);
		}

		// Free the path after traversing it
		freePath(path);

		targetLocation = calculateShortestPathWithVehicle(graph, start);
	}
	return pickedUp;
}

void modeAdmin()
{
	int option = 99;

	do {
		clrscr();

		adminmenu();

		fflush(stdin);

		ListElem listHfiltered = NULL;
		int modified = 0;
		char oldGeolocation[50] = "";
		Vehicle v = (Vehicle)malloc(sizeof(struct datavehicle));
		Client c = (Client)malloc(sizeof(struct dataclient));
		Location* base = findLocationByGeolocation(graph, garage);

		printf(" Selecione a opcao pretendida: ");
		char auxOption[10];
		scanf(" %[^\n]", auxOption);
		printf("\n");
		if (isInt(auxOption))
		{
			option = stringToInt(auxOption);
			switch (option) {
			case 1:
				listV = registerVehicle(listV, &modified, v);
				if (modified)
				{
					storeDataVehicles(listV);
					Location* location = findLocationByGeolocation(graph, v->geolocation);
					if (location == NULL)
					{
						location = createLocation(v->geolocation, 0.0, 0.0);
						addLocation(graph, location);
						maintainConnectivity(graph);
					}
					addVehicleToLocation(location, v);

					storeDataGraph(graph);
				}
				else
					errornotvalid();
				break;
			case 2:
				listV = editVehicle(listV, &modified, &v, oldGeolocation);
				if (modified)
				{
					storeDataVehicles(listV);
					if (strcmp(oldGeolocation, v->geolocation));
					{
						Location* oldLocation = findLocationByGeolocation(graph, oldGeolocation);
						removeVehicleFromLocation(oldLocation, v->code, v->type);
						Location* newLocation = findLocationByGeolocation(graph, v->geolocation);
						if (newLocation == NULL)
						{
							newLocation = createLocation(v->geolocation, 0.0, 0.0);
							addLocation(graph, newLocation);
							maintainConnectivity(graph);
						}
						addVehicleToLocation(newLocation, v);
					}
					storeDataGraph(graph);
				}
				else
					errornotvalid();
				break;
			case 3:
				listV = removeVehicle(listV, &modified, &v);
				if (modified)
				{
					Location* vehicleLocation = findLocationByVehicle(graph, v);
					removeVehicleFromLocation(vehicleLocation, v->code, v->type);
					storeDataVehicles(listV);
					storeDataGraph(graph);
					free(v);
				}
				else
					errornotvalid();
				break;
			case 4:
				listC = registerClient(listC, listA, &modified, c);
				if (modified)
				{
					storeDataClients(listC);
					Location* location = findLocationByGeolocation(graph, c->geolocation);
					if (location == NULL)
					{
						location = createLocation(c->geolocation, 0.0, 0.0);
						addLocation(graph, location);
						maintainConnectivity(graph);
					}
					addClientToLocation(location, c);
					storeDataGraph(graph);
				}
				else
					errornotvalid();
				break;
			case 5:
				listC = editClient(listC, &modified, &c, oldGeolocation);
				if (modified)
				{
					storeDataClients(listC);
					if (strcmp(oldGeolocation, c->geolocation));
					{
						Location* oldLocation = findLocationByGeolocation(graph, oldGeolocation);
						removeClientFromLocation(oldLocation, c->username);
						Location* newLocation = findLocationByGeolocation(graph, c->geolocation);
						if (newLocation == NULL)
						{
							newLocation = createLocation(c->geolocation, 0.0, 0.0);
							addLocation(graph, newLocation);
							maintainConnectivity(graph);
						}
						addClientToLocation(newLocation, c);
					}
					storeDataGraph(graph);
				}
				else
					errornotvalid();
				break;
			case 6:
				listC = removeClient(listC, &modified, &c);
				if (modified)
				{
					Location* clientLocation = findLocationByGeolocation(graph, c->geolocation);
					removeClientFromLocation(clientLocation, c->username);
					storeDataClients(listC);
					storeDataGraph(graph);
					free(c);
				}
				else
					errornotvalid();
				break;
			case 7:
				listA = registerAdmin(listA, listC, &modified);
				if (modified)
					storeDataAdmins(listA);
				else
					errornotvalid();
				break;
			case 8:
				listA = editAdmin(listA, &modified);
				if (modified)
					storeDataAdmins(listA);
				else
					errornotvalid();
				break;
			case 9:
				listA = removeAdmin(listA, &modified);
				if (modified)
					storeDataAdmins(listA);
				else
					errornotvalid();
				break;
			case 10:
				listHfiltered = filterHistory(listH, 0);
				if (listHfiltered == NULL)
					errornotvalid();
				else
				{
					showListIterative(listHfiltered, &showHistory);
					wait();
				}
				break;
			case 11:
				listHfiltered = filterHistory(listH, 1);
				if (listHfiltered == NULL)
					errornotvalid();
				else
				{
					showListIterative(listHfiltered, &showHistory);
					wait();
				}
				break;
			case 12:
				listHfiltered = filterHistory(listH, 2);
				if (listHfiltered == NULL)
					errornotvalid();
				else
				{
					showListIterative(listHfiltered, &showHistory);
					wait();
				}
				break;
			case 13:
				adminhistory();
				showListIterative(listH, &showHistory);
				wait();
				break;
			case 14:
				vehicleStats(listV);
				break;
			case 15:
				clientStats(listC);
				break;
			case 16:
				if (returnVehicles(graph, base, 2000))
				{
					showListIterative(listV, &showVehicle);
					storeDataVehicles(listV);
					showGraph(graph);
					storeDataGraph(graph);
					wait();
				}
				break;
			case 17:
				graph = editGraph(graph, &modified);
				if (modified)
				{
					storeDataGraph(graph);
					storeDataClients(listC);
					reorderList(&listV, compareAutonomy);
					storeDataVehicles(listV);
				}
				else
					errornotvalid();
				break;
			case 0:
				break;
			default:
				errornotvalid();
			}
		}
	} while (option != 0);



}

int simulateTrip(Client c, Vehicle v, Graph* graph) {

	History h = (History)malloc(sizeof(struct datahistory));

	strcpy(h->user, c->username);

	char type_code[60];
	sprintf(type_code, "%s_%d", v->type, v->code);
	strcpy(h->transport, type_code);

	strcpy(h->start, v->geolocation);

	do
	{
		simulatemenu();
		printf("\n Insira o geocodigo do local de chegada: ");
		scanf(" %[^\n]%*c", h->finish);
	} while (!validGeolocation(h->finish));
	

	// Check if the finish geolocation is already added as a location in the graph
	Location* finishLocation = findLocationByGeolocation(graph, h->finish);
	if (finishLocation == NULL) {
		// Geolocation not found, create a new location and add it to the graph
		finishLocation = createLocation(h->finish, 0.0, 0.0);
		addLocation(graph, finishLocation);
		maintainConnectivity(graph);
	}

	Location* startLocation = findLocationByGeolocation(graph, h->start);

	double distance = calculateDistance(startLocation->coordinates.latitude, startLocation->coordinates.longitude,
		finishLocation->coordinates.latitude, finishLocation->coordinates.longitude);

	// Add finishLocation as an adjacent location to the starting location
	addAdjacentLocation(startLocation, finishLocation, distance);
	addAdjacentLocation(finishLocation, startLocation, distance);


	h->distance = (float)distance;

	printf("\n Insira a duracao do percurso em horas: ");
	char auxDuration[10];
	scanf(" %[^\n]%*c", auxDuration);

	if (isInt(auxDuration) || isFloat(auxDuration))
	{
		int hours = 0, minutes = 0;
		sscanf(auxDuration, "%d.%d", &hours, &minutes);
		h->duration = (float)hours + ((float)minutes / 60.0);
	}
	else
	{
		errornotvalidinfo();
		return 0;
	}
	if (h->duration <= 0)
	{
		errornotvalidinfo();
		return 0;
	}

	h->cost = (h->duration) * (v->costhour) + (h->distance) * (v->costkm);
	listH = addItemLastIterative(listH, h);

	c->totalkms += h->distance;
	c->totaltrips++;

	if (h->cost > c->balance)
	{

		while (h->cost > c->balance)
		{
			float difference = (h->cost) - (c->balance);
			printf("\n Nao tem saldo que chegue! Por favor, carregue a sua conta com pelo menos %.2f euros. \n", difference);
			int modified = 0;
			addBalance(c, &modified);
			if (modified)
				storeDataClients(listC);
			else
				errornotvalidinfo();
		}
	}

	c->balance -= h->cost;
	printf("\n A viagem teve um custo de: %2.f euros.\n O seu balanco e de: %.2f euros. \n\n", h->cost, c->balance);
	wait();

	strcpy(c->geolocation, h->finish);
	strcpy(v->geolocation, h->finish);
	v->totalkms += h->distance;
	v->inUse = 0;

	// Remove used client and vehicle from the starting location
	removeClientFromLocation(startLocation, c->username);
	removeVehicleFromLocation(startLocation, v->code, v->type);

	// Add client and vehicle to the finish location
	addClientToLocation(finishLocation, c);
	addVehicleToLocation(finishLocation, v);

	storeDataClients(listC);
	storeDataVehicles(listV);
	storeDataHistory(listH);
	storeDataGraph(graph);

	return 1;
}

int choosingVehicle(Graph* graph, Location* start, int mode, char* geolocation)
{
	int choice = 0;
	int length = 0;
	double maxDistance = DBL_MAX;
	char typeToFilter[50];
	ListElem filteredVehicles = NULL;

	if (mode == 1)
	{
		printf(" Insira a distancia maxima a que se pode encontrar o veiculo: ");
		char auxMaxDist[10];
		scanf(" %[^\n]%*c", auxMaxDist);

		if (isInt(auxMaxDist) || isFloat(auxMaxDist))
			maxDistance = stringToFloat(auxMaxDist);
		else
			return 0;

		//MOSTRAR OS QUE EXISTEM?
		printf("\n Insira o tipo de veiculos a procurar: ");
		scanf(" %[^\n]%*c", typeToFilter);
	}

	// Iterate over locations and vehicles to filter available vehicles within the maxDistance range
	ListElem currLocationElem = graph->locations;
	while (currLocationElem != NULL) {
		Location* location = (Location*)currLocationElem->data;
		ListElem vehicleElem = location->vehicleList;
		while (vehicleElem != NULL) {
			Vehicle vehicle = (Vehicle)vehicleElem->data;
			if (vehicle->inUse == 0) {
				Location* vehicleLocation = findLocationByVehicle(graph, vehicle);
				ListElem shortestPath = calculateShortestPath(graph, start, vehicleLocation);
				double distance = calculatePathDistance(shortestPath);
				if (distance <= maxDistance)
				{
					if (((mode == 1) && !strcmp(vehicle->type, typeToFilter)) || ((mode == 2) && !strcmp(vehicle->geolocation, geolocation))) {
						length++;
						filteredVehicles = addItemLastIterative(filteredVehicles, vehicle);
						printf(" %d.\n", length);
						showVehicleRent(vehicle);
						printf("   Distancia: %.3f\n", distance);
						printf("   Caminho: ");
						showPath(shortestPath);
						printf("\n\n");
					}
				}
			}
			vehicleElem = vehicleElem->next;
		}
		currLocationElem = currLocationElem->next;
	}

	if (length == 0) {
		return 0;
	}

	do {
		printf(" Selecione o veiculo pretendido: ");
		char auxChoice[10];
		scanf(" %[^\n]", auxChoice);
		printf("\n");

		if (isInt(auxChoice))
			choice = stringToInt(auxChoice);

		if ((choice >= 1) && (choice <= length))
		{
			ListElem selectedVehicleElem = obtainElementPosition(filteredVehicles, choice - 1);
			Vehicle selectedVehicle = (Vehicle)selectedVehicleElem->data;
			selectedVehicle->inUse = 1;
			storeDataVehicles(listV);
			currentVehicle = selectedVehicle;
			free(filteredVehicles);
			return 1;
		}
		else
		{
			errornotvalid();
			clrscr();
		}

	} while (choice != 0);

	free(filteredVehicles);
	return 0;
}

void modeClient()
{

	int option = 99;
	do
	{
		clientmenu();
		printf(" Selecione a opcao pretendida: ");

		char auxOption[10];
		scanf(" %[^\n]", auxOption);
		printf("\n");

		int modified = 0;

		if (isInt(auxOption))
		{
			ListElem listAvailableV = NULL;
			ListElem listAvailableGeoV = NULL;

			option = stringToInt(auxOption);
			switch (option)
			{
			case 1:
				listAvailableV = filterVehicleByInUse(listV);

				if (listLength(listAvailableV) > 0)
				{
					Location* clientLocation = findLocationByGeolocation(graph, currentClient->geolocation);
					if (choosingVehicle(graph, clientLocation, 1, NULL)) {
						if (!simulateTrip(currentClient, currentVehicle, graph))
						{
							currentVehicle->inUse = 0;
							storeDataVehicles(listV);
						}
					}
					else
						novehiclesavailable();
				}
				else
					novehiclesavailable();

				break;
			case 2:
				printf(" Introduza a geolocalizao a procurar: ");
				char geolocation[50];
				scanf(" %[^\n]", geolocation);

				if (!validGeolocation(geolocation))
				{
					errornotvalidinfo();
					break;
				}
					
				listAvailableV = filterVehicleByInUse(listV);
				listAvailableGeoV = filterVehicleByGeo(listAvailableV, geolocation);

				if (listLength(listAvailableGeoV) > 0)
				{
					Location* clientLocation = findLocationByGeolocation(graph, currentClient->geolocation);
					if (choosingVehicle(graph, clientLocation, 2, geolocation)) {
						if (!simulateTrip(currentClient, currentVehicle, graph))
						{
							currentVehicle->inUse = 0;
							storeDataVehicles(listV);
						}
					}
				}
				else
					novehiclesavailable();
				break;
			case 3:
				clientbalance();

				printf(" O seu saldo e de: %.2f euros \n", currentClient->balance);
				wait();
				break;
			case 4:
				clientbalance();
				modified = 0;
				currentClient = addBalance(currentClient, &modified);
				if (modified)
					storeDataClients(listC);
				else
					errornotvalidinfo();
				break;
			case 5:
				historyClientStats(listH, currentClient->username);
				wait();
				break;
			default:
				;
			}
		}
		else
			errornotvalid();

	} while (option != 0);

}

int login() {
	char tmpUsername[50];
	char tmpPassword[50];

	clrscr();
	signin();

	printf("\n");
	printf(" Insira o seu nome de utilizador: ");
	scanf(" %[^\n]", tmpUsername);
	printf(" Insira a sua password: ");
	scanf(" %[^\n]", tmpPassword);

	ListElem currentList;
	currentList = listA;
	while (currentList != NULL)
	{
		if (currentList->data != NULL)
		{
			if (strcmp(((Admin)currentList->data)->username, tmpUsername) == 0)
			{
				if (strcmp(((Admin)currentList->data)->password, tmpPassword) == 0)
				{
					// Login successful for admin
					signinokadmin();
					wait();
					return 1;
				}
				else
				{
					// Incorrect password
					errorpassword();
					return 0;
				}
			}
		}
		currentList = currentList->next;
	}
	currentList = listC;
	while (currentList != NULL)
	{
		if (currentList->data != NULL)
		{
			if (strcmp(((Client)currentList->data)->username, tmpUsername) == 0)
			{
				if (strcmp(((Client)currentList->data)->password, tmpPassword) == 0)
				{
					// Login successful for client
					currentClient = (Client)currentList->data;
					signinok(currentClient->username);
					wait();
					return 2;
				}
				else
				{
					// Incorrect password
					errorpassword();
					return 0;
				}
			}
		}
		currentList = currentList->next;
	}
	// Username not found
	errorusername();
	return 0;
}

int main()
{
	listA = NULL;
	listC = NULL;
	listV = NULL;
	listH = NULL;
	graph = createGraph();

	listA = loadDataAdmins(listA);
	listC = loadDataClients(listC);
	listV = loadDataVehicles(listV);
	listH = loadDataHistory(listH);
	graph = loadDataGraph(graph, listC, listV);

	// Code that generates a graph without a binary file. COMMENT-OUT OR DELETE THE ABOVE LINE TO USE THIS FEATURE (graph = loadDataGraph(graph, listC, listV);)
	/*
	createLocationsFromVehicles(graph, listV);
	createLocationsFromClients(graph, listC);
	pseudoStronglyConnectLocations(graph);
	storeDataGraph(graph);
	*/
	
	showGraph(graph);
	wait();

	int option = 99;
	do {
		fflush(stdin);

		menu();

		printf(" Selecione a opcao pretendida: ");
		char auxOption[10];
		scanf(" %[^\n]", auxOption);
		printf("\n");

		Client c = (Client)malloc(sizeof(struct dataclient));

		if (isInt(auxOption))
		{
			option = stringToInt(auxOption);
			int loginResult = 0;
			int modified = 0;
			switch (option)
			{
			case 1:
				listC = registerClient(listC, listA, &modified, c);
				if (modified)
				{
					storeDataClients(listC);
					Location* location = findLocationByGeolocation(graph, c->geolocation);
					if (location == NULL)
					{
						location = createLocation(c->geolocation, 0.0, 0.0);
						addLocation(graph, location);
						maintainConnectivity(graph);
					}
					addClientToLocation(location, c);
					storeDataGraph(graph);
				}
				else
					errornotvalid();
				break;

			case 2:
				loginResult = login();
				if (loginResult == 2)
				{
					modeClient();
				}
				else if (loginResult == 1)
					modeAdmin();
				break;
			case 3:
				clrscr();
				contactus();
				printf(" Selecione a opcao pretendida: ");
				char auxVisit[10];
				scanf(" %[^\n]", auxVisit);
				if (isInt(auxVisit))
				{
					int visit = stringToInt(auxVisit);
					printf("\n");
					switch (visit)
					{
					case 1:
						//ShellExecute(NULL, "open", "https://www.google.com/maps/place/Mercado+Municipal/@41.5512503,-8.4348027,14z/data=!4m8!1m2!2m1!1sbraga+mercado+!3m4!1s0x0:0x98c327809fae5bc0!8m2!3d41.5542807!4d-8.4275651?hl=pt-PT", NULL, NULL, SW_SHOWNORMAL);
						wait();
						break;
					case 2:
						//ShellExecute(NULL, "open", "https://www.google.com/maps/place/Mercado+Ferreira+Borges/@41.1418794,-8.6171275,17z/data=!3m1!4b1!4m5!3m4!1s0xd2464e1c90c863d:0xbdf9a36d350cdbae!8m2!3d41.1418794!4d-8.6149388?hl=pt-PT", NULL, NULL, SW_SHOWNORMAL);
						wait();
						break;
					case 3:
						//ShellExecute(NULL, "open", "https://www.google.pt/maps/place/Bairro+Alto+Market/@38.7031159,-9.1736784,14z/data=!4m8!1m2!2m1!1smercado+Lisboa!3m4!1s0x0:0x60293564ae4536c2!8m2!3d38.7128441!4d-9.1462576", NULL, NULL, SW_SHOWNORMAL);
						wait();
						break;
					case 0:
						break;
					default:
						errornotvalid();
					}
				}
				break;
			case 0:
				endprogram();
			default:
				errornotvalid();
			}
		}
	} while (option != 0);

	return 0;
}
