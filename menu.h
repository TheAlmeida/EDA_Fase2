#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>

/// <summary>
/// Clearscreen command.
/// </summary>
void clrscr();

/// <summary>
/// Wait message and getch().
/// </summary>
void wait();

/// <summary>
/// Error message and getch().
/// </summary>
void error();

/// <summary>
/// Error message and getch().
/// </summary>
void errornotvalid();

/// <summary>
/// Error message and getch().
/// </summary>
void errorusername();

/// <summary>
/// Error message and getch().
/// </summary>
void errorpassword();

/// <summary>
/// Error message and getch().
/// </summary>
void errorusernameexists();

/// <summary>
/// Error message and getch().
/// </summary>
void errornotvalidinfo();

/// <summary>
/// Error message and getch().
/// </summary>
void novehiclesavailable();

/// <summary>
/// Prints time and date
/// </summary>
void showtime();

/// <summary>
/// Checks if passed string is an int.
/// </summary>
/// <param name="str">string to check.</param>
/// <returns>1 is str is int, 0 otherwise.</returns>
int isInt(char* str);

/// <summary>
/// Checks if passed string is a float.
/// </summary>
/// <param name="str">string to check.</param>
/// <returns>1 is str is float, 0 otherwise.</returns>
int isFloat(char* str);

/// <summary>
/// Converts input string into an int.
/// </summary>
/// <param name="str">String to convert.</param>
/// <returns>Converted input.</returns>
int stringToInt(char* str);

/// <summary>
/// Converts input string into a float.
/// </summary>
/// <param name="str">String to convert.</param>
/// <returns>Converted input.</returns>
float stringToFloat(char* str);

/// <summary>
/// Prints the greeting menu.
/// </summary>
void menu();

/// <summary>
/// Prints a sign new user header.
/// </summary>
void signnewuser();

/// <summary>
/// Prints a confirmation of signup header.
/// </summary>
void signupok();

/// <summary>
/// Prints login header.
/// </summary>
void signin();

/// <summary>
/// Prints a confirmation of login header.
/// </summary>
/// <param name="name">name of the client to print.</param>
void signinok(char name[]);

/// <summary>
/// Prints the client menu.
/// </summary>
void clientmenu();

/// <summary>
/// Prints a header for the available vehicles.
/// </summary>
void availablevehicles();

/// <summary>
/// Prints a header for the balance operations.
/// </summary>
void clientbalance();

/// <summary>
/// Prints a header for the average stats accessed by the user.
/// </summary>
void averageclientstats();

/// <summary>
/// Prints a header for the history accessed by the user.
/// </summary>
void historyclientstats();

/// <summary>
/// Prints a header for the simulation phase.
/// </summary>
void simulatemenu();

/// <summary>
/// Prints a confirmation of log in header for admins.
/// </summary>
void signinokadmin();

/// <summary>
/// Prints the admin menu.
/// </summary>
void adminmenu();

/// <summary>
/// Prints the contactus menu.
/// </summary>
void contactus();

/// <summary>
/// Prints an end program header and terminates the program.
/// </summary>
void endprogram();
