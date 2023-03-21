#include "menu.h"

void clrscr()
{
    system("@cls||clear");
}

void wait()
{
    printf("\n Por favor, pressione alguma tecla para ser redirecionado ao menu anterior.");
    getch();
}

void error()
{
    printf("\n Foi encontrado um erro, por favor, tente novamente.");
    getch();
}

void errornotvalid()
{
    printf("\n A opcao introduzida nao e valida, por favor, tente novamente.");
    getch();
}

void errorusername()
{
    printf("\n Esse nome de utilizador nao e valido, por favor, tente novamente.");
    getch();
}

void errorpassword()
{
    printf("\n A password introduzida nao e valida, por favor, tente novamente.");
    getch();
}

void errorusernameexists()
{
    printf("\n Esse nome de utilizador ja existe, por favor, tente novamente.");
    getch();
}

void errornotvalidinfo()
{
    printf("\n A informacao introduzida nao e valida, por favor, tente novamente.");
    getch();
}

void novehiclesavailable()
{
    printf("\n Nao existem veiculos disponiveis.");
    getch();
}

void showtime()
{
    struct tm* timeinfo;
    time_t rawtime;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Hora local: %s\n", asctime(timeinfo));
    //O asctime() é uma função definida na livraria <time.h>.
    //Esta função retorna um apontador string que contem a informação guardada na struct tm.
    //Esta função retorna o local time definido pelo o sistema.
}

int isInt(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int isFloat(char* str) {
    int len = strlen(str);
    int dot_count = 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return 0;
        }
        if (str[i] == '.') {
            dot_count++;
        }
    }
    if (dot_count != 1) {
        return 0;
    }
    return 1;
}

int stringToInt(char* str) {
    int result = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        result = result * 10 + (str[i] - '0');
    }
    return result;
}

float stringToFloat(char* str) {
    return atof(str);
}

void menu()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                            CLEAN GO                                |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("| 1 - REGISTAR                                                       |\n");
    printf("| 2 - INICIAR SESSAO                                                 |\n");
    printf("| 3 - CONTACTE-NOS                                                   |\n");
    printf("| 0 - TERMINAR PROGRAMA                                              |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void signnewuser()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                       REGISTAR NOVO UTILIZADOR                     |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void signupok()
{
    printf("\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|                     REGISTO EFETUADO COM SUCESSO!                  |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void signin()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                            INICIAR SESSAO                          |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void signinok(char name[])
{
    char name_aux[50];
    strcpy(name_aux,name);

    printf("\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|                      SESSAO INICIADA COM SUCESSO!                  |\n");
    printf("                            BEM VINDO %s !                \n", _strupr(name_aux));
    printf("|--------------------------------------------------------------------|\n");
}

void clientmenu()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                            AREA CLIENTE                            |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|            **ALUGAR**            |            **SALDO**            |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|      1 - POR DISPONIBLIDADE      |         3 - CONSULTAR           |\n");
    printf("|      2 - E POR GEOCODIGO         |         4 - CARREGAR            |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("| 5 - HISTORICO E CONSUMOS                                           |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("| 0 - LOGOUT                                                         |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void availablevehicles()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                        VEICULOS DISPONIVEIS                        |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void clientbalance()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                               SALDO                                |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void averageclientstats()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                               MEDIAS                               |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void historyclientstats()
{
    printf("|--------------------------------------------------------------------|\n");
    printf("|                              HISTORICO                             |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void simulatemenu()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                        SIMULACAO DE VIAGEM                         |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void signinokadmin()
{
    printf("\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|                      SESSAO INICIADA COM SUCESSO!                  |\n");
    printf("|                           BEM VINDO ADMIN!                         |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void adminmenu()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                             MODO GESTOR                            |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|     **VEICULOS**     |     **CLIENTES**     |     **GESTORES**     |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|    1 - ADICIONAR     |    4 - ADICIONAR     |    7 - ADICIONAR     |\n");
    printf("|    2 - EDITAR        |    5 - EDITAR        |    8 - EDITAR        |\n");
    printf("|    3 - REMOVER       |    6 - REMOVER       |    9 - REMOVER       |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|                **HISTORICO**                |   **ESTATISTICAS**   |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|   10 - POR MODELO       12 - POR CLIENTE    |   14 - VEICULOS      |\n");
    printf("|   11 - POR VEICULO      13 - TODOS          |   15 - CLIENTES      |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|  0 - LOGOUT                                                        |\n");
    printf("|--------------------------------------------------------------------|\n");
}

void admincstats()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                        ESTATISTICAS CLIENTES                       |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void admincstatsbytrips()
{
    printf("|--------------------------------------------------------------------|\n");
    printf("|                       CLIENTES POR NRO VIAGENS                     |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void adminhistory()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                              HISTORICO                             |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void adminvstats()
{
    clrscr();
    showtime();

    printf("|--------------------------------------------------------------------|\n");
    printf("|                        ESTATISTICAS VEICULOS                       |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void adminvstatsbatteryunder50()
{
    printf("|--------------------------------------------------------------------|\n");
    printf("|                    VEICULOS ABAIXO 50%% BATERIA                     |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void adminvstatstotalkms()
{
    printf("|--------------------------------------------------------------------|\n");
    printf("|                        VEICULOS POR NRO KMS                        |\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("\n");
}

void contactus()
{
    clrscr();
    showtime();

    printf("|------------------------------------------------------------------|\n");
    printf("|        BRAGA        |        PORTO        |        LISBOA        |\n");
    printf("|                     |                     |                      |\n");
    printf("|                     |                     |                      |\n");
    printf("| Nro:                | Nro                 | Nro                  |\n");
    printf("| 253340965           | 223458652           | 214564753            |\n");
    printf("| Horario:            | Horario:            | Horario:             |\n");
    printf("| 2da 6ta 09h00-18h00 | 2da 6ta 09h00-18h00 | 2da 6ta 09h00-18h00  |\n");
    printf("| Morada:             | Morada:             | Morada:              |\n");
    printf("| Praca do Comercio   | R. da Bolsa         | Tv. Inglesinhos      |\n");
    printf("| 4700-370 Braga      | n19,4050-253 Porto  | n46A,1200-079 Lisboa |\n");
    printf("|                     |                     |                      |\n");
    printf("| 1 - Ver localizacao | 2 - Ver localizacao | 3 - Ver localizacao  |\n");
    printf("|------------------------------------------------------------------|\n");
    printf("|                                                    0 - RETROCEDER|\n");
    printf("|------------------------------------------------------------------|\n");
}

void endprogram()
{
    clrscr();
    showtime();

    printf("\n");
    printf("|--------------------------------------------------------------------|\n");
    printf("|                  PROGRAMA TERMINADO COM SUCESSO!                   |\n");
    printf("|--------------------------------------------------------------------|\n");
    exit(0);
}


