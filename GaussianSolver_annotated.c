// GAUSSIAN SOLVER
#include <stdio.h>      // Standard Input/Output: used for printf, scanf.
#include <unistd.h>     // Unix Standard: usually for 'sleep', though this code uses system("pause").
#include <math.h>       // Math Library: used later for fabs() and pow().
#include <conio.h>      // Console I/O: used for _getch() (Windows-specific library).

#define MatrixMax 100   // Constant: Sets the hard limit for the matrix size (100x100).

// Global Variables (Accessible by all functions)
float matrix[MatrixMax][MatrixMax+1]; // The main 2D array. +1 column allows space for the "Answers" (Augmented Matrix).
float UniqueEqns[MatrixMax];          // Array to store the final computed values (x, y, z...).
int GySIZE = 0, GxSIZE = 0;           // Tracks the *active* number of Rows (Gy) and Columns (Gx).
int swapTally = 0;                    // Tracks how many row swaps occurred (needed for Determinant sign).

// Function Prototypes (The "Table of Contents" for the compiler)
int main();
    void Choice_inputmatrix();
    int Choice_editmatrix();
        void Edit_choiceDimes();      // Sub-function to edit dimensions.
        void Edit_choiceValues();     // Sub-function to edit specific numbers.
    void Choice_demomatrices();
        void LoadDemo(int page);

int comp_execution();                 // The math manager (Solver).
    void printmatrix();
    int fwdElimination();
        int rowSwap(int h);
    float Determinant();
    void revSubstitution();
    void printsolutions();

void header();                        // Clears screen and prints title.
void print_grid(int current_r, int current_c, int mode); // Visual helper.
int validinput();                     // Input checker (Integers).
float Fvalidinput();                  // Input checker (Floats).

int main(){
    while (1) // Infinite Loop: The program runs forever until "return 0" inside case 5.
    {
        header(); // Visuals: Clear screen, print "Gaussian Solver".
        printf("[1] Input new Matrix\n[2] Edit Matrix\n[3] Demo Matrices \n[4] Run Calculations\n[5] Exit\nSelect: ");
        
        // Takes a validated integer input and jumps to the matching case.
        switch (validinput()) 
        {
        case 1: // Input New Data
            Choice_inputmatrix();
            header();
            print_grid(GySIZE, GxSIZE, 1); // Show the final result of the input.
            system("pause"); // Wait for user to read screen.
            header();
            comp_execution(); // Immediately run the solver after input.
            system("pause");
            break;

        case 2: // Edit Existing Data
            Choice_editmatrix();
            break;

        case 3: // Demo Mode (Previewer)
            Choice_demomatrices();
            break;

        case 4: // Run Math (Manual Trigger)
            system("cls");
            comp_execution();
            system("pause");
            break;

        case 5: // Exit
            printf("Exiting program...\n");
            system("pause");
            return 0; // The only way to break the while(1) loop and close the app.
            break;

        default: // User typed 6, 99, -1, etc.
            printf("Invalid choice, Select from 1-5.\n");
            system("pause");
            break;
        }
    }
}

void Choice_inputmatrix(){
    printf("\n");
    printf("Enter # of Equations: ");
    GySIZE = validinput(); // Sets the active Height (Rows).
    printf("Enter # of Variables (including constants): ");
    GxSIZE = validinput(); // Sets the active Width (Cols).

    // Outer Loop: Iterates through Rows (i)
    for (int i = 0; i < GySIZE; i++) {
        // Inner Loop: Iterates through Columns (j)
        for (int j = 0; j < GxSIZE; j++) { 
            header(); // Clear screen every single input for a "clean" UI.
            
            // Dynamic Label Generation:
            // Prints "z, y, x..." based on how many columns exist.
            printf("Enter values (");
            for (int v = GySIZE-1; v >=0; v--){
                printf("%c, ", 'z'-v); // ASCII math: 'z' minus offset creates previous letters.
            }
            printf("| constant):\n");
            
            // Prints the matrix grid with a cursor '?' at the current [i][j].
            print_grid(i, j, 1); 
            
            // Text Prompt: Checks if we are at the last column (Constant) or a variable.
            if (j < 3) printf("\nEnter Coeff [%d][%d]: ", i+1, j+1);
            else       printf("\nEnter Constant for Row %d: ", i+1);
            
            // Gets float input and stores it in the global matrix.
            matrix[i][j] = Fvalidinput(); 
        }
    }
}

int Choice_editmatrix(){
    // Safety Check: If size is 0, there is no matrix to edit.
    if (GxSIZE == 0 || GySIZE == 0)
    {
        printf("\nUser has not input a matrix yet, choose [1] on the next prompt and try again later.\n");
        system("pause");
        return 1; // Return error code (though main ignores it).
    }
    
    // Edit Menu Loop
    while (1)
    {
        header();
        printmatrix(); // Show current state.
        printf("[1] Edit Dimensions\n[2] Edit Values\n[3] Return to Main Menu\nSelect: ");
        
        switch (validinput())
        {
        case 1:
            Edit_choiceDimes(); // Jump to Dimension Editor.
            break;
        case 2:
            Edit_choiceValues(); // Jump to Value Editor.
            break;
        case 3:
            return 0; // Break the loop and go back to Main.
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
    case 1: // Reset Size completely (Note: Does not clear old data in memory, just changes the limit).
        printf("Enter # of Equations: ");
        GySIZE = validinput();
        printf("Enter # of Variables (including constants): ");
        GxSIZE = validinput();
        break;
    case 2: // Append Rows
        printf("Enter # of Equations to add: ");
        int y_add = validinput();
        GySIZE += y_add; // Increases the active Row count.
        break;
    case 3: // Append Columns
        printf("Enter # of Variables to add: ");
        int x_add = validinput();
        GxSIZE += x_add; // Increases the active Column count.
        break;
    default:
        printf("Invalid choice, Select from 1-3 later.\n");
        break;
    }
}

void Edit_choiceValues(){
    int X_index, Y_index;
    print_grid(GySIZE, GxSIZE, 1); // Show full grid.
    printf("\nNote matrix index starts at [1][1]\n");

    // Loop 1: Validate Row Index
    while (1)
    {
        header();
        print_grid(GySIZE, GxSIZE, 1);
        printf("Enter # i: ");
        Y_index = validinput();
        Y_index -= 1; // Convert User Input (1-based) to C Array Index (0-based).
        
        // Bounds Checking
        if (Y_index < 0 || Y_index > GySIZE-1)
        {
            printf("Cannot Access value outside matrix dimension.\n");
            system("pause");
        }
        else
        {
            break; // Valid index found.
        }
    }

    // Loop 2: Validate Column Index
    while (1)
    {
        header();
        print_grid(GySIZE, GxSIZE, 1);
        printf("Enter # j: ");
        X_index = validinput();
        X_index -= 1; // Convert User Input (1-based) to C Array Index (0-based).
        
        // Bounds Checking
        if (X_index < 0 || X_index > GxSIZE-1)
        {
            printf("Cannot Access value outside matrix dimension.\n");
            system("pause");
        }
        else
        {
            break; // Valid index found.
        }
    }

    // Input Phase
    header();
    printf("Enter values (");
    // Visual label Helper
    for (int v = GySIZE-1; v >=0; v--){
        printf("%c, ", 'z'-v);
    }
    printf("| constant):\n");
    
    // Print grid in "Mode 2" (likely highlights the specific cell being edited).
    print_grid(Y_index, X_index, 2); 
    
    if (X_index < 3) printf("\nEnter Coeff [%d][%d]: ", Y_index+1, X_index+1);
    else       printf("\nEnter Constant for Row %d: ", Y_index+1);
    
    // Overwrite the specific cell.
    matrix[Y_index][X_index] = Fvalidinput(); 
    
    header();
    print_grid(GySIZE, GxSIZE, 1); // Confirmation display.
}

void Choice_demomatrices(){
    int page = 0;             // Tracks which demo is currently visible (0-indexed).
    const int MAX_PAGES = 5;  // Total number of available demos.

    // Pre-load the first page (Page 0) so the user sees data immediately upon entering this menu.
    LoadDemo(page); 

    // UI Loop: Keeps redrawing the screen until the user Selects or Cancels.
    while (1) {
        header(); // Clears screen.
        printf("==== DEMO PREVIEW (Page %d / %d) ====\n\n", page + 1, MAX_PAGES);
        
        // Dynamic Title: Prints a specific header based on the current page number.
        switch (page)
        {
        case 0:
            printf("==== 3x3 System With Unique Solutions ====\n\n");
            break;
        case 1:
            printf("==== 3x3 System With No Solutions ====\n\n");
            break;
        case 2:
            printf("==== 3x3 System With Infinite Solutions ====\n\n");
            break;
        case 3:
            printf("==== 4x4 System ====\n\n");
            break;
        case 4:
            printf("==== 5x5 System ====\n\n");
            break;
        }

        // Visual Feedback: Prints the matrix currently stored in the global variables.
        // Since LoadDemo() updated the global matrix, this prints the correct demo.
        printmatrix(); 

        printf("\n----------------------------------------\n");
        
        // Navigation Bar Logic:
        // Only show "< PREV" if we are NOT on the first page.
        if (page > 0)             printf("[1] < PREV   ");
        else                      printf("             "); // Print empty space to keep layout stable.
        
        printf("[2] SELECT");
        
        // Only show "NEXT >" if we are NOT on the last page.
        if (page < MAX_PAGES - 1) printf("   [3] NEXT >");
        else                      printf("             "); // Print empty space to keep layout stable.

        printf("\n\n[ESC] Cancel / Return to Menu\n");

        // INSTANT INPUT: _getch() waits for a keypress. 
        // Unlike scanf(), it does not echo the character to the screen and doesn't need Enter.
        char key = _getch(); 

        // Input Logic
        if (key == '1') { // Previous Page
            if (page > 0) {
                page--;
                LoadDemo(page); // Important: Reloads the global matrix with the new page's data immediately.
            }
        }
        else if (key == '3') { // Next Page
            if (page < MAX_PAGES - 1) {
                page++;
                LoadDemo(page); // Reloads data.
            }
        }
        else if (key == '2') { // Select / Confirm
            printf("\nMatrix Loaded Successfully!\n");
            system("pause"); // User feedback pause.
            return;   // Exits the function. The global 'matrix' now holds the chosen demo data.
        }
        else if (key == 27) { // ASCII 27 = ESC Key.
            // If user cancels, we clear the sizes so the Main Menu knows nothing is loaded.
            GySIZE = 0; GxSIZE = 0;
            return;
        }
    }
}

void LoadDemo(int page) {
    // 1. Wipe the Slate:
    // Sets every single cell in the 100x101 global matrix to 0.0 to prevent leftover data ghosts.
    for(int i=0; i<MatrixMax; i++) 
        for(int j=0; j<MatrixMax+1; j++) matrix[i][j] = 0.0;

    // 2. Load the Data:
    switch (page) {
        case 0: // PAGE 1: Unique Solution (3x3)
            GySIZE = 3; GxSIZE = 4; // Set dimensions first.
            // Create a local temporary array with the hardcoded values.
            float p1[3][4] = {
                { 0.0,  2.0,  1.0,   7.0}, 
                { 1.0,  1.0,  1.0,   6.0}, 
                { 3.0,  2.0, -1.0,   4.0}
            };
            // Copy the local array into the Global 'matrix'.
            for(int i=0; i<3; i++) 
                for(int j=0; j<4; j++) matrix[i][j] = p1[i][j];
            break;

        case 1: // PAGE 2: No Solution (3x3)
            GySIZE = 3; GxSIZE = 4;
            float p2[3][4] = {
                { 1.0,  1.0,  1.0,   6.0}, 
                { 2.0,  2.0,  2.0,  12.0}, // Row 2 is 2x Row 1...
                { 2.0,  2.0,  2.0,  20.0}  // ...but equals 20 instead of 12. Contradiction.
            };
            for(int i=0; i<3; i++) 
                for(int j=0; j<4; j++) matrix[i][j] = p2[i][j];
            break;

        case 2: // PAGE 3: Infinite Solutions (3x3)
            GySIZE = 3; GxSIZE = 4;
            float p3[3][4] = {
                { 1.0,  2.0,  3.0,  14.0}, 
                { 1.0, -1.0,  1.0,   6.0}, 
                { 2.0,  1.0,  4.0,  20.0} // This row is sum of R1 + R2 (Dependent).
            };
            for(int i=0; i<3; i++) 
                for(int j=0; j<4; j++) matrix[i][j] = p3[i][j];
            break;

        case 3: // PAGE 4: 4x4 System
            GySIZE = 4; GxSIZE = 5;
            float p4[4][5] = {
                { 1.0,  1.0,  1.0,  1.0,   10.0}, // w + x + y + z = 10
                { 1.0, -1.0,  2.0,  0.0,    5.0}, 
                { 0.0,  1.0, -1.0,  3.0,   11.0}, 
                { 2.0,  2.0,  0.0, -1.0,    2.0}  
            };
            for(int i=0; i<4; i++) 
                for(int j=0; j<5; j++) matrix[i][j] = p4[i][j];
            break;

        case 4: // PAGE 5: 5x5 System (Diagonally Dominant)
            GySIZE = 5; GxSIZE = 6;
            float p5[5][6] = {
                { 5.0,  1.0,  1.0,  1.0,  1.0,   19.0}, // Pivot is 5.0 (Strong)
                { 1.0,  5.0,  1.0,  1.0,  1.0,   23.0},
                { 1.0,  1.0,  5.0,  1.0,  1.0,   27.0},
                { 1.0,  1.0,  1.0,  5.0,  1.0,   31.0},
                { 1.0,  1.0,  1.0,  1.0,  5.0,   35.0} 
            };
            for(int i=0; i<5; i++) 
                for(int j=0; j<6; j++) matrix[i][j] = p5[i][j];
            break;
    }
}

int comp_execution(){
    // 1. Pre-Flight Check:
    // If the matrix dimensions are 0, no data exists. Abort to prevent crashes.
    if (GxSIZE == 0 || GySIZE == 0)
    {
        printf("\nUser has not input a matrix yet, choose [1] on the next prompt and try again later.\n");
        return 1; // Return error code.
    }

    // 2. Show Initial State:
    printf("==== Raw Matrix ====\n");
    printmatrix(); 

    // 3. Step 1: Forward Elimination (Gaussian Elimination):
    // This converts the matrix into "Row Echelon Form" (zeros below the diagonal).
    fwdElimination(); 

    // 4. Show Intermediate State:
    printf("==== Partial REF ====\n");
    printmatrix(); 

    // 5. Step 2: Analyze Solvability (The Determinant Check):
    // If the Determinant is effectively zero (fabs < 1e-6), the matrix is "Singular".
    // This means it has either NO solution or INFINITE solutions.
    if (fabs(Determinant()) < 1e-6)
    {
        // Check the bottom row for a contradiction (e.g., 0x + 0y = 5).
        // If the variable side (left) is 0 but the constant side (right) is NOT 0...
        if (matrix[GySIZE-1][0] != matrix[GySIZE-1][GxSIZE-1]) 
        {
            printf("This matrix has NO Solutions\n"); // Parallel planes.
        }
        else
        {
            printf("This matrix has INFINITE Solutions\n"); // Overlapping planes.
        }
    }
    else
    {
        // 6. Step 3: Back Substitution:
        // If the matrix is solvable (Determinant != 0), find the values for x, y, z.
        revSubstitution();
        printsolutions();
    }
}

void printmatrix(){
    // Loop through Rows
    for (size_t i = 0; i < GySIZE; i++)
    {
        // Loop through Columns
        for (size_t j = 0; j < GxSIZE; j++)
        {   
            // Visual Divider:
            // If we are at the last column, print a vertical bar to separate constants.
            if (j == GxSIZE-1)
            {
                printf("| ");
            }
            
            // Print the number formatted to 2 decimal places with 6 spaces of padding.
            printf("%6.2f  ",matrix[i][j]);
        }
        printf("\n"); // Newline at the end of each row.
    }
    printf("\n");
}

int fwdElimination(){
    // Outer Loop: 'h' is the Pivot Row (Current Diagonal position).
    for (size_t h = 0; h < GySIZE - 1; h++)
    {
        // Pivot Check:
        // If the number on the diagonal is 0, we can't divide by it.
        // We must swap this row with a non-zero row below it.
        if (fabs(matrix[h][h]) < 1e-6)
        {
            rowSwap(h);
        }

        float pvt1 = matrix[h][h]; // The "Anchor" value (Diagonal).

        // Inner Loop: 'i' iterates through all rows BELOW the pivot 'h'.
        for (size_t i = h + 1; i < GySIZE; i++)
        {
            float pvt2 = matrix[i][h]; // The value we want to kill (turn to 0).
            
            // Safety Catch:
            // If both the pivot and the target are 0, something is wrong or redundant.
            if (pvt2 == 0 && pvt1 == 0)
            {
                printf("cannot calculate Partial REF due to division by 0.\n");
                return 1;
            }
            
            // The Gaussian Formula:
            // Ratio = Target / Pivot
            float ratio = pvt2 / pvt1;
            
            // Apply this subtraction to the ENTIRE row 'i'.
            for (size_t j = 0; j < GxSIZE; j++)
            {   
                // Row_i = Row_i - (Ratio * Row_h)
                matrix[i][j] = matrix[i][j] - (ratio * matrix[h][j]);
            }
        }
    }
    return 0; // Success.
}

int rowSwap(int h){
    swapTally++; // INCREMENT TALLY: Essential for correcting the Determinant sign later.
    
    // Search rows below 'h'
    for (size_t i = h + 1; i < GySIZE; i++)
    {
        // Found a row where the value in column 'h' is NOT zero?
        if (fabs(matrix[i][h]) > 1e-6)
        {
            // Swap loop: Move every number from Row 'h' to 'i' and vice versa.
            for (size_t j = 0; j < GxSIZE; j++)
            {
                float temp = matrix[h][j];
                matrix[h][j] = matrix[i][j];
                matrix[i][j] = temp;
            }
            break; // Stop after the first successful swap.
        }
    }
}

float Determinant(){
    float Det = 1;
    // For a triangular matrix, the Determinant is just the product of the diagonal.
    for (size_t i = 0; i < GySIZE; i++)
    {
        Det = Det * matrix[i][i];
    }
    
    // The "Swap Tax":
    // Every row swap flips the sign of the determinant.
    // We multiply by (-1)^swaps to correct it.
    Det = Det * pow(-1, swapTally);
    
    return Det;
}

void revSubstitution(){
    // Loop backwards from the last row up to the first.
    for (int i = GySIZE-1; i >= 0 ; i--)
    {
        float bucket = matrix[i][GxSIZE-1]; // Start with the Constant (Right side of |).
        
        // Subtract the values of variables we ALREADY know (from rows below).
        for (int j = i + 1; j < GySIZE; j++)
        {
            bucket = bucket - (matrix[i][j] * UniqueEqns[j]);
        }
        
        // Divide by the coefficient of the current variable to isolate it.
        // x = (Constant - knowns) / coefficient
        UniqueEqns[i] = bucket / matrix[i][i];
    }
}

void printsolutions(){
    int i2 = 0; // Index for the UniqueEqns array.
    
    // Loop backwards to print z, y, x... or x4, x3, x2, x1.
    for (int i = GySIZE-1; i >= 0; i--)
    {
        // ASCII Logic: 
        // 122 is 'z'. 122-0='z', 122-1='y', 122-2='x'.
        printf("%c = %6.2f\n", 122-i, UniqueEqns[i2]);
        i2++;
    }
    
    // Print the final Determinant as a bonus info.
    int det = Determinant();
    printf("\nDeterminant = %d\n", det);
}

void header(){
    system("cls"); // Windows Command: Clears the terminal text.
    printf("==== Gaussian Solver ====\n"); // Prints the title banner.
}

// Parameters:
// current_r, current_c: The coordinates of the "Active Cursor".
// mode: 1 = Input Sequence (Past is numbers, Future is dots), 2 = Edit Mode (All numbers, one target).
void print_grid(int current_r, int current_c, int mode) {
    printf("\n");
        // Loop through all Rows
        for(int r = 0; r < GySIZE; r++) {
            printf("  [ ");
            // Loop through all Columns
            for(int c = 0; c < GxSIZE; c++) {
                
                // Visual Separator:
                // If this is the last column, print the pipe symbol | to denote the constants.
                if (c == GxSIZE-1) printf("| "); 
                
                // === MODE 1: SEQUENTIAL INPUT ===
                if (mode == 1)
                {
                    // Case A: Past (We passed this cell)
                    // If row is less than current, OR same row but column is less...
                    if(r < current_r || (r == current_r && c < current_c)) {
                        printf("%6.2f ", matrix[r][c]); // Show the number we already typed.
                    }
                    // Case B: Present (We are here)
                    else if(r == current_r && c == current_c) {
                        printf("%6c ", '?'); // Show a Question Mark cursor.
                    }
                    // Case C: Future (We haven't reached here yet)
                    else {
                        printf("%6c ", '.'); // Show a dot placeholder.
                    }
                }
                
                // === MODE 2: EDIT MODE ===
                else if (mode == 2) 
                {
                    // If this is the specific cell selected for editing...
                    if(r == current_r && c == current_c) { 
                        printf("%6c ", '?'); // Mask it with '?' to show it's being changed.
                    }
                    else {
                        printf("%6.2f ", matrix[r][c]); // Otherwise show the existing number.
                    }
                }
                
            }
            printf("]\n"); // End of Row bracket.
        }
    printf("\n");
}

int validinput(){
    int valid = 0; // Flag: 0 = Invalid, 1 = Good to go.
    int input;

    // "Do...While" ensures the code runs at least once.
    do {
        // scanf returns the number of items successfully read.
        // If it returns 0 (or anything not 1), it means it failed to find an integer.
        if (scanf(" %d", &input) != 1) {
            printf("Invalid input.\nenter value: ");
            
            // THE BUFFER FLUSH:
            // When scanf fails, the bad letter stays in the input stream (stdin).
            // We loop through getchar() to "eat" every character until we hit a Newline (\n) or End of File.
            // This clears the stream so we can try scanf again cleanly.
            int c; while ((c = getchar()) != '\n' && c != EOF); 
            
            continue; // Jump back to the start of the 'do' loop.
        }
        valid = 1; // If scanf succeeded, set flag to true.
    } while (!valid);
    
    return input; // Send the clean integer back to main.
}

float Fvalidinput(){
    int valid = 0;
    float input; // Variable type is now float.

    do {
        // We look for "%f" (float) instead of "%d".
        if (scanf(" %f", &input) != 1) {
            printf("Invalid input.\nenter value: ");
            // Flush the buffer (Eat the bad characters).
            int c; while ((c = getchar()) != '\n' && c != EOF); 
            continue; 
        }
        valid = 1;
    } while (!valid);
    
    return input;
}