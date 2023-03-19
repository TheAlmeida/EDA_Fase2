#include "admin.h"


void showAdmin(void* data)
{
    Admin a = (Admin)data;
    if (a != NULL)
    {
        printf(" Username: %s\n"
            " Password: %s\n"
            " Name: %s\n"
            " Email: %s\n",
            a->username, a->password, a->name, a->email);
        printf("\n");
    }
}

ListElem loadDataAdmins(ListElem listAdmins)
{
    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        error();

    char admins_bin[500];
    strcpy(admins_bin, cwd);
    strcat(admins_bin, "\\admins.bin");

    FILE* adminsFile = fopen(admins_bin, "rb");
    if (adminsFile == NULL) {
        error();
    }

    while (1) {
        Admin a = (Admin)malloc(sizeof(struct dataadmin));
        memset(a, 0, sizeof(struct dataadmin));
        if (fscanf(adminsFile, "%19[^|]|%49[^|]|%49[^|]|%49[^\n]\n",
            a->username, a->password, a->name, a->email) == EOF) {
            free(a);
            break;
        }
        listAdmins = addItemOrderedIterative(listAdmins, a, &compareNamesAdmins);
    }

    fclose(adminsFile);
    return listAdmins;
}

void storeDataAdmins(ListElem listA) {

    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        error();
    }

    char admins_bin[500];
    strcpy(admins_bin, cwd);
    strcat(admins_bin, "\\admins.bin");

    FILE* file = fopen(admins_bin, "wb"); // open the file in write binary mode
    if (file == NULL) {
        printf("Error opening file %s.\n", admins_bin);
        return;
    }

    // iterate through the list of admins and write their information to the file
    ListElem current = listA;
    while (current != NULL) {
        Admin admin = (Admin)current->data;
        fprintf(file, "%s|%s|%s|%s\n",
            admin->username, admin->password, admin->name, admin->email);
        current = current->next;
    }

    fclose(file); // close the file
}

int compareNamesAdmins(void* data1, void* data2)
{
    Admin d1 = (Admin)data1;
    Admin d2 = (Admin)data2;
    return (strcmp(d1->username, d2->username));
}

void changeAUsername(Admin admin, char* newUsername)
{
    strcpy(admin->username, newUsername);
}

void changeAPassword(Admin admin, char* newPassword)
{
    strcpy(admin->password, newPassword);
}

void changeAName(Admin admin, char* newName)
{
    strcpy(admin->name, newName);
}

void changeAEmail(Admin admin, char* newEmail)
{
    strcpy(admin->email, newEmail);
}

ListElem removeAdmin(ListElem listAdmin, int* modified)
{
    showListIterative(listAdmin, &showAdmin);

    printf(" Insira o indice do admin a ser removido: ");
    char auxOption[10];
    scanf(" %[^\n]%*c", auxOption);

    int option = 0;
    if (isInt(auxOption))
        option = stringToInt(auxOption);

    if ((option != 0) && (option <= listLength(listAdmin)))
    {
        ListElem auxElem = obtainElementPosition(listAdmin, option - 1);
        Admin admin = (Admin)auxElem->data;

        clrscr();
        showAdmin(admin);

        printf(" Deseja remover o admin acima? (1- Sim / 2- Nao): ");

        int removeOption = 0;
        char auxRemoveOption[10];
        scanf(" %[^\n]%*c", auxRemoveOption);

        if (isInt(auxRemoveOption))
            removeOption = stringToInt(auxRemoveOption);

        if (removeOption == 1) {
            listAdmin = removeElementByIndex(listAdmin, option - 1);
            free(admin);
            *modified = 1;
        }

    }

    return listAdmin;
}

ListElem editAdmin(ListElem listAdmin, int* modified)
{
    showListIterative(listAdmin, &showAdmin);

    printf(" Insira o indice do gestor a alterar: ");
    char auxOption[10];
    scanf(" %[^\n]%*c", auxOption);

    int option = 0;
    if (isInt(auxOption))
        option = stringToInt(auxOption);

    if ((option != 0) && (option <= listLength(listAdmin)))
    {
        ListElem auxElem = obtainElementPosition(listAdmin, option - 1);
        Admin admin = (Admin)auxElem->data;

        clrscr();
        showAdmin(admin);

        printf(" Insira o indice do parametro a alterar: ");
        char auxLine[10];
        scanf(" %[^\n]%*c", auxLine);

        int optionLine = 0;
        if (isInt(auxLine))
            optionLine = stringToInt(auxLine);
        //printf("\n");

        char edit[50];

        switch (optionLine) {
        case 1:
            printf(" Insira um novo nome de utilizador: ");
            scanf(" %[^\n]%*c", edit);

            ListElem auxHead = removeElementByIndex(listAdmin, option - 1);
            listAdmin = auxHead;
            changeAUsername(admin, edit);
            listAdmin = addItemOrderedIterative(listAdmin, (void*)admin, &compareNamesAdmins);
            *modified = 1;
            break;
        case 2:
            printf(" Insira uma nova password: ");
            scanf(" %[^\n]%*c", edit);
            changeAPassword(admin, edit);
            *modified = 1;
            break;
        case 3:
            printf(" Insira um novo nome: ");
            scanf(" %[^\n]%*c", edit);
            for (int i = 0; edit[i] != '\0'; i++)
            {
                if ((edit[i] >= 'a' && edit[i] <= 'z') || edit[i] == ' ' || (edit[i] >= 'A' && edit[i] <= 'Z'))
                    continue;
                else
                    return listAdmin;
            }
            changeAName(admin, edit);
            *modified = 1;
            break;
        case 4:
            printf(" Insira um novo email: ");
            scanf(" %[^\n]%*c", edit);
            changeAEmail(admin, edit);
            *modified = 1;
            break;
        default:
            break;
        }
        return listAdmin;
    }
    else

    return listAdmin;
}

ListElem registerAdmin(ListElem listAdmin, ListElem listClient, int* modified) {

    clrscr();

    showtime();

    signnewuser();

    fflush(stdin);

    Admin a = (Admin)malloc(sizeof(struct dataadmin));

    printf("\nInsira o seu nome de utilizador: "); //NOME DE UTILIZADOR
    scanf(" %[^\n]", a->username);

    if (usernameExists(a->username, listClient, listAdmin))
    {
        free(a);
        return listAdmin;
    }

    printf("\nInsira o seu nome: "); //NOME
    scanf(" %[^\n]%*c", a->name);

    for (int i = 0; a->name[i] != '\0'; i++)
    {
        if ((a->name[i] >= 'a' && a->name[i] <= 'z') || a->name[i] == ' ' || (a->name[i] >= 'A' && a->name[i] <= 'Z'))
            continue;
        else
        {
            printf("%d", i);
            free(a);
            return listAdmin;
        }
    }

    printf("\nInsira o seu email: "); //EMAIL
    scanf(" %[^\n]%*c", a->email);

    int h;
    for (h = 0; a->email[h] != '\0'; h++)
    {
        if (a->email[h] == '@') break;
        else if (h == strlen(a->email))
        {
            free(a);
            return listAdmin;
        }
        else continue;
    }

    printf("\nInsira a sua password: "); //PASSWORD
    scanf(" %[^\n]", a->password);

    printf("\nConfirme a sua password: "); //CONFIRMAO DA PASSWORD
    char passcheck[50];
    scanf(" %[^\n]", passcheck);


    if (strcmp(a->password, passcheck) != 0)
    {
        free(a);
        errorpassword();
        return listAdmin;
    }

    listAdmin = addItemOrderedIterative(listAdmin, (void*)a, &compareNamesAdmins);

    *modified = 1;

    printf("\n");
    signupok();
    wait(); //Espera input do user
    return listAdmin;
}
