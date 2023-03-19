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
/// List of vehicles
/// </summary>
ListElem listV;
/// <summary>
/// List of histories
/// </summary>
ListElem listH;

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
/// Allows the user to select the vehicle to use on it's trip.
/// </summary>
/// <param name="listAV">A pointer to the head of the linked list of available vehicles.</param>
/// <returns>1 if selection was successfull. 0 otherwise.</returns>
int choosingVehicle(ListElem listAV);

/// <summary>
/// Simulates the renting of a vehicle by asking the user for input.
/// </summary>
/// <param name="c">A pointer to the current logged in client.</param>
/// <param name="v">A pointer to the currently selected vehicle.</param>
/// <returns>1 if simulation was successfull. 0 otherwise.</returns>
int simulateTrip(Client c, Vehicle v);

/// <summary>
/// Allows the user to add/edit/remove any client/admin/vehicle, to see the histories filtered by vehicle(type+code)/type/client and to see some stats for clients/vehicles.
/// Updates the files with every change.
/// </summary>
void modeAdmin();

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

    //ListElem sortListV = NULL;
    ListElem sortListV = sortByTotalKms(listVehicles);
    showListIterative(sortListV, &showVehicle);

    wait();
}

void modeAdmin() 
{
    int option = 99;

    do {
        clrscr();

        adminmenu();

        fflush(stdin);

        ListElem listHfiltered = NULL;
        char filter[60];
        int modified = 0;

        printf(" Selecione a opcao pretendida: ");
        char auxOption[10];
        scanf(" %[^\n]", auxOption);
        printf("\n");
        if (isInt(auxOption))
        {
            option = stringToInt(auxOption);
            switch (option) {
            case 1:
                listV = registerVehicle(listV,&modified);
                if (modified)
                    storeDataVehicles(listV);
                else
                    errornotvalid();
                break;
            case 2:
                listV = editVehicle(listV, &modified);
                if (modified)
                    storeDataVehicles(listV);
                else
                    errornotvalid();
                break;
            case 3:
                listV = removeVehicle(listV, &modified);
                if (modified)
                    storeDataVehicles(listV);
                else
                    errornotvalid();
                break;
            case 4:
                listC = registerClient(listC, listA, &modified);
                if (modified)
                    storeDataClients(listC);
                else
                    errornotvalid();
                break;
            case 5:
                listC = editClient(listC, &modified);
                if (modified)
                    storeDataClients(listC);
                else
                    errornotvalid();
                break;
            case 6:
                listC = removeClient(listC, &modified);
                if (modified)
                    storeDataClients(listC);
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
                showListIterative(listH, &showHistory);
                wait();
                break;
            case 14:
                vehicleStats(listV);
                break;
            case 15:
                clientStats(listC);
                break;
            case 0:
                break;
            default:
                errornotvalid();
            }
        }
    } while (option != 0);



}

int simulateTrip(Client c, Vehicle v) {

    simulatemenu();

    History h = (History)malloc(sizeof(struct datahistory));

    strcpy(h->user, c->username);

    char type_code[60];
    sprintf(type_code, "%s_%d", v->type, v->code);

    strcpy(h->transport, type_code);

    strcpy(h->start, v->geolocation);

    printf("\n Insira o geocodigo do local de chegada: "); //NOME
    scanf(" %[^\n]%*c", h->finish);

    printf("\n Insira a distancia do percurso: ");
    char auxDistance[10];
    scanf(" %[^\n]%*c", auxDistance);

    if (isInt(auxDistance) || isFloat(auxDistance))
    {
        h->distance = stringToFloat(auxDistance);
    }
    else
    {
        errornotvalidinfo();
        return 0;
    }
    if (h->distance <= 0)
    {
        errornotvalidinfo();
        return 0;
    }

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

    strcpy(v->geolocation, h->finish);
    v->totalkms += h->distance;
    v->inUse = 0;

    storeDataClients(listC);
    storeDataVehicles(listV);
    storeDataHistory(listH);

    return 1;
}

int choosingVehicle(ListElem listAV)
{
    int choice = 99;
    int lenght = listLength(listAV);
    do {
        availablevehicles();
        showListIterative(listAV, &showVehicle);
        
        printf(" Selecione o veiculo pretendido: ");
        char auxChoice[10];
        scanf(" %[^\n]", auxChoice);
        printf("\n");

        if (isInt(auxChoice))
            choice = stringToInt(auxChoice);
         
        if ((choice != 0) && (choice <= lenght))
        {
            ListElem auxElem = obtainElementPosition(listAV, choice - 1);
            Vehicle auxVehicle = (Vehicle)auxElem->data;

            Vehicle inUseVehicle = getVehicleByTypeAndCode(listV, auxVehicle->type, auxVehicle->code);
            inUseVehicle->inUse = 1;

            storeDataVehicles(listV);
            currentVehicle = inUseVehicle;
            return 1;
        }

    } while ((choice > 0) && (choice <= lenght));

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
                    if (choosingVehicle(listAvailableV)) {
                        if (!simulateTrip(currentClient, currentVehicle))
                        {
                            currentVehicle->inUse = 0;
                            storeDataVehicles(listV);
                        }
                    }
                }
                else
                    novehiclesavailable();

                break;
            case 2:
                printf(" Introduza a geolocalizao a procurar: ");
                char geolocation[50];
                scanf(" %[^\n]", geolocation);

                listAvailableV = filterVehicleByInUse(listV);
                listAvailableGeoV = filterVehicleByGeo(listAvailableV, geolocation);

                if (listLength(listAvailableGeoV) > 0)
                {
                    if (choosingVehicle(listAvailableGeoV)) {
                        if (!simulateTrip(currentClient, currentVehicle))
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
    scanf(" %[^\n]", tmpUsername); // %[^\n]%*c
    printf(" Insira a sua password: ");
    scanf(" %[^\n]", tmpPassword);

    ListElem currentList;
    // Check admin list
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
    // Check client list
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

    listA = loadDataAdmins(listA);
    listC = loadDataClients(listC);
    listV = loadDataVehicles(listV);
    listH = loadDataHistory(listH);

    int option = 99;
    do {
        fflush(stdin);

        menu();

        printf(" Selecione a opcao pretendida: ");
        char auxOption[10];
        scanf(" %[^\n]", auxOption);
        printf("\n");

        if (isInt(auxOption))
        {
            option = stringToInt(auxOption);
            int loginResult = 0;
            int modified = 0;
            switch (option)
            {
            case 1:
                listC = registerClient(listC, listA, &modified);
                if (modified)
                    storeDataClients(listC);
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
        //free(a);
        //free(v);
        //free(c);
    } while (option != 0);

    return 0;
}
