#include "client.h"

void showClient(void* data)
{
    Client c = (Client)data;
    if (c != NULL)
    {
        printf(" Username: %s\n"
            " Password: %s\n"
            " Name: %s\n"
            " Email: %s\n"
            " Cellphone number: %d\n"
            " NIF: %d\n"
            " Age: %d anos\n"
            " Address: %s\n"
            " Total trips: %d\n"
            " Total distance travelled: %.2f kms\n"
            " Saldo: %.2f euros\n",
            c->username, c->password, c->name, c->email, c->cellphone,
            c->nif, c->age, c->address, c->totaltrips, c->totalkms, c->balance);
        //for (int i = 0; i < 5; i++)
            //printf("%s: %.2f euros\n", c->bankaccount[i].naccount, c->bankaccount[i].balance);
        printf("\n");
    }
}

ListElem loadDataClients(ListElem listClients)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        error();

    char clients_bin[500];
    strcpy(clients_bin, cwd);
    strcat(clients_bin, "\\clients.bin");

    FILE* clientsFile = fopen(clients_bin, "rb");
    if (clientsFile == NULL) {
        error();
    }

    Client c = NULL;
    while (1) {
        c = (Client)malloc(sizeof(struct dataclient));
        if (fscanf(clientsFile, "%19[^|]|%49[^|]|%49[^|]|%49[^|]|%d|%d|%49[^|]|%f|%d|%d|%f\n",
            c->username, c->password, c->name, c->email,
            &(c->cellphone), &(c->age), c->address, &(c->totalkms),
            &(c->nif), &(c->totaltrips), &(c->balance)) == EOF) {
            free(c);
            break;
        }
        listClients = addItemOrderedIterative(listClients, c, &compareNamesClients);
    }

    fclose(clientsFile);
    return listClients;
}

void storeDataClients(ListElem listC) {

    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        error(); //Espera input do user
        //registerClient(registerFromAdmin);
    }

    char clients_bin[500];
    strcpy(clients_bin, cwd);
    strcat(clients_bin, "\\clients.bin");

    FILE* file = fopen(clients_bin, "wb"); // open the file in write binary mode
    if (file == NULL) {
        printf("Error opening file %s.\n", clients_bin);
        return;
    }

    // iterate through the list of clients and write their information to the file
    ListElem current = listC;
    while (current != NULL) {
        Client client = (Client)current->data;
        fprintf(file, "%s|%s|%s|%s|%d|%d|%s|%.2f|%d|%d|%.2f\n",
            client->username, client->password, client->name, client->email, client->cellphone, client->age,
            client->address, client->totalkms, client->nif, client->totaltrips, client->balance);
        current = current->next;
    }

    fclose(file); // close the file
}

int compareNamesClients(void* data1, void* data2)
{
    Client d1 = (Client)data1;
    Client d2 = (Client)data2;
    return (strcmp(d1->username, d2->username));
}

int compareTrips(void* data1, void* data2)
{
    Client d1 = (Client)data1;
    Client d2 = (Client)data2;
    if (d1->totaltrips < d2->totaltrips)
        return (1);
    else if (d1->totaltrips > d2->totaltrips)
        return (-1);
    else
        return (0);
}

void changeCUsername(Client client, char* newUsername)
{
    strcpy(client->username, newUsername);
}

void changeCPassword(Client client, char* newPassword)
{
    strcpy(client->password, newPassword);
}

void changeCName(Client client, char* newName)
{
    strcpy(client->name, newName);
}

void changeCEmail(Client client, char* newEmail)
{
    strcpy(client->email, newEmail);
}

void changeCCellphone(Client client, int newCellphone)
{
    client->cellphone = newCellphone;
}

void changeCAge(Client client, int newAge)
{
    client->age = newAge;
}

void changeCAddress(Client client, char* newAddress)
{
    strcpy(client->address, newAddress);
}

void changeCTotalKms(Client client, float newTotalKms)
{
    client->totalkms = newTotalKms;
}

void changeCBalance(Client client, float newBalance)
{
    client->balance = newBalance;
}

void changeCNif(Client client, int newNif)
{
    client->nif = newNif;
}

void changeCTotalTrips(Client client, int newTotalTrips)
{
    client->totaltrips = newTotalTrips;
}

ListElem removeClient(ListElem listClient, int* modified) 
{
    showListIterative(listClient, &showClient);

    printf(" Insira o indice do cliente a remover: ");
    char auxOption[10];
    scanf(" %[^\n]%*c", auxOption);

    int option = 0;
    if (isInt(auxOption))
        option = stringToInt(auxOption);

    if ((option != 0) && (option <= listLength(listClient)))
    {
        ListElem auxElem = obtainElementPosition(listClient, option - 1);
        Client client = (Client)auxElem->data;

        clrscr();
        showClient(client);

        printf(" Deseja remover o cliente acima? (1- Sim / 2- Nao): ");

        int removeOption = 0;
        char auxRemoveOption[10];
        scanf(" %[^\n]%*c", auxRemoveOption);

        if (isInt(auxRemoveOption))
            removeOption = stringToInt(auxRemoveOption);

        if (removeOption == 1) {
            listClient = removeElementByIndex(listClient, option - 1);
            free(client);
            *modified = 1;
        }
        else
            errornotvalid();

    }
    else
        errornotvalid();

    return listClient;
}

ListElem sortByTrips(ListElem listClient)// , ListElem sortedList)
{
    ListElem sortedList = NULL;
    Client c;

    while (listClient != NULL) {
        c = (Client)listClient->data;
        sortedList = addItemOrderedIterative(sortedList, c, &compareTrips);
        listClient = listClient->next;
    }

    return sortedList;
}

void findClientWithMostTrips(ListElem listClient, char* username, int* totaltrips)
{
    if (listClient == NULL)
        return;

    Client maxClient = (Client)(listClient->data);
    int maxTrips = maxClient->totaltrips;

    ListElem c;
    for (c = listClient->next; c != NULL; c = c->next)
    {
        Client client = (Client)(c->data);
        if (client->totaltrips > maxTrips)
        {
            maxClient = client;
            maxTrips = client->totaltrips;
        }
    }

    strncpy(username, maxClient->username, 20);
    username[20] = '\0'; // add null terminator
    *totaltrips = maxTrips;
}

float calculateAverageAge(ListElem listClient)
{
    if (listClient == NULL)
        return 0.0;

    int sumAge = 0;
    int numClients = 0;

    ListElem client;
    for (client = listClient; client != NULL; client = client->next)
    {
        sumAge += ((Client)(client->data))->age;
        numClients++;
    }

    return (float)sumAge / numClients;
}

ListElem editClient(ListElem listClient, int* modified)
{
    showListIterative(listClient, &showClient);

    printf(" Insira o indice do cliente a alterar: ");
    char auxOption[10];
    scanf(" %[^\n]%*c", auxOption);

    int option = 0;
    if (isInt(auxOption))
        option = stringToInt(auxOption);

    if ((option != 0) && (option <= listLength(listClient)))
    {
        ListElem auxElem = obtainElementPosition(listClient, option - 1);
        Client client = (Client)auxElem->data;

        clrscr();
        showClient(client);

        printf(" Insira o indice do parametro a alterar: ");
        char auxLine[10];
        scanf(" %[^\n]%*c", auxLine);

        int optionLine = 0;
        if (isInt(auxLine))
            optionLine = stringToInt(auxLine);

        char edit[50];
        int editInt = 0;
        float editFloat = 0;

        switch (optionLine) {
        case 1:
            printf(" Insira um novo nome de utilizador: ");
            scanf(" %[^\n]%*c", edit);

            ListElem auxHead = removeElementByIndex(listClient, option - 1);
            listClient = auxHead;
            changeCUsername(client, edit);
            listClient = addItemOrderedIterative(listClient, (void*)client, &compareNamesClients);
            *modified = 1;
            break;
        case 2:
            printf(" Insira uma nova password: ");
            scanf(" %[^\n]%*c", edit);
            changeCPassword(client, edit);
            *modified = 1;
            break;
        case 3:
            printf(" Insira um novo nome: ");
            scanf(" %[^\n]%*c", edit);
            changeCName(client, edit);
            *modified = 1;
            break;
        case 4:
            printf(" Insira um novo email: ");
            scanf(" %[^\n]%*c", edit);
            changeCEmail(client, edit);
            *modified = 1;
            break;
        case 5:
            printf(" Insira um novo numero de telemovel: ");
            scanf("%d", &editInt);
            changeCCellphone(client, editInt);
            *modified = 1;
            break;
        case 6:
            printf(" Insira um novo NIF: ");
            scanf("%d", &editInt);
            changeCNif(client, editInt);
            *modified = 1;
            break;
        case 7:
            printf(" Insira uma nova idade: ");
            scanf("%d", &editInt);
            changeCAge(client, editInt);
            *modified = 1;
            break;
        case 8:
            printf(" Insira uma nova morada: ");
            scanf(" %[^\n]%*c", edit);
            changeCAddress(client, edit);
            *modified = 1;
            break;
        case 9:
            printf(" Insira um novo total de viagens: ");
            scanf("%d", &editInt);
            changeCTotalTrips(client, editInt);
            *modified = 1;
            break;
        case 10:
            printf(" Insira um novo total de kms: ");
            scanf("%f", &editFloat);
            changeCTotalKms(client, editFloat);
            *modified = 1;
            break;
        case 11:
            printf(" Insira um novo saldo: ");
            scanf("%f", &editFloat);
            changeCBalance(client, editFloat);
            *modified = 1;
            break;
        case 0:
            break;
        default:
            ;
        }
    }
    else
        errornotvalid();

    return listClient;
}

int usernameExists(char* username, ListElem listClient, ListElem listAdmin) {
    while (listClient != NULL) {
        Client client = (Client)listClient->data;
        if (strcmp(username, client->username) == 0) {
            return 1;
        }
        listClient = listClient->next;
    }
    while (listAdmin != NULL) {
        Admin admin = (Admin)listAdmin->data;
        if (strcmp(username, admin->username) == 0) {
            return 1;
        }
        listAdmin = listAdmin->next;
    }
    return 0;
}

ListElem registerClient(ListElem listClient, ListElem listAdmin, int* modified) {

    clrscr();

    showtime();

    signnewuser();

    fflush(stdin);

    Client c = (Client)malloc(sizeof(struct dataclient));

    printf("\n Insira o seu nome de utilizador: "); //NOME DE UTILIZADOR
    scanf(" %[^\n]", c->username);

    if (usernameExists(c->username, listClient, listAdmin)) {
        free(c);
        errorusernameexists();
        return listClient;
    }

    printf("\n Insira o seu nome: "); //NOME
    scanf(" %[^\n]%*c", c->name);

    int i;
    for (i = 0; c->name[i] != '\0'; i++) {
        if ((c->name[i] >= 'a' && c->name[i] <= 'z') || c->name[i] == ' ' || (c->name[i] >= 'A' && c->name[i] <= 'Z'))
            continue;
        else {
            printf("%d", i);
            errornotvalidinfo();
            return listClient;
        }
    }

    printf("\n Insira a sua morada: ");//MORADA
    scanf(" %[^\n]%*c", c->address);

    char auxAge[5];
    printf("\n Insira a sua idade: "); //IDADE
    scanf(" %[^\n]%*c", auxAge);

    if (isInt(auxAge)) c->age = stringToInt(auxAge);
    else {
        errornotvalidinfo();
        return listClient;
    }
    if (c->age <= 0) {
        error();
        return listClient;
    }

    printf("\n Insira o seu email: "); //EMAIL
    scanf(" %[^\n]%*c", c->email);

    int h;
    for (h = 0; c->email[h] != '\0'; h++) {
        if (c->email[h] == '@') break;
        else if (h == strlen(c->email)) {
            errornotvalidinfo();
            return listClient;
        }
        else continue;
    }

    printf("\n Insira o seu numero de contacto: "); //N?MERO
    char auxCellphone[20];
    scanf(" %[^\n]%*c", auxCellphone);

    if (isInt(auxCellphone)) c->cellphone = stringToInt(auxCellphone);
    else {
        errornotvalidinfo();
        return listClient;
    }
    if (c->cellphone <= 0) {
        errornotvalidinfo();
        return listClient;
    }

    printf("\n Insira o seu NIF: "); //IDADE
    char auxNif[20];
    scanf(" %[^\n]%*c", auxNif);

    if (isInt(auxNif)) c->nif = stringToInt(auxNif);
    else {
        errornotvalidinfo();
        return listClient;
    }
    if (c->nif <= 0) {
        errornotvalidinfo();
        return listClient;
    }

    printf("\n Insira a sua password: "); //PASSWORD
    scanf(" %[^\n]", c->password);

    printf("\n Confirme a sua password: "); //CONFIRMA??O DA PASSWORD
    char passcheck[50];
    scanf(" %[^\n]", passcheck);


    if (strcmp(c->password, passcheck) != 0) {
        errorpassword();
        return listClient;
    }

    c->totalkms = 0;
    c->totaltrips = 0;
    c->balance = 0;

    listClient = addItemOrderedIterative(listClient, (void*)c, &compareNamesClients);
    *modified = 1;
    
    printf("\n");
    signupok();
    wait(); //Espera input do user

    return listClient;
}

Client addBalance(Client c, int* modified)
{
    float amountToAdd = 0;
    int paymentMethod;
    char cardNumber[20];
    char expirationDate[6];
    char cvv[4];
    char paypalEmail[50];
    char cellphoneMBWway[10];

    printf(" Com quanto quer carregar a conta? ");
    scanf("%f", &amountToAdd);
    printf("\n Selecionou %.2f euros para adicionar a conta. Confirma? (S/N) ", amountToAdd);
    char confirmation;
    scanf(" %c", &confirmation);
    if (confirmation == 'S' || confirmation == 's') {
        printf("\n Otimo! Por favor indique o metodo de pagamento:");
        printf("\n 1. Cartao de credito\n 2. PayPal\n 3. MBway\n\n ");
        scanf("%d", &paymentMethod);

        switch (paymentMethod) {
        case 1:
            printf("\n Insira o numero do carta: ");
            scanf(" %s", cardNumber);
            if (strlen(cardNumber) != 16) {
                printf("\n Numero invalido.");
                break;
            }
            for (int i = 0; i < 16; i++) {
                if (!isdigit(cardNumber[i])) {
                    printf("\n Numero invalido.");
                    break;
                }
            }
            printf("\n Por favor insira a data de expiracao do cartao (MM/AA): ");
            scanf(" %s", expirationDate);
            if (strlen(expirationDate) != 5 || expirationDate[2] != '/') {
                printf("\n Data num formato invalido.");
                break;
            }
            for (int i = 0; i < 5; i++) {
                if (i != 2 && !isdigit(expirationDate[i])) {
                    printf("\n Data invalida.");
                    break;
                }
            }
            printf("\n Por favor insira o codigo CVV: ");
            scanf(" %s", cvv);
            if (strlen(cvv) != 3) {
                printf("\n Comprimento de CVV invalido.");
                break;
            }
            for (int i = 0; i < 3; i++) {
                if (!isdigit(cvv[i])) {
                    printf("\n CVV invalido.");
                    break;
                }
            }
            c->balance += amountToAdd;
            *modified = 1;
            
            printf("\n Obrigado, foram adicionados %s! %.2f euros. O seu novo saldo e %.2f euros.\n", c->username, amountToAdd, c->balance);
            wait();
            break;
        case 2:
            printf("\n Gostaria de utilizar o email %s para pagamento paypal? (S/N) ", c->email);
            scanf(" %c", &confirmation);
            if (confirmation == 'N' || confirmation == 'n') {
                printf("\n Por favor insira o email para efetuar o pagamento: ");
                scanf(" %s", paypalEmail);
            }
            printf("\n Carregue em qualquer tecla quando tiver concluido o pagamento na janela popup.\n");
            getch();

            c->balance += amountToAdd;
            *modified = 1;

            printf("\n Obrigado %s! Foram adicionados %.2f euros. O seu novo saldo e %.2f euros.\n", c->username, amountToAdd, c->balance);
            wait();
            break;
        case 3:
            printf("\n Gostaria de utilizar o numero de telemovel %d para pagamento MBway? (S/N) ", c->cellphone);
            scanf(" %c", &confirmation);
            if (confirmation == 'N' || confirmation == 'n') {
                printf("\n Por favor insira o numero de telemovel para efetuar o pagamento: ");
                scanf(" %s", cellphoneMBWway);
            }
            printf("\n Carregue em qualquer tecla quando tiver concluido o pagamento no seu telemovel.\n");
            getch();

            c->balance += amountToAdd;
            *modified = 1;

            printf("\n Obrigado %s! Foram adicionados %.2f euros. O seu novo saldo e %.2f euros.\n", c->username, amountToAdd, c->balance);
            wait();
            break;
        default:
            errornotvalid();
            break;
        }
    }
    else {
        printf("Transacao cancelada.\n");
    }

    return c;
}