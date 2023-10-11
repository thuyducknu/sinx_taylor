#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void sinx_taylor(int num_elements, int terms, double* x, double* result) {
    for (int i = 0;i<num_elements;i++) {
        double value = x[i];
        double numner = x[i]*x[i]*x[i];
        double denom = 6;
        int sign = -1;

        for (int j=1;j<terms;j++) {
            value += (double)sign * numner/denom;
            numner *= x[i]*x[i];
            denom *= (2.*(double)j+2.)*(2.*(double)j+3.);
            sign*=-1;
        }
        result[i] = value;
    }
}
int main() {
    int num_elements = 10;
    int terms = 100;

    double* x = malloc(num_elements * sizeof(double));
    double* result = malloc(num_elements * sizeof(double));

    for (int i = 0; i < num_elements; i++) {
        x[i] = i * 0.1;
    }

    sinx_taylor(num_elements, terms, x, result);

    for (int i = 0; i < num_elements; i++) {
        printf("sin(%f) = %f\n", x[i], result[i]);
    }

    free(x);
    free(result);

    return 0;
}