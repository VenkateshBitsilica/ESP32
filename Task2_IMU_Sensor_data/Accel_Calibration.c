#include <stdio.h>
#include <math.h>  // For sqrt()

int main() {

    int x = 162;
    int y = 307;
    int z = 15027;
    double res = sqrt((x * x) + (y * y) + (z * z));
    printf(" %.2f\n", res);
    return 0;
}
