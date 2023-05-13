#pragma once
#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")
#define CURL_STATICLIB
#include "curl\curl.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define APIKEY "0NMQZEL0"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double latitude;
    double longitude;
} Coordinates;

double degreesToRadians(double degrees);

//Haversine formula implementation
double calculateDistance(double lat1, double lon1, double lat2, double lon2);

// Function to handle curl response
size_t writeCallback(void* contents, size_t size, size_t nmemb, char* output);

// Function to convert geolocation to coordinates
Coordinates geolocationToCoordinates(const char* location);

// Function to convert coordinates to geolocation
char* coordinatesToGeolocation(double latitude, double longitude);
