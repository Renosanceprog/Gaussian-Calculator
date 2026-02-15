// GAUSSIAN SOLVER
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define MatrixMax 100
float matrix[MatrixMax][MatrixMax+1];
float UniqueEqns[MatrixMax];
int GySIZE = 0, GxSIZE = 0, swapTally = 0;

int main();
    void Choice_inputmatrix();
    int Choice_editmatrix();
        void Edit_choiceDimes();
        void Edit_choiceValues();
    void Choice_demomatrices();

void comp_execution();
    void printmatrix();
    int fwdElimination();
        int rowSwap(int h);
    float Determinant();
    void revSubstitution();
    void printsolutions();

void header();
void print_grid(int current_r, int current_c, int mode);
int validinput();
float Fvalidinput();

int main(){
    while (1)
    {
        header();
        printf("[1] Input new Matrix\n[2] Edit Matrix\n[3] Demo Matrices \n[4] Run Calculations\n[5] Exit\nSelect: ");
        switch (validinput())
        {
        case 1:
            Choice_inputmatrix();
            header();
            print_grid(GySIZE, GxSIZE, 1);
            system("pause");
            header();
            comp_execution();
            system("pause");
            break;
        case 2:
            Choice_editmatrix();
            break;
        case 3:
            /* code */
            break;
        case 4:
            comp_execution();
            system("pause");
            break;
        case 5:
            printf("Exiting program...\n");
            system("pause");
            return 0;
            break;
        default:
            printf("Invalid choice, Select from 1-5.\n");
            system("pause");
            break;
        }
    }
}

void Choice_inputmatrix(){
    printf("\n");
    printf("Enter # of Equations: ");
    GySIZE = validinput();
    printf("Enter # of Variables (including constants): ");
    GxSIZE = validinput();
    for (int i = 0; i < GySIZE; i++) {
        for (int j = 0; j < GxSIZE; j++) { // Loop 4 times (x,y,z, constant)
            header();
            printf("Enter values (");
            for (int v = GySIZE-1; v >=0; v--){
                printf("%c, ", 'z'-v);
            }
            printf("| constant):\n");
            print_grid(i, j, 1);
            
            if (j < 3) printf("\nEnter Coeff [%d][%d]: ", i+1, j+1);
            else       printf("\nEnter Constant for Row %d: ", i+1);
            
            matrix[i][j] = Fvalidinput();
        }
    }
}

int Choice_editmatrix(){
    if (GxSIZE == 0 || GySIZE == 0)
    {
        printf("\nUser has not input a matrix yet, choose [1] on the next prompt and try again later.\n");
        system("pause");
        return 1;
    }
    while (1)
    {
        header();
        printmatrix();
        printf("[1] Edit Dimensions\n[2] Edit Values\n[3] Return to Main Menu\nSelect: ");
        switch (validinput())
        {
        case 1:
            Edit_choiceDimes();
            break;
        case 2:
            Edit_choiceValues();
            break;
        case 3:
            return 0;
            break;
        default:
            printf("Invalid choice, Select from 1-3.\n");
            system("pause");
            break;
        }
    }
}

void Edit_choiceDimes(){
    printf("[1] Edit Dimension Values\n[2] Add Equations (Rows)\n[3] Add Variables (Columns)\nSelect: ");
    switch (validinput())
    {
    case 1:
        printf("Enter # of Equations: ");
        GySIZE = validinput();
        printf("Enter # of Variables (including constants): ");
        GxSIZE = validinput();
        break;
    case 2:
        printf("Enter # of Equations to add: ");
        int y_add = validinput();
        GySIZE += y_add;
        break;
    case 3:
        printf("Enter # of Variables to add: ");
        int x_add = validinput();
        GxSIZE += x_add;
        break;
    default:
        printf("Invalid choice, Select from 1-3 later.\n");
        break;
    }

}

void Edit_choiceValues(){
    int X_index, Y_index;
    print_grid(GySIZE, GxSIZE, 1);
    printf("\nNote matrix index starts at [1][1]\n");
    while (1)
    {
        header();
        print_grid(GySIZE, GxSIZE, 1);
        printf("Enter # i: ");
        Y_index = validinput();
        Y_index -= 1;
        if (Y_index < 0 || Y_index > GySIZE-1)
        {
            printf("Cannot Access value outside matrix dimension.\n");
            system("pause");
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        header();
        print_grid(GySIZE, GxSIZE, 1);
        printf("Enter # j: ");
        X_index = validinput();
        X_index -= 1;
        if (X_index < 0 || X_index > GxSIZE-1)
        {
            printf("Cannot Access value outside matrix dimension.\n");
            system("pause");
        }
        else
        {
            break;
        }
    }
    header();
    printf("Enter values (");
    for (int v = GySIZE-1; v >=0; v--){
        printf("%c, ", 'z'-v);
    }
    printf("| constant):\n");
    print_grid(Y_index, X_index, 2);
    if (X_index < 3) printf("\nEnter Coeff [%d][%d]: ", Y_index+1, X_index+1);
    else       printf("\nEnter Constant for Row %d: ", Y_index+1);
    matrix[Y_index][X_index] = Fvalidinput();
    header();
    print_grid(GySIZE, GxSIZE, 1);
}

void Choice_demomatrices(){

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

float Determinant(){
    float Det = 1;
    for (size_t i = 0; i < GySIZE; i++)
    {
        Det = Det * matrix[i][i];
    }
    Det = Det * pow(-1, swapTally);
    return Det;
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

void printsolutions(){
    int i2 = 0;
    for (int i = GySIZE-1; i >= 0; i--)
    {
        printf("%c = %6.2f\n", 122-i, UniqueEqns[i2]);
        i2++;
    }
    int det = Determinant();
    printf("\nDeterminant = %d\n", det);
}

void header(){
    system("cls");
    printf("==== Gaussian Solver ====\n");
}

void print_grid(int current_r, int current_c, int mode) {
    printf("\n");
        for(int r = 0; r < GySIZE; r++) {
            printf("  [ ");
            for(int c = 0; c < GxSIZE; c++) {
                // Separator for the constant column
                if (c == GxSIZE-1) printf("| "); 
                if (mode == 1)
                {
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
                else if (mode == 2) // edit mode
                {
                    if(r == current_r && c == current_c) { // replaces chosen index with ?
                        printf("%6c ", '?'); // Cursor
                    }
                    else {
                        printf("%6.2f ", matrix[r][c]); // Already entered
                    }
                }
                
            }
            printf("]\n");
        }
    printf("\n");
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