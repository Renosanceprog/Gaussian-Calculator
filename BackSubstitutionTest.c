//
#include <stdio.h>

float matrix[3][4] = {
    { 2,  4, -2, -3},
    { 0,  4,  0,  2},
    { 0,  0,  2,  3}
};

int main(){
    float x = 0, y = 0, z = 0, bucket;

    z = matrix[2][3] / matrix[2][2];

    bucket = matrix[1][3] - (z*matrix[1][2]);
    y = bucket / matrix[1][1];



    bucket = matrix[0][3] - (z*matrix[0][2] + y*matrix[0][1]);
    x = bucket / matrix[0][0];

    printf("x = %5.2f\n", x);
    printf("y = %5.2f\n", y);
    printf("z = %5.2f\n", z);
    return 0;
}