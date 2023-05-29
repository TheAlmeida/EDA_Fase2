#include "history.h"

void showHistory(void* data)
{
    History h = (History)data;
    if (h != NULL)
        printf(" Cliente: %s\n"
            " Veiculo: %s\n"
            " Origem: %s\n"
            " Destino: %s\n"
            " Distancia: %.2f\n"
            " Duracao: %.2f\n"
            " Custo: %.2f euros\n",
            h->user, h->transport, h->start, h->finish, h->distance, h->duration, h->cost);
    printf("\n");
}

ListElem loadDataHistory(ListElem listHistory)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        error();

    char history_bin[500];
    strcpy(history_bin, cwd);
    strcat(history_bin, "\\history.bin");

    FILE* historyFile = fopen(history_bin, "rb");
    if (historyFile == NULL) {
        error();
    }

    History h = NULL;
    while (1) {
        h = (History)malloc(sizeof(struct datahistory));
        if (fscanf(historyFile, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%f|%f|%f\n",
            h->user, h->transport, h->start, h->finish, &(h->distance),
            &(h->duration), &(h->cost)) == EOF) {
            free(h);
            break;
        }
        listHistory = addItemLastIterative(listHistory, h);
    }

    fclose(historyFile);
    return listHistory;

}

void storeDataHistory(ListElem listH)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        error();
    }

    char history_bin[500];
    strcpy(history_bin, cwd);
    strcat(history_bin, "\\history.bin");

    FILE* file = fopen(history_bin, "wb"); // Open the file in write binary mode
    if (file == NULL) {
        printf("Error opening file %s.\n", history_bin);
        return;
    }

    // Iterate through the list of history records and write their information to the file
    ListElem current = listH;
    while (current != NULL) {
        History history = (History)current->data;
        fprintf(file, "%s|%s|%s|%s|%.2f|%.2f|%.2f\n",
            history->user, history->transport, history->start, history->finish, history->distance, history->duration, history->cost);
        current = current->next;
    }

    fclose(file);
}

ListElem filterHistoryByUser(ListElem originalList, char* userToFilter)
{
    ListElem filteredList = NULL;
    ListElem current = originalList;

    while (current != NULL) {
        History history = (History)current->data;
        if (strcmp(history->user, userToFilter) == 0) {
            ListElem newElem = (ListElem)malloc(sizeof(SListElem));
            newElem->data = current->data;
            newElem->next = filteredList;
            filteredList = newElem;
        }
        current = current->next;
    }

    return filteredList;
}

ListElem filterHistoryByTransport(ListElem originalList, char* transportToFilter)
{
    ListElem filteredList = NULL;
    ListElem current = originalList;

    char* last_space = strrchr(transportToFilter, ' ');

    if (last_space != NULL) {
        *last_space = '_';
    }

    while (current != NULL) {
        History history = (History)current->data;
        if (strcmp(history->transport, transportToFilter) == 0) {
            ListElem newElem = (ListElem)malloc(sizeof(SListElem));
            newElem->data = current->data;
            newElem->next = filteredList;
            filteredList = newElem;
        }
        current = current->next;
    }

    return filteredList;
}

ListElem filterHistoryByType(ListElem originalList, char* typeToFilter)
{
    ListElem filteredList = NULL;
    ListElem current = originalList;

    while (current != NULL) {
        History history = (History)current->data;
        char* transport = history->transport;
        char* underscorePos = strchr(transport, '_');
        if (underscorePos != NULL) {
            *underscorePos = '\0';  // Null-terminate the string at the underscore
        }
        if (strcmp(transport, typeToFilter) == 0) {
            ListElem newElem = (ListElem)malloc(sizeof(SListElem));
            newElem->data = current->data;
            newElem->next = filteredList;
            filteredList = newElem;
        }
        if (underscorePos != NULL) {
            *underscorePos = '_';  // Restore the original string
        }
        current = current->next;
    }

    return filteredList;
}

ListElem filterHistory(ListElem listHistory, int by)
{
    adminhistory();

    if (by == 0)
        printf(" Insira o modelo do veiculo a utilizar como filtro: ");
    else if (by == 1)
        printf(" Insira o modelo e o codigo (1 espaco entre cada) do veiculo a utilizar como filtro: ");
    else
        printf(" Insira o username do cliente a utilizar como filtro: ");

    char filter[60];
    scanf(" %[^\n]", filter);

    ListElem listHfiltered = NULL;

    if (by == 0)
        listHfiltered = filterHistoryByType(listHistory, filter);
    else if (by == 1)
        listHfiltered = filterHistoryByTransport(listHistory, filter);
    else
        listHfiltered = filterHistoryByUser(listHistory, filter);

    return listHfiltered;
}

float averageDistance(ListElem listHistory)
{
    int count = 0;
    float sum = 0;
    History h;

    while (listHistory != NULL) {
        h = (History)listHistory->data;
        sum += h->distance;
        count++;
        listHistory = listHistory->next;
    }

    if (count == 0) {
        return 0.0;
    }

    return sum / count;
}

float averageDuration(ListElem listHistory)
{
    int count = 0;
    float sum = 0;
    History h;

    while (listHistory != NULL) {
        h = (History)listHistory->data;
        sum += h->duration;
        count++;
        listHistory = listHistory->next;
    }

    if (count == 0) {
        return 0.0;
    }

    return sum / count;
}

float averageCost(ListElem listHistory)
{
    int count = 0;
    float sum = 0;
    History h;

    while (listHistory != NULL) {
        h = (History)listHistory->data;
        sum += h->cost;
        count++;
        listHistory = listHistory->next;
    }

    if (count == 0) {
        return 0.0;
    }

    return sum / count;
}

char* mostCommonTransport(ListElem listHistory) {
    int maxCount = 0;
    char mostCommon[50] = "";

    // Traverse the list and count the occurrences of each transport string
    while (listHistory != NULL) {
        History h = (History)listHistory->data;
        // Replace the underscore in the transport string with a space
        char* underscorePos = strchr(h->transport, '_');
        if (underscorePos != NULL) {
            *underscorePos = ' ';
        }
        // Check if the current transport string has occurred more frequently than the current most common
        int count = 1;
        ListElem current = listHistory->next;
        while (current != NULL) {
            History h2 = (History)current->data;
            char* underscorePos2 = strchr(h2->transport, '_');
            if (underscorePos2 != NULL) {
                *underscorePos2 = ' ';
            }
            if (strcmp(h->transport, h2->transport) == 0) {
                count++;
            }
            current = current->next;
        }
        if (count > maxCount) {
            maxCount = count;
            strcpy(mostCommon, h->transport);
        }
        listHistory = listHistory->next;
    }

    // Replace the underscore in the most common transport string with a space before returning it
    char* underscorePos = strchr(mostCommon, '_');
    if (underscorePos != NULL) {
        *underscorePos = ' ';
    }

    return mostCommon;
}

void historyClientStats(ListElem listHistory, char* username)
{
    ListElem listHfiltered = filterHistoryByUser(listHistory, username);
    if (listHfiltered != NULL)
    {
        averageclientstats();

        float avgDistance = averageDistance(listHfiltered);
        printf(" A distancia media das suas viagens e de: %.2f kms\n\n", avgDistance);
        float avgDuration = averageDuration(listHfiltered);
        printf(" A duracao media das suas viagens e de: %.2f horas\n\n", avgDuration);
        float avgCost = averageCost(listHfiltered);
        printf(" O custo medio das suas viagens e de: %.2f euros\n\n", avgCost);
        char mostUsedVehicle[50];
        strcpy(mostUsedVehicle, mostCommonTransport(listHfiltered));
        printf(" O seu veiculo mais utilizado e: %s\n\n", mostUsedVehicle);

        historyclientstats();

        showListIterative(listHfiltered, &showHistory);       
    }
    else
        printf(" Ainda nao efetuou nenhuma viagem com os nossos veiculos!");
}
