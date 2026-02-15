//
#include <stdio.h>
#include <math.h>

#define GySIZE 3
#define GxSIZE 4

float matrix[GySIZE][GxSIZE] = {
    {1,  1,  1,  1, 10},  
    {0,  -3, -2,  -1, -16},  
    {0,  0,  -4,  -1, -16},
    {0, 0, 0, -1.83, -7.33}  
};

float x = 0, y = 0, z = 0;
float UniqueEqns[GySIZE];

void manual();
void revSubstitution();
int main(){
    //manual();
    revSubstitution();
    printf("z = %5.2f\n", UniqueEqns[3]);
    printf("y = %5.2f\n", UniqueEqns[2]); // z =  1.00
    printf("x = %5.2f\n", UniqueEqns[1]); // y = -1.00
    printf("w = %5.2f\n", UniqueEqns[0]); // x =  2.00

    return 0;
}

void manual(){
    float bucket;
    bucket = matrix[2][3] - (z*matrix[2][2] + y*matrix[2][1]);
    z = bucket / matrix[2][2];

    bucket = matrix[1][3] - (z*matrix[1][2] + y*matrix[1][1]);
    y = bucket / matrix[1][1];

    bucket = matrix[0][3] - (z*matrix[0][2] + y*matrix[0][1]);
    x = bucket / matrix[0][0];
}

void revSubstitution(){
    for (int i = GySIZE-1; i >= 0 ; i--)
    {
        float bucket = matrix[i][GxSIZE-1];
        float total;
        for (int j = i + 1; j < GySIZE; j++)
        {
            bucket = bucket - (matrix[i][j] * UniqueEqns[j]);
        }
        UniqueEqns[i] = bucket / matrix[i][i];
    }
}