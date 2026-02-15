// ==========================================
//   GAUSSIAN SOLVER (UNIVERSAL SAFE MODE)
//   Compatible with: Windows, Linux, macOS
// ==========================================

#include <stdio.h>
#include <math.h>
#include <stdlib.h> // For exit()

#define MatrixMax 100

// --- GLOBAL VARIABLES ---
float matrix[MatrixMax][MatrixMax+1];
float UniqueEqns[MatrixMax];
int GySIZE = 0, GxSIZE = 0, swapTally = 0;

// --- PROTOTYPES ---
int main();
void Choice_inputmatrix();
int Choice_editmatrix();
    void Edit_choiceDimes();
    void Edit_choiceValues();
void Choice_demomatrices();
    void LoadDemo(int page);

int comp_execution();
    void printmatrix();
    int fwdElimination();
    int rowSwap(int h);
    float Determinant();
    void revSubstitution();
    void printsolutions();

// Utilities
#ifdef _WIN32
    #include <stdlib.h>
    void ClearScreen() {
        system("cls");
    }
#else
    #include <stdio.h>
    void ClearScreen() {
        // ANSI Escape Code for Linux/Mac
        printf("\033[1;1H\033[2J");
    }
#endif
void PressEnter();
void print_grid(int current_r, int current_c, int mode);
int validinput();
float Fvalidinput();

// ==========================================
//   MAIN PROGRAM
// ==========================================
int main(){
    int choice;
    while (1) {
        ClearScreen();
        printf("==== GAUSSIAN SOLVER (SAFE MODE) ====\n");
        printf("[1] Input New Matrix\n");
        printf("[2] Edit Matrix Data\n");
        printf("[3] Load Demo Matrix\n");
        printf("[4] Run Solver\n");
        printf("[5] Exit\n");
        printf("Select: ");
        
        choice = validinput();

        switch (choice) {
            case 1:
                Choice_inputmatrix();
                printf("\nMatrix Stored Successfully.\n");
                PressEnter();
                
                // Optional: Auto-run
                printf("Run solver now? (1=Yes, 0=No): ");
                if (validinput() == 1) {
                    comp_execution();
                    PressEnter();
                }
                break;

            case 2:
                Choice_editmatrix();
                break;

            case 3:
                Choice_demomatrices();
                break;

            case 4:
                comp_execution();
                PressEnter();
                break;

            case 5:
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice. Please select 1-5.\n");
                PressEnter();
                break;
        }
    }
}

// ==========================================
//   INPUT LOGIC
// ==========================================
void Choice_inputmatrix(){
    printf("\n--- NEW MATRIX INPUT ---\n");
    printf("Enter # of Equations (Rows): ");
    GySIZE = validinput();
    printf("Enter # of Variables (Cols): ");
    GxSIZE = validinput();

    for (int i = 0; i < GySIZE; i++) {
        for (int j = 0; j < GxSIZE; j++) { 
            ClearScreen();
            printf("--- FILLING DATA ---\n");
            
            // Helpful context header
            printf("Values: ( ");
            for (int v = GySIZE-1; v >=0; v--) printf("%c ", 'z'-v);
            printf("| Const )\n");
            
            print_grid(i, j, 1);
            
            if (j < GxSIZE - 1) 
                printf("\nEnter Coefficient for %c [%d][%d]: ", 'z'-(GySIZE-1-j), i+1, j+1);
            else       
                printf("\nEnter Constant for Row %d: ", i+1);
            
            matrix[i][j] = Fvalidinput();
        }
    }
}

// ==========================================
//   EDIT LOGIC
// ==========================================
int Choice_editmatrix(){
    if (GxSIZE == 0 || GySIZE == 0) {
        printf("\nError: No matrix data found. Input or Load Demo first.\n");
        PressEnter();
        return 1;
    }

    int choice;
    while(1) {
        ClearScreen();
        printf("--- EDIT MENU ---\n");
        printmatrix();
        printf("[1] Edit Dimensions\n");
        printf("[2] Edit Specific Value\n");
        printf("[3] Return to Main Menu\n");
        printf("Select: ");
        
        choice = validinput();
        
        switch (choice) {
            case 1: Edit_choiceDimes(); break;
            case 2: Edit_choiceValues(); break;
            case 3: return 0;
            default: printf("Invalid choice.\n"); PressEnter(); break;
        }
    }
}

void Edit_choiceDimes(){
    printf("\n[1] Reset All Dimensions\n[2] Add Equation (Row)\n[3] Add Variable (Col)\nSelect: ");
    switch (validinput()) {
        case 1:
            printf("New Rows: "); GySIZE = validinput();
            printf("New Cols: "); GxSIZE = validinput();
            break;
        case 2:
            printf("Add Rows: "); GySIZE += validinput();
            break;
        case 3:
            printf("Add Cols: "); GxSIZE += validinput();
            break;
    }
}

void Edit_choiceValues(){
    int r, c;
    printf("\nRow Index (1-%d): ", GySIZE);
    r = validinput();
    if (r < 1 || r > GySIZE) { printf("Invalid Row.\n"); PressEnter(); return; }
    
    printf("Col Index (1-%d): ", GxSIZE);
    c = validinput();
    if (c < 1 || c > GxSIZE) { printf("Invalid Col.\n"); PressEnter(); return; }
    
    printf("Current Value: %.2f\n", matrix[r-1][c-1]);
    printf("New Value: ");
    matrix[r-1][c-1] = Fvalidinput();
    printf("Value updated.\n");
    PressEnter();
}

// ==========================================
//   DEMO LOGIC (Standard Menu)
// ==========================================
void Choice_demomatrices(){
    int choice;
    while(1) {
        ClearScreen();
        printf("--- DEMO LIBRARY ---\n");
        printf("[1] 3x3 Unique Solution\n");
        printf("[2] 3x3 No Solution (Inconsistent)\n");
        printf("[3] 3x3 Infinite Solutions\n");
        printf("[4] 4x4 Large System\n");
        printf("[5] 5x5 Diagonally Dominant\n");
        printf("[6] Return to Main Menu\n");
        printf("Select: ");
        
        choice = validinput();
        if (choice == 6) return;
        if (choice >= 1 && choice <= 5) {
            LoadDemo(choice - 1); // Adjust for 0-index
            printf("\nDemo %d Loaded Successfully!\n", choice);
            printmatrix();
            PressEnter();
            return;
        }
        printf("Invalid selection.\n");
        PressEnter();
    }
}

void LoadDemo(int page) {
    // Clear Matrix
    for(int i=0; i<MatrixMax; i++) for(int j=0; j<MatrixMax+1; j++) matrix[i][j] = 0.0;

    switch (page) {
        case 0: // Unique
            GySIZE = 3; GxSIZE = 4;
            float p1[3][4] = {{0,2,1,7}, {1,1,1,6}, {3,2,-1,4}};
            for(int i=0;i<3;i++) for(int j=0;j<4;j++) matrix[i][j]=p1[i][j];
            break;
        case 1: // No Sol
            GySIZE = 3; GxSIZE = 4;
            float p2[3][4] = {{1,1,1,6}, {2,2,2,12}, {2,2,2,20}};
            for(int i=0;i<3;i++) for(int j=0;j<4;j++) matrix[i][j]=p2[i][j];
            break;
        case 2: // Infinite
            GySIZE = 3; GxSIZE = 4;
            float p3[3][4] = {{1,2,3,14}, {1,-1,1,6}, {2,1,4,20}};
            for(int i=0;i<3;i++) for(int j=0;j<4;j++) matrix[i][j]=p3[i][j];
            break;
        case 3: // 4x4
            GySIZE = 4; GxSIZE = 5;
            float p4[4][5] = {{1,1,1,1,10}, {1,-1,2,0,5}, {0,1,-1,3,11}, {2,2,0,-1,2}};
            for(int i=0;i<4;i++) for(int j=0;j<5;j++) matrix[i][j]=p4[i][j];
            break;
        case 4: // 5x5
            GySIZE = 5; GxSIZE = 6;
            float p5[5][6] = {{5,1,1,1,1,19}, {1,5,1,1,1,23}, {1,1,5,1,1,27}, {1,1,1,5,1,31}, {1,1,1,1,5,35}};
            for(int i=0;i<5;i++) for(int j=0;j<6;j++) matrix[i][j]=p5[i][j];
            break;
    }
}

// ==========================================
//   SOLVER CORE
// ==========================================
int comp_execution(){
    if (GxSIZE == 0 || GySIZE == 0) {
        printf("\nError: No matrix data found.\n");
        return 1;
    }

    printf("\n--- RAW MATRIX ---\n");
    printmatrix();
    fwdElimination();
    printf("--- PARTIAL REF ---\n");
    printmatrix();

    if (fabs(Determinant()) < 1e-6) {
        if (matrix[GySIZE-1][0] != matrix[GySIZE-1][GxSIZE-1])
            printf("RESULT: NO SOLUTION (Inconsistent System)\n");
        else
            printf("RESULT: INFINITE SOLUTIONS (Dependent System)\n");
    } else {
        revSubstitution();
        printsolutions();
    }
    return 0;
}

void printmatrix(){
    for (size_t i = 0; i < GySIZE; i++) {
        printf("  ");
        for (size_t j = 0; j < GxSIZE; j++) {   
            if (j == GxSIZE-1) printf("| ");
            printf("%6.2f ",matrix[i][j]);
        }
        printf("\n");
    }
}

int fwdElimination(){
    for (size_t h = 0; h < GySIZE - 1; h++) {
        if (fabs(matrix[h][h]) < 1e-6) rowSwap(h);
        float pvt1 = matrix[h][h];
        for (size_t i = h + 1; i < GySIZE; i++) {
            float pvt2 = matrix[i][h];
            if (pvt2 == 0 && pvt1 == 0) return 1;
            float ratio = pvt2 / pvt1;
            for (size_t j = 0; j < GxSIZE; j++) {   
                matrix[i][j] = matrix[i][j] - (ratio * matrix[h][j]);
            }
        }
    }
    return 0;
}

int rowSwap(int h){
    swapTally++;
    for (size_t i = h + 1; i < GySIZE; i++) {
        if (fabs(matrix[i][h]) > 1e-6) {
            for (size_t j = 0; j < GxSIZE; j++) {
                float temp = matrix[h][j];
                matrix[h][j] = matrix[i][j];
                matrix[i][j] = temp;
            }
            break;
        }
    }
    return 0;
}

float Determinant(){
    float Det = 1;
    for (size_t i = 0; i < GySIZE; i++) Det = Det * matrix[i][i];
    Det = Det * pow(-1, swapTally);
    return Det;
}

void revSubstitution(){
    for (int i = GySIZE-1; i >= 0 ; i--) {
        float bucket = matrix[i][GxSIZE-1];
        for (int j = i + 1; j < GySIZE; j++) {
            bucket = bucket - (matrix[i][j] * UniqueEqns[j]);
        }
        UniqueEqns[i] = bucket / matrix[i][i];
    }
}

void printsolutions(){
    printf("\n--- SOLUTIONS ---\n");
    int i2 = 0;
    for (int i = GySIZE-1; i >= 0; i--) {
        printf("  %c = %6.2f\n", 122-i, UniqueEqns[i2]);
        i2++;
    }
    printf("\n  Determinant = %.2f\n", Determinant());
}

// ==========================================
//   UNIVERSAL UTILITIES
// ==========================================

// Safe replacement for system("pause")
void PressEnter() {
    printf("\nPress [ENTER] to continue...");
    int c;
    // Consume any leftover newline from previous input
    while ((c = getchar()) != '\n' && c != EOF); 
    // Wait for actual Enter
    // Note: If input buffer was clean, this might require 2 enters. 
    // Usually standard `getchar()` is enough.
    getchar(); 
}

void print_grid(int current_r, int current_c, int mode) {
    printf("\n");
    for(int r = 0; r < GySIZE; r++) {
        printf("    [ ");
        for(int c = 0; c < GxSIZE; c++) {
            if (c == GxSIZE-1) printf("| "); 
            
            if (mode == 1) { // Input Mode
                if(r < current_r || (r == current_r && c < current_c))
                    printf("%6.2f ", matrix[r][c]);
                else if(r == current_r && c == current_c)
                    printf("%6c ", '?');
                else
                    printf("%6c ", '.');
            }
            else { // Static Mode (for Edit)
                printf("%6.2f ", matrix[r][c]);
            }
        }
        printf("]\n");
    }
    printf("\n");
}

int validinput(){
    int valid = 0, input;
    do {
        if (scanf(" %d", &input) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
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
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        valid = 1;
    } while (!valid);
    return input;
}