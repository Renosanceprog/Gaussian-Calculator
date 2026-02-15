// ==========================================
//   REVAMPED GAUSSIAN SOLVER (UI UPDATE)
// ==========================================

#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <stdlib.h>     // For system()
#include <string.h>     // For strlen()

// --- SETTINGS & MACROS ---
#define MatrixMax 100
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_BACKSPACE 8

// --- GLOBAL VARIABLES ---
float matrix[MatrixMax][MatrixMax+1];
float UniqueEqns[MatrixMax];
int GySIZE = 0, GxSIZE = 0, swapTally = 0;

// --- PROTOTYPES ---
// Core Logic (Your Original Structure)
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

// UI & Utilities (Revamped)
void Banner(const char* subtitle);
int ArrowMenu(const char* title, const char* options[], int count);
void print_grid(int current_r, int current_c, int mode);
void PauseKey();
int validinput();
float Fvalidinput();
void DrawLine(int length);

// ==========================================
//   MAIN PROGRAM
// ==========================================
int main(){
    const char* mainOptions[] = {
        "Input New Matrix",
        "Edit Matrix Data",
        "Demo Showcase",
        "Run Solver",
        "Exit Application"
    };
    int numOptions = 5;

    while (1)
    {
        // Use the new Arrow Menu
        int choice = ArrowMenu("MAIN MENU", mainOptions, numOptions);

        switch (choice)
        {
        case 0: // Input
            Choice_inputmatrix();
            Banner("INPUT SUCCESS");
            print_grid(GySIZE, GxSIZE, 1);
            PauseKey();
            
            // Auto-run solver query
            Banner("SOLVER");
            printf("Calculating results...\n");
            comp_execution();
            PauseKey();
            break;

        case 1: // Edit
            Choice_editmatrix();
            break;

        case 2: // Demo
            Choice_demomatrices();
            break;

        case 3: // Run
            Banner("SOLVER RESULTS");
            comp_execution();
            PauseKey();
            break;

        case 4: // Exit
            Banner("SHUTDOWN");
            printf("\n  Thank you for using Gaussian Solver.\n  Goodbye!\n\n");
            return 0;
        }
    }
}

// ==========================================
//   UI HELPERS (THE NEW STUFF)
// ==========================================

// Creates a nice navigable menu using arrow keys
// Creates a nice navigable menu using arrow keys
int ArrowMenu(const char* title, const char* options[], int count) {
    int selected = 0;
    int key;

    while(1) {
        Banner(title);
        printf("  Use [UP/DOWN] to move, [ENTER] to select.\n\n");

        for(int i = 0; i < count; i++) {
            if(i == selected) {
                printf("  >> [ %s ] <<\n", options[i]); // Highlight
            } else {
                printf("       %s      \n", options[i]);
            }
        }
        printf("\n");
        DrawLine(40);

        key = _getch();

        // FIX: Check for BOTH 0 and 224
        if(key == 0 || key == 224) {
            key = _getch(); // Get the second code (the actual arrow)
            if(key == KEY_UP) {
                selected--;
                if(selected < 0) selected = count - 1; // Wrap to bottom
            }
            if(key == KEY_DOWN) {
                selected++;
                if(selected >= count) selected = 0; // Wrap to top
            }
        }
        else if(key == KEY_ENTER) {
            return selected;
        }
    }
}

// Standard header for every screen
void Banner(const char* subtitle) {
    system("cls");
    printf("\n");
    DrawLine(40);
    printf("  GAUSSIAN SOLVER PRO\n");
    if(subtitle != NULL) {
        printf("  :: %s\n", subtitle);
    }
    DrawLine(40);
    printf("\n");
}

void DrawLine(int length) {
    printf("  ");
    for(int i=0; i<length; i++) printf("-");
    printf("\n");
}

void PauseKey() {
    printf("\n  > Press any key to continue...");
    _getch();
}

// ==========================================
//   INPUT LOGIC
// ==========================================
void Choice_inputmatrix(){
    Banner("NEW MATRIX INPUT");
    printf("  Enter # of Equations (Rows): ");
    GySIZE = validinput();
    printf("  Enter # of Variables (Cols): ");
    GxSIZE = validinput();

    for (int i = 0; i < GySIZE; i++) {
        for (int j = 0; j < GxSIZE; j++) { 
            Banner("FILLING DATA");
            
            // Helpful context header
            printf("  Values: ( ");
            for (int v = GySIZE-1; v >=0; v--) printf("%c ", 'z'-v);
            printf("| Const )\n");
            
            print_grid(i, j, 1);
            
            if (j < GxSIZE - 1) 
                printf("\n  Enter Coeff for %c [%d][%d]: ", 'z'-(GySIZE-1-j), i+1, j+1);
            else       
                printf("\n  Enter Constant for Row %d: ", i+1);
            
            matrix[i][j] = Fvalidinput();
        }
    }
}

// ==========================================
//   EDIT LOGIC (Refined)
// ==========================================
int Choice_editmatrix(){
    if (GxSIZE == 0 || GySIZE == 0) {
        Banner("ERROR");
        printf("  No matrix data found.\n  Please Input or Load Demo first.");
        PauseKey();
        return 1;
    }

    const char* editOptions[] = {
        "Edit Matrix Dimensions",
        "Edit Specific Value",
        "Return to Main Menu"
    };

    while(1) {
        // Show current state above menu
        Banner("EDIT DASHBOARD");
        printmatrix();
        printf("\n");
        
        int choice = ArrowMenu("EDIT MENU", editOptions, 3);
        
        switch (choice) {
            case 0: Edit_choiceDimes(); break;
            case 1: Edit_choiceValues(); break;
            case 2: return 0;
        }
    }
}

void Edit_choiceDimes(){
    const char* dimOptions[] = {
        "Reset All Dimensions",
        "Add Equation (Row)",
        "Add Variable (Col)",
        "Cancel"
    };
    
    int choice = ArrowMenu("DIMENSION EDITOR", dimOptions, 4);
    
    Banner("MODIFY DIMENSIONS");
    switch (choice) {
        case 0:
            printf("  New Rows: "); GySIZE = validinput();
            printf("  New Cols: "); GxSIZE = validinput();
            break;
        case 1:
            printf("  Add Rows: ");
            GySIZE += validinput();
            break;
        case 2:
            printf("  Add Cols: ");
            GxSIZE += validinput();
            break;
        case 3:
            return;
    }
}

void Edit_choiceValues(){
    int cursorX = 0;
    int cursorY = 0;
    int key;

    while(1) {
        Banner("INTERACTIVE EDITOR");
        printf("  [ARROWS] Navigate   [ENTER] Edit Value   [ESC] Done\n");
        
        // Use Mode 3 to show the cursor
        print_grid(cursorY, cursorX, 3);
        
        // Visual Footer
        DrawLine(40);
        printf("  Current Cell [%d][%d]: %.2f\n", cursorY+1, cursorX+1, matrix[cursorY][cursorX]);

        key = _getch();

        // 1. Navigation Logic
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == KEY_UP) {
                if (cursorY > 0) cursorY--;
                else cursorY = GySIZE - 1; // Wrap to bottom
            }
            else if (key == KEY_DOWN) {
                if (cursorY < GySIZE - 1) cursorY++;
                else cursorY = 0; // Wrap to top
            }
            else if (key == KEY_LEFT) {
                if (cursorX > 0) cursorX--;
                else cursorX = GxSIZE - 1; // Wrap to right
            }
            else if (key == KEY_RIGHT) {
                if (cursorX < GxSIZE - 1) cursorX++;
                else cursorX = 0; // Wrap to left
            }
        }
        
        // 2. Edit Logic
        else if (key == KEY_ENTER) {
            printf("\n  > Enter new value: ");
            matrix[cursorY][cursorX] = Fvalidinput();
            printf("  Updated!");
            // Loop continues, redrawing the grid with the new value
        }
        
        // 3. Exit Logic
        else if (key == KEY_ESC) {
            return; // Goes back to Edit Menu
        }
    }
}

// ==========================================
//   DEMO LOGIC (With Arrow Nav)
// ==========================================
void Choice_demomatrices(){
    int page = 0;
    const int MAX_PAGES = 5;
    LoadDemo(page); 

    while (1) {
        Banner("DEMO LIBRARY");
        
        // Titles
        const char* titles[] = {
            "3x3 Unique Solution",
            "3x3 No Solution (Inconsistent)",
            "3x3 Infinite Solutions",
            "4x4 Large System",
            "5x5 Diagonally Dominant"
        };
        
        printf("  Page %d / %d: %s\n", page + 1, MAX_PAGES, titles[page]);
        printmatrix();
        DrawLine(40);
        printf("  [<] Prev   [ENTER] Select   [>] Next\n");
        printf("  [ESC] Return to Menu\n");

        int key = _getch();
        
        // FIX: Check for BOTH 0 and 224
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == KEY_LEFT && page > 0) {
                page--;
                LoadDemo(page);
            }
            if (key == KEY_RIGHT && page < MAX_PAGES - 1) {
                page++;
                LoadDemo(page);
            }
        }
        else if (key == KEY_ENTER) {
            printf("\n  Matrix Loaded!\n");
            PauseKey();
            return;
        }
        else if (key == KEY_ESC) {
            GySIZE = 0; GxSIZE = 0;
            return;
        }
    }
}

void LoadDemo(int page) {
    // Clear Matrix
    for(int i=0; i<MatrixMax; i++) for(int j=0; j<MatrixMax+1; j++) matrix[i][j] = 0.0;

    // Load Data
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
//   SOLVER CORE (YOUR LOGIC)
// ==========================================
int comp_execution(){
    if (GxSIZE == 0 || GySIZE == 0) return 1;

    printf("\n  [ Raw Matrix ]\n");
    printmatrix();
    fwdElimination();
    printf("  [ Partial REF ]\n");
    printmatrix();

    if (fabs(Determinant()) < 1e-6) {
        DrawLine(40);
        if (matrix[GySIZE-1][0] != matrix[GySIZE-1][GxSIZE-1])
            printf("  RESULT: NO SOLUTION (Inconsistent)\n");
        else
            printf("  RESULT: INFINITE SOLUTIONS (Dependent)\n");
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
    DrawLine(40);
    int i2 = 0;
    for (int i = GySIZE-1; i >= 0; i--) {
        printf("  %c = %6.2f\n", 122-i, UniqueEqns[i2]);
        i2++;
    }
    printf("\n  Determinant = %.2f\n", Determinant());
}

// ==========================================
//   UTILITIES
// ==========================================
void print_grid(int current_r, int current_c, int mode) {
    printf("\n");
    for(int r = 0; r < GySIZE; r++) {
        printf("    [ ");
        for(int c = 0; c < GxSIZE; c++) {
            if (c == GxSIZE-1) printf("| "); 
            
            // MODE 1: Input (Past=Value, Present=?, Future=.)
            if (mode == 1) { 
                if(r < current_r || (r == current_r && c < current_c))
                    printf("%6.2f ", matrix[r][c]);
                else if(r == current_r && c == current_c)
                    printf("%6c ", '?');
                else
                    printf("%6c ", '.');
            }
            // MODE 2: Target (Hide value, show ?)
            else if (mode == 2) { 
                if(r == current_r && c == current_c)
                    printf("%6c ", '?');
                else
                    printf("%6.2f ", matrix[r][c]);
            }
            // MODE 3: NAVIGATION (Show value, but highlighted)
            else if (mode == 3) {
                if(r == current_r && c == current_c)
                    printf(">%5.2f ", matrix[r][c]); // The > Cursor
                else
                    printf(" %6.2f ", matrix[r][c]); // Normal
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