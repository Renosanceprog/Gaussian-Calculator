//
#include <stdio.h>
#include <math.h>

#define GySIZE 3
#define GxSIZE 4

void printmatrix();
void fwdElimination();
int Determinant();
int rowSwap(int h);



float matrix[3][4] = {
    { 1.0,  1.0,  1.0,   4.0},
    { 2.0,  2.0,  2.0,  10.0}, // <--- The culprit (2 * Row1 = 8, not 10)
    { 0.0,  1.0, -1.0,   3.0}
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
    return 0;
}

void printmatrix(){
    for (size_t i = 0; i < GySIZE; i++)
    {
        for (size_t j = 0; j < GxSIZE; j++)
        {   
            if (j == 3)
            {
                printf("| ");
            }
            
            printf("%5.2f  ",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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