//
#include <stdio.h>
#include <math.h>

#define GySIZE 4
#define GxSIZE 5

void printmatrix();
void printsolutions();
void fwdElimination();
void revSubstitution();
int Determinant();
int rowSwap(int h);

float UniqueEqns[GySIZE];
float matrix[4][5] = {
    { 1.0,  2.0,  1.0,  0.0,   5.0},
    { 0.0,  1.0,  0.0,  1.0,   2.0},
    { 0.0,  0.0,  1.0,  1.0,   3.0},
    { 1.0,  3.0,  1.0,  1.0,   7.0}  // This is (R1 + R2)
};

int main(){
    printmatrix();
    fwdElimination();
    printmatrix();
    if (fabs(Determinant()) < 1e-6)
    {
        if (matrix[GySIZE-1][0] != matrix[GySIZE-1][GxSIZE-1])
        {
            printf("This matrix has NO Solutions\n");
        }
        else
        {
            printf("This matrix has INFINITE Solutions\n");
        }
    }
    else
    {
        revSubstitution();
        printsolutions();
    }

    return 0;
}

void printmatrix(){
    for (size_t i = 0; i < GySIZE; i++)
    {
        for (size_t j = 0; j < GxSIZE; j++)
        {   
            if (j == GxSIZE-1)
            {
                printf("| ");
            }
            
            printf("%6.2f  ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printsolutions(){
    int i2 = 0;
    for (int i = GySIZE-1; i >= 0; i--)
    {
        printf("%c = %6.2f\n", 122-i, UniqueEqns[i2]);
        i2++;
    }
}

int Determinant(){
    float Det = 1;
    for (size_t i = 0; i < GySIZE; i++)
    {
        Det = Det * matrix[i][i];
    }
    return Det;
}

int rowSwap(int h){
    for (size_t i = h + 1; i < GySIZE; i++)
    {
        if (fabs(matrix[i][h]) > 1e-6)
        {
            for (size_t j = 0; j < GxSIZE; j++)
            {
                float temp = matrix[h][j];
                matrix[h][j] = matrix[i][j];
                matrix[i][j] = temp;
            }
            break;
        }
    }
}

void fwdElimination(){
    for (size_t h = 0; h < GySIZE - 1; h++)
    {
        if (fabs(matrix[h][h]) < 1e-6)
        {
            rowSwap(h);
        }

        float pvt1 = matrix[h][h];
        for (size_t i = h + 1; i < GySIZE; i++)
        {
            float pvt2 = matrix[i][h];
            float ratio = pvt2 / pvt1;
            for (size_t j = 0; j < GxSIZE; j++)
            {   
                matrix[i][j] = matrix[i][j] - (ratio * matrix[h][j]);
            }
        }
    }
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