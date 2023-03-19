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

typedef struct
{
    char naccount[50];
    float balance;
} BankAccount;

//Variaveis globais
ListElem listC = NULL;
ListElem listA = NULL;
ListElem listV = NULL;
ListElem listH = NULL;

Client currentClient;
Vehicle currentVehicle;

// Prototypes
int main();
//int registerClient(ListElem listClient);
//int usernameExists(char* username, ListElem listClients, ListElem listAdmins);
int login();
void modeClient();
int choosingVehicle(ListElem listAV);
int simulateTrip(Client c, Vehicle v);
void modeAdmin();
//int registerAdmin(ListElem listAdmin);
//int registerVehicle(ListElem listVehicle);
//int vehicleExists(int code, char* type, ListElem listVehicles);
void vehicleStats(ListElem listVehicles);
void clientStats(ListElem listClients);
char getWorkingDirectory(char cwd[]);

// Implementações
char getWorkingDirectory(char cwd[])
{

}

void clientStats(ListElem listClients)
{
    char clientMaxTrips[21];
    int maxTrips = 0;
    findClientWithMostTrips(listClients, clientMaxTrips, &maxTrips);
    printf(" O cliente com mais viagens e: %s com %d viagens.\n", clientMaxTrips, maxTrips);

    float avgAge = calculateAverageAge(listClients);
    printf(" A idade media dos utilizadores e: %.1f anos\n", avgAge);

    ListElem sortListC = sortByTrips(listClients);
    showListIterative(sortListC, &showClient);

    wait();
}

void vehicleStats(ListElem listVehicles)
{
    float percentInUse = percentageInUse(listVehicles);
    printf(" A percentagem de veiculos em uso e de: %.2f%%\n", percentInUse);

    float avgAutonomy = averageAutonomy(listVehicles);
    printf(" A autonomia media de todos os veiculos registados e de: %.2fkms\n", avgAutonomy);

    ListElem auxListV = filterVehicleByBattery(listVehicles);
    showListIterative(auxListV, &showVehicle);

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
                modified = 0;
                listV = registerVehicle(listV,&modified);
                if(modified)
                    storeDataVehicles(listV);
                break;
            case 2:
                modified = 0;
                listV = editVehicle(listV, &modified);
                if (modified)
                    storeDataVehicles(listV);
                else
                    errornotvalid();
                break;
            case 3:
                modified = 0;
                listV = removeVehicle(listV, &modified);
                if (modified)
                    storeDataVehicles(listV);
                else
                    errornotvalid();
                break;
            case 4:
                modified = 0;
                listC = registerClient(listC, listA, &modified);
                if (modified)
                    storeDataClients(listC);
                else
                    errornotvalid();
                break;
            case 5:
                modified = 0;
                listC = editClient(listC, &modified);
                if (modified)
                    storeDataClients(listC);
                else
                    errornotvalid();
                break;
            case 6:
                modified = 0;
                listC = removeClient(listC, &modified);
                if (modified)
                    storeDataClients(listC);
                else
                    errornotvalid();
                break;
            case 7:
                modified = 0;
                listA = registerAdmin(listA, listC, &modified);
                if (modified)
                    storeDataAdmins(listA);
                else
                    errornotvalid();
                break;
            case 8:
                modified = 0;
                listA = editAdmin(listA, &modified);
                if (modified)
                    storeDataAdmins(listA);
                else
                    errornotvalid();
                break;
            case 9:
                modified = 0;
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
        h->duration = stringToFloat(auxDuration);
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
    c->balance -= h->cost;

    strcpy(v->geolocation, h->finish);
    v->totalkms += h->distance;
    v->inUse = 0;

    //showHistory(h);

    storeDataClients(listC);
    storeDataVehicles(listV);
    storeDataHistory(listH);

    return 1;
}

int choosingVehicle(ListElem listAV)
{
    int choice = 99;
    do {
        availablevehicles();
        showListIterative(listAV, &showVehicle);

        printf(" Selecione o veiculo pretendido: ");
        char auxChoice[10];
        scanf(" %[^\n]", auxChoice);
        printf("\n");

        if (isInt(auxChoice))
        {
            choice = stringToInt(auxChoice);
            ListElem auxElem = obtainElementPosition(listAV, choice - 1);
            Vehicle auxVehicle = (Vehicle)auxElem->data;

            Vehicle inUseVehicle = getVehicleByTypeAndCode(listV, auxVehicle->type, auxVehicle->code);
            inUseVehicle->inUse = 1;

            storeDataVehicles(listV);
            currentVehicle = inUseVehicle;
            return 1;
        }
    } while (choice != 0);

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

// Procedimento principal
int main()
{
    listA = loadDataAdmins(listA);
    listC = loadDataClients(listC);
    listV = loadDataVehicles(listV);
    listH = loadDataHistory(listH);

    int option = 99;
    do {
        fflush(stdin);

        menu();

        //loadData(firstLoad);

        showListIterative(listA, &showAdmin);
        showListIterative(listC, &showClient);
        showListIterative(listV, &showVehicle);
        showListIterative(listH, &showHistory);

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
                if(registerClient(listC,listA,&modified))
                    storeDataClients(listC);
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
