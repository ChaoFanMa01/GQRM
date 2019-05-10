#include <stdio.h>
#include <stdlib.h>

#include "../src/prr.h"

int
main(int argc, char* argv[])
{
    double  power, distance;

    if (argc != 3)
        return -1;

    power = strtod(argv[1], NULL);
    distance = strtod(argv[2], NULL);

    printf("PRR is %lf\n", prr(power, distance));
    return 0;
}
