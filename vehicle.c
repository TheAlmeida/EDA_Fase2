#include "vehicle.h"

void showVehicle(void* data)
{
    Vehicle v = (Vehicle)data;
    if (v != NULL)
        printf(" Veiculo: %s\n"
            " Identificador: %d\n"
            " Localizacao: %s\n"
            " Peso: %.2f kgs\n"
            " Bateria: %.2f %%\n"
            " Autonomia: %.2f kms\n"
            " Custo/hora: %.2f euros\n"
            " Custo/km: %.2f euros\n"
            " Em uso: %d\n"
            " Total kms percorridos: %.2f kms\n",
            v->type, v->code, v->geolocation, v->weight, v->battery, v->autonomy, v->costhour, v->costkm, v->inUse, v->totalkms);
    printf("\n");
}

void showVehicleRent(void* data)
{
    Vehicle v = (Vehicle)data;
    if (v != NULL)
        printf(" Veiculo: %s\n"
            " Identificador: %d\n"
            " Localizacao: %s\n"
            " Bateria: %.2f %%\n"
            " Autonomia: %.2f kms\n"
            " Custo/hora: %.2f euros\n"
            " Custo/km: %.2f euros\n",
            v->type, v->code, v->geolocation, v->battery, v->autonomy, v->costhour, v->costkm);
    printf("\n");
}

ListElem loadDataVehicles(ListElem listVehicles)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        error();

    char vehicles_bin[500];
    strcpy(vehicles_bin, cwd);
    strcat(vehicles_bin, "\\vehicles.bin");

    FILE* vehiclesFile = fopen(vehicles_bin, "rb");
    if (vehiclesFile == NULL) {
        error();
    }

    Vehicle v = NULL;
    while (1) {
        v = (Vehicle)malloc(sizeof(struct datavehicle));
        if (fscanf(vehiclesFile, "%d|%49[^|]|%49[^|]|%f|%f|%f|%f|%f|%d|%f\n",
            &(v->code), v->type, v->geolocation, &(v->weight), &(v->battery),
            &(v->autonomy), &(v->costhour), &(v->costkm),
            &(v->inUse), &(v->totalkms)) == EOF) {
            free(v);
            break;
        }

        listVehicles = addItemOrderedIterative(listVehicles, v, &compareAutonomy);
    }

    fclose(vehiclesFile);
    return listVehicles;
}

void storeDataVehicles(ListElem listV)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        error();
    }

    char vehicles_bin[500];
    strcpy(vehicles_bin, cwd);
    strcat(vehicles_bin, "\\vehicles.bin");

    FILE* file = fopen(vehicles_bin, "wb"); // open the file in write binary mode
    if (file == NULL) {
        printf(" Error opening file %s.\n", vehicles_bin);
        return;
    }

    // iterate through the list of vehicles and write their information to the file
    ListElem current = listV;
    while (current != NULL) {
        Vehicle vehicle = (Vehicle)current->data;
        fprintf(file, "%d|%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f|%d|%.2f\n",
            vehicle->code, vehicle->type, vehicle->geolocation, vehicle->weight, vehicle->battery, vehicle->autonomy, vehicle->costhour, vehicle->costkm, vehicle->inUse, vehicle->totalkms);
        current = current->next;
    }

    fclose(file); // close the file
}

int compare(void* data1, void* data2)
{
    Vehicle d1 = (Vehicle)data1;
    Vehicle d2 = (Vehicle)data2;
    if (d1->code < d2->code)
        return (1);
    else if (d1->code > d2->code)
        return (-1);
    else
        return (0);
}

int compareAutonomy(void* data1, void* data2)
{
    Vehicle d1 = (Vehicle)data1;
    Vehicle d2 = (Vehicle)data2;
    if (d1->autonomy < d2->autonomy)
        return (1);
    else if (d1->autonomy > d2->autonomy)
        return (-1);
    else
        return (0);
}

int compareNamesVehicles(void* data1, void* data2)
{
    Vehicle d1 = (Vehicle)data1;
    Vehicle d2 = (Vehicle)data2;
    return (strcmp(d1->type, d2->type));
}

int compareTotalKms(void* data1, void* data2)
{
    Vehicle v1 = (Vehicle)data1;
    Vehicle v2 = (Vehicle)data2;

    if (v1->totalkms > v2->totalkms)
        return -1;
    else if (v1->totalkms < v2->totalkms)
        return 1;
    else
        return 0;
}

void changeVType(Vehicle vehicle, const char* newType)
{
    strcpy(vehicle->type, newType);
}

void changeVCode(Vehicle vehicle, int code)
{
    vehicle->code = code;
}

void changeVGeolocation(Vehicle vehicle, const char* newGeolocation)
{
    strcpy(vehicle->geolocation, newGeolocation);
}

void changeVWeight(Vehicle vehicle, float newWeight)
{
    vehicle->weight = newWeight;
}

void changeVBattery(Vehicle vehicle, float newBattery)
{
    vehicle->battery = newBattery;
}

void changeVAutonomy(Vehicle vehicle, float newAutonomy)
{
    vehicle->autonomy = newAutonomy;
}

void changeVCosthour(Vehicle vehicle, float newCosthour)
{
    vehicle->costhour = newCosthour;
}

void changeVCostkm(Vehicle vehicle, float newCostkm)
{
    vehicle->costkm = newCostkm;
}

void changeVstate(Vehicle vehicle, int inUse)
{
    vehicle->inUse = inUse;
}

void changeVkms(Vehicle vehicle, float newTotalkms)
{
    vehicle->totalkms = newTotalkms;
}

ListElem removeVehicle(ListElem listVehicle, int* modified, Vehicle* v) 
{
    showListIterative(listVehicle, &showVehicle);

    printf(" Insira o indice do veiculo a ser removido: ");
    char auxOption[10];
    scanf(" %[^\n]%*c", auxOption);

    int option = 0;
    if (isInt(auxOption))
        option = stringToInt(auxOption);

    if ((option != 0) && (option <= listLength(listVehicle)))
    {
        ListElem auxElem = obtainElementPosition(listVehicle, option - 1);
        *v = (Vehicle)auxElem->data;

        clrscr();
        showVehicle(*v);

        printf(" Deseja remover o veiculo acima? (1- Sim / 2- Nao): ");

        int removeOption = 0;
        char auxRemoveOption[10];
        scanf(" %[^\n]%*c", auxRemoveOption);

        if (isInt(auxRemoveOption))
            removeOption = stringToInt(auxRemoveOption);

        if (removeOption == 1) {
            listVehicle = removeElementByIndex(listVehicle, option - 1);
            *modified = 1;
        }

    }
    return listVehicle;
}

ListElem sortByTotalKms(ListElem listVehicle)// , ListElem sortedList)
{
    ListElem sortedList = NULL;
    Vehicle v;

    while (listVehicle != NULL) {
        v = (Vehicle)listVehicle->data;        
        sortedList = addItemOrderedIterative(sortedList, v, &compareTotalKms);
        listVehicle = listVehicle->next;
    }

    return sortedList;
}

float percentageInUse(ListElem listVehicle)
{
    int total = 0, inUse = 0;
    Vehicle v;

    while (listVehicle != NULL) {
        v = (Vehicle)listVehicle->data;
        if (v->inUse) {
            inUse++;
        }
        total++;
        listVehicle = listVehicle->next;
    }

    if (total == 0) {
        return 0.0;
    }

    return ((float)inUse / total) * 100;
}

float averageAutonomy(ListElem listVehicle)
{
    int count = 0;
    float sum = 0;
    Vehicle v;

    while (listVehicle != NULL) {
        v = (Vehicle)listVehicle->data;
        sum += v->autonomy;
        count++;
        listVehicle = listVehicle->next;
    }

    if (count == 0) {
        return 0.0;
    }

    return sum / count;
}

void refillBatteryAndAutonomy(Vehicle vehicle) {
    vehicle->autonomy = (vehicle->autonomy / vehicle->battery) * 100.0;
    vehicle->battery = 100.0;
}

ListElem filterVehicleByBattery(ListElem listVehicle)
{
    ListElem filteredList = NULL;
    Vehicle v;

    while (listVehicle != NULL) {
        v = (Vehicle)listVehicle->data;
        if (v->battery < 50) {
            filteredList = addItemLastIterative(filteredList, v);
        }
        listVehicle = listVehicle->next;
    }

    return filteredList;
}

ListElem filterVehicleByGeo(ListElem listVehicle, char* geolocation) {
    ListElem filteredList = NULL;
    Vehicle v;

    while (listVehicle != NULL) {
        v = (Vehicle)listVehicle->data;
        if (strcmp(v->geolocation, geolocation) == 0) {
            filteredList = addItemLastIterative(filteredList, v);
        }
        listVehicle = listVehicle->next;
    }

    return filteredList;
}

ListElem filterVehicleByInUse(ListElem listVehicle) {
    ListElem filteredList = NULL;
    Vehicle v;

    while (listVehicle != NULL) {
        v = (Vehicle)listVehicle->data;
        if (v->inUse == 0) {
            filteredList = addItemLastIterative(filteredList, v);
        }
        listVehicle = listVehicle->next;
    }

    return filteredList;
}

Vehicle getVehicleByTypeAndCode(ListElem listVehicle, char* type, int code) {
    ListElem currElem = listVehicle;
    while (currElem != NULL) {
        Vehicle currVehicle = (Vehicle)currElem->data;
        if (strcmp(currVehicle->type, type) == 0 && currVehicle->code == code) {
            return currVehicle;
        }
        currElem = currElem->next;
    }
    return NULL;
}

Vehicle getVehicleByType(ListElem listVehicle, char* type) {
    ListElem currElem = listVehicle;
    while (currElem != NULL) {
        Vehicle currVehicle = (Vehicle)currElem->data;
        if (strcmp(currVehicle->type, type)) {
            return currVehicle;
        }
        currElem = currElem->next;
    }
    return NULL;
}

ListElem editVehicle(ListElem listVehicle, int* modified, Vehicle* v, char* oldGeolocation)
{
    showListIterative(listVehicle, &showVehicle);

    printf(" Insira o indice do veiculo a alterar: ");
    char auxOption[10];
    scanf(" %[^\n]%*c", auxOption);

    int option = 0;
    if (isInt(auxOption))
        option = stringToInt(auxOption);

    if ((option != 0) && (option <= listLength(listVehicle)))
    {
        ListElem auxElem = obtainElementPosition(listVehicle, option - 1);
        *v = (Vehicle)auxElem->data;
        strcpy(oldGeolocation, (*v)->geolocation);

        clrscr();
        showVehicle(*v);

        printf(" Insira o indice do parametro a alterar: ");
        char auxLine[10];
        scanf(" %[^\n]%*c", auxLine);

        int optionLine = 0;
        if (isInt(auxLine))
            optionLine = stringToInt(auxLine);

        char edit[50];
        int editInt = -1;
        float editFloat = -1;

        switch (optionLine) {
        case 1:
            printf(" Insira o novo modelo: ");
            scanf(" %[^\n]%*c", edit);
            for (int i = 0; edit[i] != '\0'; i++)
            {
                if ((edit[i] >= 'a' && edit[i] <= 'z') || edit[i] == ' ' || (edit[i] >= 'A' && edit[i] <= 'Z'))
                    continue;
                else
                    return listVehicle;
            }
            if (!vehicleExists((*v)->code, edit, listVehicle))
            {
                changeVType(*v, edit);
                *modified = 1;
            }
            break;
        case 2:
            printf(" Insira o novo identificador: ");
            char auxCode[10];
            scanf(" %[^\n]%*c", auxCode);

            if (isInt(auxCode))
                editInt = stringToInt(auxCode);

            if (editInt > 0 && !vehicleExists(editInt, (*v)->type, listVehicle))
            {
                changeVCode(*v, editInt);
                *modified = 1;
            }
            break;
        case 3:
            printf(" Insira a nova geolocalizacao: ");
            scanf(" %[^\n]%*c", edit);
            if (validGeolocation(edit))
            {
                changeVGeolocation(*v, edit);
                *modified = 1;
            }
            break;
        case 4:
            printf(" Insira o novo peso: ");
            char auxWeight[10];
            scanf(" %[^\n]%*c", auxWeight);

            if (isInt(auxWeight) || isFloat(auxWeight))
                editFloat = stringToFloat(auxWeight);

            if ((editFloat >= 0) && (editFloat <= 9999))
            {
                changeVWeight(*v, editFloat);
                *modified = 1;
            }
            break;
        case 5:
            printf(" Insira o novo nivel de bateria: ");
            char auxBattery[10];
            scanf(" %[^\n]%*c", auxBattery);

            if (isInt(auxBattery) || isFloat(auxBattery))
                editFloat = stringToFloat(auxBattery);

            if ((editFloat >= 0) && (editFloat <= 100))
            {
                changeVBattery(*v, editFloat);
                *modified = 1;
            }            
            break;
        case 6:
            printf(" Insira o novo nivel de autonomia: ");
            char auxAutonomy[10];
            scanf(" %[^\n]%*c", auxAutonomy);

            if (isInt(auxAutonomy) || isFloat(auxAutonomy))
                editFloat = stringToFloat(auxAutonomy);

            if (editFloat > 0)
            {
                ListElem auxHead = removeElementByIndex(listVehicle, option - 1);
                listVehicle = auxHead;
                changeVAutonomy(*v, editFloat);
                listVehicle = addItemOrderedIterative(listVehicle, (void*)(*v), &compareAutonomy);
                *modified = 1;
            }          
            break;
        case 7:
            printf(" Insira o novo custo por hora: ");
            char auxCosthour[10];
            scanf(" %[^\n]%*c", auxCosthour);

            if (isInt(auxCosthour) || isFloat(auxCosthour))
                editFloat = stringToFloat(auxCosthour);
            if (editFloat > 0)
            {
                changeVCosthour(*v, editFloat);
                *modified = 1;
            }
            break;
        case 8:
            printf(" Insira o novo custo por km: ");
            char auxCostkm[10];
            scanf(" %[^\n]%*c", auxCostkm);

            if (isInt(auxCostkm))
                editFloat = stringToInt(auxCostkm);
            if (editFloat > 0)
            {
                changeVCostkm(*v, editFloat);
                *modified = 1;
            }
            break;
        case 9:
            printf(" Insira o novo estado de aluger: ");
            char auxRentState[10];
            scanf(" %[^\n]%*c", auxRentState);

            if (isInt(auxRentState))
                editInt = stringToInt(auxRentState);

            if (editInt == 0 || editInt == 1)
            {
                changeVstate(*v, editInt);
                *modified = 1;   
            }
            break;
        case 10:
            printf(" Insira o novo total de kms: ");
            char auxTotalkm[10];
            scanf(" %[^\n]%*c", auxTotalkm);

            if (isInt(auxTotalkm))
                editFloat = stringToInt(auxTotalkm);

            if (editFloat > 0)
            {
                changeVkms(*v, editFloat);
                *modified = 1;
            }        
            break;
        default:
            break;
        }
        return listVehicle;
    }
    return listVehicle;
}

int vehicleExists(int code, char* type, ListElem listVehicles) {
    while (listVehicles != NULL) {
        Vehicle vehicle = (Vehicle)listVehicles->data;
        if (code == vehicle->code && strcmp(type, vehicle->type) == 0) {
            return 1;
        }
        listVehicles = listVehicles->next;
    }
    return 0;
}

ListElem registerVehicle(ListElem listVehicle, int* modified, Vehicle v) {

    clrscr();

    showtime();

    signnewuser();

    fflush(stdin);

    printf("\n Insira o modelo do veiculo: ");
    scanf(" %[^\n]%*c", v->type);

    for (int i = 0; v->type[i] != '\0'; i++)
    {
        if ((v->type[i] >= 'a' && v->type[i] <= 'z') || v->type[i] == ' ' || (v->type[i] >= 'A' && v->type[i] <= 'Z'))
            continue;
        else
        {
            free(v);
            return listVehicle;
        }
            
    }

    printf("\n Insira o codigo do veiculo: ");
    char auxCode[10];
    scanf(" %[^\n]%*c", auxCode);

    if (isInt(auxCode))
        v->code = stringToInt(auxCode);
    else
    {
        free(v);
        return listVehicle;
    }
    if (v->code <= 0)
    {
        free(v);
        return listVehicle;
    }

    if (vehicleExists(v->code, v->type, listVehicle))
    {
        free(v);
        return listVehicle;
    }

    printf("\n Insira a localizacao: ");
    scanf(" %[^\n]%*c", v->geolocation);

    if (!validGeolocation(v->geolocation))
    {
        free(v);
        return listVehicle;
    }

    printf("\n Insira o peso do veiculo: ");
    char auxWeight[10];
    scanf(" %[^\n]%*c", auxWeight);

    if (isInt(auxWeight) || isFloat(auxWeight))
        v->weight = stringToFloat(auxWeight);
    else
    {
        free(v);
        return listVehicle;
    }

    if (v->weight <= 0)
    {
        free(v);
        return listVehicle;
    }

    printf("\n Insira a bateria do veiculo: ");
    char auxBattery[10];
    scanf(" %[^\n]%*c", auxBattery);

    if (isInt(auxBattery) || isFloat(auxBattery))
        v->battery = stringToFloat(auxBattery);
    else
    {
        free(v);
        return listVehicle;
    }

    if ((v->battery <= 0) || (v->battery > 100))
    {
        free(v);
        return listVehicle;
    }

    printf("\n Insira a autonima do veiculo: ");
    char auxAutonomy[10];
    scanf(" %[^\n]%*c", auxAutonomy);

    if (isInt(auxAutonomy) || isFloat(auxAutonomy))
        v->autonomy = stringToFloat(auxAutonomy);
    else
    {
        free(v);
        return listVehicle;
    }
    if (v->autonomy <= 0)
    {
        free(v);
        return listVehicle;
    }

    printf("\n Insira o custo por hora do veiculo: ");
    char auxCosthour[10];
    scanf(" %[^\n]%*c", auxCosthour);

    if (isInt(auxCosthour) || isFloat(auxCosthour))
        v->costhour = stringToFloat(auxCosthour);
    else
    {
        free(v);
        return listVehicle;
    }
    if (v->costhour <= 0)
    {
        free(v);
        return listVehicle;
    }

    printf("\n Insira o custo por km do veiculo: ");
    char auxCostkm[10];
    scanf(" %[^\n]%*c", auxCostkm);

    if (isInt(auxCostkm))
        v->costkm = stringToInt(auxCostkm);
    else
    {
        free(v);
        return listVehicle;
    }
    if (v->costkm <= 0)
    {
        free(v);
        return listVehicle;
    }

    v->inUse = 0;
    v->totalkms = 0;

    listVehicle = addItemOrderedIterative(listVehicle, (void*)v, &compareAutonomy);

    printf("\n");
    signupok();
    wait();
    *modified = 1;
    return listVehicle;
}
