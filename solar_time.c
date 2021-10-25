/* solar_time.c -- Print local solar time to stdout. */

#include <stdio.h>
#include <time.h>
#include <math.h>

/* Output format. */
#define SOLAR_FORMAT ("%H:%M:%S")

/* Longitude of Paris Time Zone reference meridian, in degrees. */
#define REF_LON (15.0)

/* Longitude of location. */
//#define LON (2.3522) // Paris
#define LON (8.6821) // FfM

/* Time conversion factors */
#define DEGREE_IN_TIME_SECONDS (240)
#define HOUR_IN_SECONDS (3600)
#define LEAP_YEAR_IN_DAYS (365.25)
#define YEAR_IN_DAYS (365)

/* Coefficients of equation of time expansion, in hours. */
#define A0 (2.0870e-4)
#define A1 (9.2869e-3)
#define B1 (-1.2229e-1)
#define A2 (-5.2258e-2)
#define B2 (-1.5698e-1)
#define A3 (-1.3077e-3)
#define B3 (-5.1602e-3)
#define A4 (-2.1867e-3)
#define B4 (-2.9823e-3)
#define A5 (-1.5100e-4)
#define B5 (-2.3463e-4)

/**
 ** Number of the day in leap-year cycle (of 4 years):
 **      1 for january 1st of leap year,
 **      1461 for december 31st of last year of cycle.
 **
 ** WARNING: here we consider that leap years are those divisible by 4,
 **       which is valid from 2000 to 2100.
 **
 **/
int leap_cycle_day(struct tm);

/**
 ** Equation of time (i.e. delay between the real solar time and the mean
 ** solar time) in hours. It is calculated as a trigonometric series of the
 ** year fraction (no dimensions).
 **
 **/
float eot_h(float year_fraction);

/**
 ** Main function, does calculations starting from current time
 ** and prints local solar time to stdout.
 **
 **/
int main(int argc, char** argv) {
    char buff[20];
     /* Current equation of time in hours. */
    float current_eot_h;
    /* Current fraction of the year in leap-year cycle. */
    float year_fraction;
    /* Clock time and solar time in seconds. */
    time_t clock_s;
    time_t solar_s;
    struct tm clock_data;

    /* Determine clock time seconds and corresponding tm struct. */
    clock_s = time(0);
    clock_data = *localtime(&clock_s);

    /* Determine the fraction of the year and then equation of time. */
    year_fraction = 2 * M_PI * leap_cycle_day(clock_data) / LEAP_YEAR_IN_DAYS;
    current_eot_h = eot_h(year_fraction);

    /* Calculate local solar time in seconds. This compensates for location. */
    solar_s = clock_s + (current_eot_h * HOUR_IN_SECONDS
                      - (REF_LON - LON) * DEGREE_IN_TIME_SECONDS
                      - clock_data.tm_isdst * HOUR_IN_SECONDS);

    /* Convert back to tm struct and output to stdout. */
    strftime(buff, 20, SOLAR_FORMAT, localtime(&solar_s));
    printf("%s\n", buff);
    return 0;
}

int leap_cycle_day(struct tm time_s) {
        return (1 + time_s.tm_yday + YEAR_IN_DAYS * (time_s.tm_year % 4));
}

float eot_h(float year_fraction){
    return (A0
          + A1 * cos(year_fraction)       + B1 * sin(year_fraction)
          + A2 * cos(2.0 * year_fraction) + B2 * sin(2.0 * year_fraction)
          + A3 * cos(3.0 * year_fraction) + B3 * sin(3.0 * year_fraction)
          + A4 * cos(4.0 * year_fraction) + B4 * sin(4.0 * year_fraction)
          + A5 * cos(5.0 * year_fraction) + B5 * sin(5.0 * year_fraction));

}
