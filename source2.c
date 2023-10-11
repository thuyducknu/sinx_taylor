#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#define N 4
#define MAXLINE 100
#define _USE_MATH_DEFINES
void sinx_taylor(int num_elements, int terms, double* x, double* result) {
    for (int i = 0; i < num_elements; i++) {
        double value = x[i];
        double numner = x[i]*x[i]*x[i];
        double denom = 6;
        int sign = -1;

        for (int j = 1; j < terms; j++) {
            value += (double)sign * numner/denom;
            numner *= x[i]*x[i];
            denom *= (2.*(double)j+2.)*(2.*(double)j+3.);
            sign*=-1;
        }
        result[i] = value;
    }
}
void sinx_taylor_using_pid(int num_elements, int terms, double* x, double* result){
    int fd[2*N];
    char message[MAXLINE], line[MAXLINE];
    int length;
    for (int i = 0; i<num_elements;i++) {
        int* p_id=&fd[2+i];
        pipe(p_id);
    }
    int pid;
    for (int i = 0; i < num_elements; i++) {
        int* p_fd = &fd[2*i];
        if((pid = fork()) == 0) {
            close(p_fd[0]);
            int my_id=i;
            double value = x[i];
            double numner = x[i]*x[i]*x[i];
            double denom = 6;
            int sign = -1;

            for (int j = 1; j < terms; j++) {
                value += (double)sign * numner/denom;
                numner *= x[i]*x[i];
                denom *= (2.*(double)j+2.)*(2.*(double)j+3.);
                sign*=-1;
            }
            result[i] = value;
            sprintf(message, "%lf", result[i]);
            length = strlen(message)+1;
            write(fd[1], message, length);
            exit(my_id);
        } else {
            close(p_fd[1]);
        }
    }

    int stat;
    for (int i = 0;i<num_elements;i++) {
        wait(&stat);
        int child_id = stat >> 8;
        read(fd[2*child_id], line, MAXLINE);

        double res = atof(line);
        printf("%d %lf %sWn", child_id, res, line);
        result[child_id] = res;
    }
}
int main() {
    double x[N]={0, M_PI/6., M_PI/3., 0.134};
    double res[N];

    // Calculate the sine of each element in the x array.
    sinx_taylor(N, 3, x, res);

    // Print the results.
    for (int i = 0; i < N; i++) {
        printf("sin(%f) = %f\n", x[i], res[i]);
    }

    return 0;
}