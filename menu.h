#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>

void clrscr();

void wait();

void error();

void errornotvalid();

void errorusername();

void errorpassword();

void errorusernameexists();

void errornotvalidinfo();

void novehiclesavailable();

void showtime();

int isInt(char* str);

int isFloat(char* str);

int stringToInt(char* str);

float stringToFloat(char* str);

void menu();

void signnewuser();

void signupok();

void signin();

void signinok(char name[]);

void clientmenu();

void availablevehicles();

void clientbalance();

void averageclientstats();

void historyclientstats();

void simulatemenu();

void signinokadmin();

void adminmenu();

void contactus();

void endprogram();
