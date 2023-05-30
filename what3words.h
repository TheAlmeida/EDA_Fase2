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

//#define APIKEY "0NMQZEL0"
//#define APIKEY "5G4OATGL"
#define APIKEY "U1O07WRP"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/// <summary>
/// Struct representing coordinates.
/// </summary>
typedef struct {
    double latitude;   ///< Latitude value
    double longitude;  ///< Longitude value
} Coordinates;

/// <summary>
/// Converts degrees to radians.
/// </summary>
/// <param name="degrees">Degrees value.</param>
/// <returns>Radians value.</returns>
double degreesToRadians(double degrees);

/// <summary>
/// Calculates the distance between two points using the Haversine formula.
/// </summary>
/// <param name="lat1">Latitude of the first point.</param>
/// <param name="lon1">Longitude of the first point.</param>
/// <param name="lat2">Latitude of the second point.</param>
/// <param name="lon2">Longitude of the second point.</param>
/// <returns>Distance between the two points.</returns>
double calculateDistance(double lat1, double lon1, double lat2, double lon2);

/// <summary>
/// Callback function to handle the response from a curl request and write the contents to a buffer.
/// </summary>
/// <param name="contents">Pointer to the response contents.</param>
/// <param name="size">Size of each element.</param>
/// <param name="nmemb">Number of elements.</param>
/// <param name="buffer">Pointer to the buffer to write the contents.</param>
/// <returns>The total size of the response contents.</returns>
size_t writeCallback(void* contents, size_t size, size_t nmemb, char* output);

/// <summary>
/// Converts a geolocation to coordinates using an API.
/// </summary>
/// <param name="location">Geolocation string.</param>
/// <returns>Coordinates of the location, or {0.0, 0.0} if conversion fails or an error occurs.</returns>
Coordinates geolocationToCoordinates(const char* location);

/// <summary>
/// Converts coordinates to a geolocation using an API.
/// </summary>
/// <param name="latitude">Latitude value.</param>
/// <param name="longitude">Longitude value.</param>
/// <returns>Geolocation string, or NULL if conversion fails or an error occurs.</returns>
char* coordinatesToGeolocation(double latitude, double longitude);

/// <summary>
/// Checks if the given geolocation is valid (three words separated by two dots, e.g. just.like.this).
/// </summary>
/// <param name="geolocation">The geolocation string to validate.</param>
/// <returns>1 if the geolocation is valid, 0 otherwise.</returns>
int validGeolocation(char* geolocation);
