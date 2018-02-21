/* Print local solar time to stdout. */

#include <stdio.h>
#include <time.h>
#include <math.h>

#define SOLAR_FORMAT "%H:%M:%S" /* Output format. */

/* Longitude difference between Paris Observatory and Paris Time Zone
 * meridian, in degrees. */
#define LON 12.663457

/* Coefficients of equation of time expansion, in hours. */
#define A0 2.0870e-4
#define A1 9.2869e-3
#define B1 -1.2229e-1
#define A2 -5.2258e-2
#define B2 -1.5698e-1
#define A3 -1.3077e-3
#define B3 -5.1602e-3
#define A4 -2.1867e-3
#define B4 -2.9823e-3
#define A5 -1.5100e-4
#define B5 -2.3463e-4

/* Number of days in leap-year cycle (of 4 years):
 *      1 for january 1st of leap year,
 *      1461 for december 31st of last year of cycle.
 *
 * WARNING: here we consider that leap years are those divisible by 4,
 *          which is valid from 2000 to 2100. */
int leap_cycle_day(struct tm);

int main(int argc, char** argv) {
    char buff[20];
    float EOT_h; /* Equation of time in hours. */
    float trig_arg; /* Number of days in leap-year cycle. */
    time_t clock_s; /* Clock and solar time in seconds. */
    time_t solar_s;
    struct tm clock_data;

    /* Determine clock time seconds and struct. */
    clock_s = time(0);
    clock_data = *localtime(&clock_s);

    /* Determine the equation of time in seconds. */
    trig_arg = 2 * M_PI * leap_cycle_day(clock_data) / 365.25;

    EOT_h = A0
        + A1 * cos(trig_arg) + B1 * sin(trig_arg)
        + A2 * cos(2.0 * trig_arg) + B2 * sin(2.0 * trig_arg)
        + A3 * cos(3.0 * trig_arg) + B3 * sin(3.0 * trig_arg)
        + A4 * cos(4.0 * trig_arg) + B4 * sin(4.0 * trig_arg)
        + A5 * cos(5.0 * trig_arg) + B5 * sin(5.0 * trig_arg);
    
    /* Calculate local solar time and convert to output format. */
    solar_s = clock_s + (time_t) (EOT_h * 3600.0 
                         - LON * 240.0 - clock_data.tm_isdst * 3600.0);
    
    /*Output to stdout. */
    strftime(buff, 20, SOLAR_FORMAT, localtime(&solar_s));
    printf("%s\n", buff);
    return 0;
}

int leap_cycle_day(struct tm time_s) {
        return 1 + time_s.tm_yday + 365 * (time_s.tm_year % 4);
}
