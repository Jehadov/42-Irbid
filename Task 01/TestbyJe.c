#include <unistd.h>
#include <stdlib.h> // For malloc and NULL

// Correct C struct definition. We use a typedef for convenience.
typedef struct s_Node
{
    int             Data; // 0=empty, 1-4=skyscraper, -1=corner, or clue value
    struct s_Node   *Right;
    struct s_Node   *Left;
    struct s_Node   *Up;
    struct s_Node   *Down;
} Node;

// A global pointer to the top-left corner of our 6x6 grid.
Node *g_grid_start = NULL;

// --- Function Prototypes ---
void    print_grid(void);
int     parse_clues(char *str, int *clues_out);
int     solve(void);
void    free_grid(void);

// --- Grid Initialization (Your Function) ---

/**
 * @brief Creates the 6x6 grid of Nodes and populates the
 * outer border with the 16 clues.
 */
void    InitialValue(int col1up, int col2up, int col3up, int col4up,
                     int col1down, int col2down, int col3down, int col4down,
                     int row1left, int row2left, int row3left, int row4left,
                     int row1right, int row2right, int row3right, int row4right)
{
    // Temporary 2D array *of pointers* to build the grid
    Node *nodes[6][6];

    // 1. Malloc all 36 nodes
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            nodes[i][j] = (Node*)malloc(sizeof(Node));
            if (nodes[i][j] == NULL)
                return; // Malloc failed
        }
    }

    // 2. Link all 36 nodes together
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            nodes[i][j]->Data = 0; // Default data
            nodes[i][j]->Up =    (i > 0) ? nodes[i - 1][j] : NULL;
            nodes[i][j]->Down =  (i < 5) ? nodes[i + 1][j] : NULL;
            nodes[i][j]->Left =  (j > 0) ? nodes[i][j - 1] : NULL;
            nodes[i][j]->Right = (j < 5) ? nodes[i][j + 1] : NULL;
        }
    }

    // 3. Set the global start pointer
    g_grid_start = nodes[0][0];

    // 4. Populate the clue nodes (outer border)
    nodes[0][1]->Data = col1up;
    nodes[0][2]->Data = col2up;
    nodes[0][3]->Data = col3up;
    nodes[0][4]->Data = col4up;

    nodes[5][1]->Data = col1down;
    nodes[5][2]->Data = col2down;
    nodes[5][3]->Data = col3down;
    nodes[5][4]->Data = col4down;

    nodes[1][0]->Data = row1left;
    nodes[2][0]->Data = row2left;
    nodes[3][0]->Data = row3left;
    nodes[4][0]->Data = row4left;

    nodes[1][5]->Data = row1right;
    nodes[2][5]->Data = row2right;
    nodes[3][5]->Data = row3right;
    nodes[4][5]->Data = row4right;
    
    // 5. Set corner nodes to -1 (sentinel value)
    nodes[0][0]->Data = -1;
    nodes[0][5]->Data = -1;
    nodes[5][0]->Data = -1;
    nodes[5][5]->Data = -1;
}

// --- Solver Helper Functions ---

/**
 * @brief Calculates the view from one direction for a given line.
 * @param node The starting node of the data line (e.g., [1][1] for row 1).
 * @param is_row 1 if checking a row, 0 if checking a column.
 * @param is_reverse 0 for left-to-right/top-to-bottom, 1 for reverse.
 * @return The number of visible skyscrapers.
 */
int calculate_view(Node *node, int is_row, int is_reverse)
{
    int max_height = 0;
    int visible_count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (node->Data > max_height)
        {
            max_height = node->Data;
            visible_count++;
        }
        if (is_row)
            node = is_reverse ? node->Left : node->Right;
        else
            node = is_reverse ? node->Up : node->Down;
    }
    return (visible_count);
}

/**
 * @brief Checks if the fully-solved grid matches all clues.
 * @return 1 if all clues match, 0 otherwise.
 */
int check_all_clues(void)
{
    Node *start_node;

    // Get the [1][1] node
    Node *top_left_data = g_grid_start->Down->Right;
    
    // Check all 4 rows (left and right clues)
    start_node = top_left_data;
    for (int i = 0; i < 4; i++)
    {
        if (calculate_view(start_node, 1, 0) != start_node->Left->Data) return 0;
        
        Node *row_end_node = start_node->Right->Right->Right;
        if (calculate_view(row_end_node, 1, 1) != row_end_node->Right->Data) return 0;
        
        start_node = start_node->Down; // Move to the next row
    }
    
    // Check all 4 columns (up and down clues)
    start_node = top_left_data;
    for (int i = 0; i < 4; i++)
    {
        if (calculate_view(start_node, 0, 0) != start_node->Up->Data) return 0;
        
        Node *col_end_node = start_node->Down->Down->Down;
        if (calculate_view(col_end_node, 0, 1) != col_end_node->Down->Data) return 0;
        
        start_node = start_node->Right; // Move to the next column
    }
    return (1); // All checks passed
}

/**
 * @brief Checks if placing 'num' at 'node' is valid (no duplicates).
 * This function works by "walking" the linked list.
 */
int is_valid_placement(Node *node, int num)
{
    Node *current;

    // Check row for duplicates
    current = node;
    while (current->Left->Data != -1) // Go to start of data row
        current = current->Left;
    for (int i = 0; i < 4; i++)
    {
        if (current->Data == num) return (0);
        current = current->Right;
    }

    // Check column for duplicates
    current = node;
    while (current->Up->Data != -1) // Go to start of data col
        current = current->Up;
    for (int i = 0; i < 4; i++)
    {
        if (current->Data == num) return (0);
        current = current->Down;
    }
    return (1);
}

/**
 * @brief Finds the next empty (Data == 0) cell in the 4x4 grid.
 * @return Pointer to the empty Node, or NULL if grid is full.
 */
Node *find_empty_cell(void)
{
    Node *row_start = g_grid_start->Down->Right; // Start at [1][1]
    
    for (int i = 0; i < 4; i++)
    {
        Node *current = row_start;
        for (int j = 0; j < 4; j++)
        {
            if (current->Data == 0)
                return (current);
            current = current->Right;
        }
        row_start = row_start->Down; // Go to next row
    }
    return (NULL); // No empty cells
}

// --- The Core Backtracking Solver ---

int solve(void)
{
    // 1. Find the next empty cell
    Node *empty_node = find_empty_cell();

    // 2. Base Case: If no cells are empty, check if it's a valid solution
    if (empty_node == NULL)
    {
        return (check_all_clues());
    }

    // 3. Recursive Step: Try numbers 1-4
    for (int num = 1; num <= 4; num++)
    {
        // Check if this number is valid in its row/col
        if (is_valid_placement(empty_node, num))
        {
            empty_node->Data = num; // Place the number

            // Recursively call solve for the *next* empty cell
            if (solve())
            {
                return (1); // Found a solution!
            }
            
            // If the recursive call didn't lead to a solution, backtrack.
            empty_node->Data = 0; // Reset the cell to empty
        }
    }
    
    // 4. If no number 1-4 worked, trigger backtracking
    return (0);
}

// --- Main Function and I/O ---

int main(int argc, char **argv)
{
    int clues[16];

    if (argc != 2 || !parse_clues(argv[1], clues))
    {
        write(1, "Error\n", 6);
        return (1);
    }
    
    // This is the function you provided. We call it with the parsed clues.
    // This builds our global g_grid_start
    InitialValue(clues[0], clues[1], clues[2], clues[3],
                 clues[4], clues[5], clues[6], clues[7],
                 clues[8], clues[9], clues[10], clues[11],
                 clues[12], clues[13], clues[14], clues[15]);

    if (g_grid_start == NULL) // Check if malloc failed
    {
         write(1, "Error\n", 6);
         return (1);
    }

    // Attempt to solve the puzzle
    if (solve())
    {
        print_grid();
    }
    else
    {
        write(1, "Error\n", 6);
    }
    
    // Clean up all malloc'd memory
    free_grid();
    return (0);
}

/**
 * @brief Parses the input string "4 3 2 1..." into a 16-int array
 */
int parse_clues(char *str, int *clues_out)
{
    int count = 0;
    while (*str && count < 16)
    {
        if (*str >= '1' && *str <= '4')
        {
            clues_out[count] = *str - '0';
            count++;
        }
        else if (*str != ' ')
        {
            return (0); // Invalid character
        }
        str++;
    }
    return (count == 16); // Must have exactly 16 clues
}

/**
 * @brief Prints the final 4x4 data grid using write()
 */
void print_grid(void)
{
    char buffer[8]; // "1 2 3 4\n"
    
    // Start at the first data node [1][1]
    Node *row_start = g_grid_start->Down->Right;
    
    for (int i = 0; i < 4; i++)
    {
        Node *current = row_start;
        for (int j = 0; j < 4; j++)
        {
            buffer[j * 2] = current->Data + '0';
            if (j < 3)
            {
                buffer[j * 2 + 1] = ' ';
            }
            current = current->Right;
        }
        buffer[7] = '\n';
        write(1, buffer, 8);
        
        row_start = row_start->Down; // Move to next row
    }
}

/**
 * @brief Frees all 36 malloc'd nodes
 */
void free_grid(void)
{
    Node *row_start = g_grid_start;
    while (row_start != NULL)
    {
        Node *current = row_start;
        Node *next_row_start = row_start->Down;
        while (current != NULL)
        {
            Node *next_node = current->Right;
            free(current);
            current = next_node;
        }
        row_start = next_row_start;
    }
    g_grid_start = NULL;
}

