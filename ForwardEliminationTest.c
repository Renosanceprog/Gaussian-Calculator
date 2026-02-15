//
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define MatrixMax 100
float matrix[MatrixMax][MatrixMax+1];
float UniqueEqns[MatrixMax];
int GySIZE, GxSIZE, swapTally = 0;

void printmatrix();
void printsolutions();
void header();
void print_grid(int current_r, int current_c);
int fwdElimination();
void revSubstitution();
void comp_execution();
float Determinant();
int rowSwap(int h);
int validinput();
float Fvalidinput();

int main(){
    header();
    printf("Enter # of Equations: ");
    GySIZE = validinput();
    printf("Enter # of Variables (including constants): ");
    GxSIZE = validinput();
    for (int i = 0; i < GySIZE; i++) {
        for (int j = 0; j < GxSIZE; j++) { // Loop 4 times (x,y,z, constant)
            header();
            printf("Enter values (x, y, z | constant):\n\n");
            print_grid(i, j);
            
            if (j < 3) printf("\nEnter Coeff [%d][%d]: ", i+1, j+1);
            else       printf("\nEnter Constant for Row %d: ", i+1);
            
            matrix[i][j] = Fvalidinput();
        }
    }
    header();
    printf("\n");
    print_grid(GySIZE, GxSIZE);
    printf("\n");
    system("pause");
    header();
    comp_execution();
    return 0;
}
void header(){
    system("cls");
    printf("==== Gaussian Solver ====\n");
}
void comp_execution(){
    printf("==== Raw Matrix ====\n");
    printmatrix();
    fwdElimination();
    printf("==== Partial REF ====\n");
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
    int det = Determinant();
    printf("\nDeterminant = %d", det);
}

void print_grid(int current_r, int current_c) {
    for(int r = 0; r < GySIZE; r++) {
        printf("  [ ");
        for(int c = 0; c < GxSIZE; c++) {
            // Separator for the constant column
            if (c == GxSIZE-1) printf("| "); 
            
            if(r < current_r || (r == current_r && c < current_c)) {
                printf("%6.2f ", matrix[r][c]); // Already entered
            }
            else if(r == current_r && c == current_c) {
                printf("%6c ", '?'); // Cursor
            }
            else {
                printf("%6c ", '.'); // Future
            }
        }
        printf("]\n");
    }
}

float Determinant(){
    float Det = 1;
    for (size_t i = 0; i < GySIZE; i++)
    {
        Det = Det * matrix[i][i];
    }
    Det = Det * pow(-1, swapTally);
    return Det;
}

int rowSwap(int h){
    swapTally++;
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

int fwdElimination(){
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
            if (pvt2 == 0 && pvt1 == 0)
            {
                printf("cannot calculate Partial REF due to division by 0.\n");
                return 1;
            }
            
            float ratio = pvt2 / pvt1;
            for (size_t j = 0; j < GxSIZE; j++)
            {   
                matrix[i][j] = matrix[i][j] - (ratio * matrix[h][j]);
            }
        }
    }
    return 0;
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

int validinput(){
    int valid = 0;
    int input;

    do {
        if (scanf(" %d", &input) != 1) {
            printf("Invalid input.\nenter value: ");
            int c; while ((c = getchar()) != '\n' && c != EOF); /* clear bad input */
            continue; /* try again */
        }
        valid = 1;
    } while (!valid);
    return input;
}
float Fvalidinput(){
    int valid = 0;
    float input;

    do {
        if (scanf(" %f", &input) != 1) {
            printf("Invalid input.\nenter value: ");
            int c; while ((c = getchar()) != '\n' && c != EOF); /* clear bad input */
            continue; /* try again */
        }
        valid = 1;
    } while (!valid);
    return input;
}