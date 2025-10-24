#include <unistd.h>
#include <stdlib.h> // For NULL and malloc

// Struct definition
typedef struct s_Node
{
    int             Data;
    struct s_Node   *Right;
    struct s_Node   *Left;
    struct s_Node   *Up;
    struct s_Node   *Down;
} Node;


Node *StartPoint00 = NULL;


void    print_grid(void);
int     parse_clues(char *str, int *clues_out);
int     solve(void);
void    InitialValue(int col1up, int col2up, int col3up, int col4up,
                     int col1down, int col2down, int col3down, int col4down,
                     int row1left, int row2left, int row3left, int row4left,
                     int row1right, int row2right, int row3right, int row4right);
int     calculate_view(Node *node, int is_row, int is_reverse);
int     check_all_clues(void);
int     is_valid_placement(Node *node, int num);
Node    *find_empty_cell(void);



void    InitialValue(int col1up, int col2up, int col3up, int col4up,
                     int col1down, int col2down, int col3down, int col4down,
                     int row1left, int row2left, int row3left, int row4left,
                     int row1right, int row2right, int row3right, int row4right)
{
    Node *prevRowStart = NULL;
    Node *CurrentRowStart = NULL;
    Node *NodeUp = NULL;
    Node *NodeLeft = NULL;
    StartPoint00 = NULL; 

    for (int i = 0; i < 6; i++) // Rows
    {
        NodeLeft = NULL;
        CurrentRowStart = NULL;
        NodeUp = prevRowStart; 

        for (int j = 0; j < 6; j++) // Columns
        {
            Node *node = (Node*)malloc(sizeof(Node));
            node->Data = 0; 
            node->Right = NULL;
            node->Down = NULL;

            if (StartPoint00 == NULL) 
                StartPoint00 = node;
            
            if (CurrentRowStart == NULL)
                CurrentRowStart = node;

            // Link Left / Right
            node->Left = NodeLeft;
            if (NodeLeft != NULL)
                NodeLeft->Right = node;

            // Link Up / Down
            node->Up = NodeUp;
            if (NodeUp != NULL)
            {
                NodeUp->Down = node;
                NodeUp = NodeUp->Right; 
            }
            NodeLeft = node;
        }
        prevRowStart = CurrentRowStart; 
    }
    
    // this one for Moving 
    Node *current;

    // Top row 
    current = StartPoint00->Right; 
    current->Data = col1up;
    current = current->Right; 
    current->Data = col2up;
    current = current->Right; 
    current->Data = col3up;
    current = current->Right; 
    current->Data = col4up;

    // Bottom row
    current = StartPoint00->Down->Down->Down->Down->Down; 
    current = current->Right; 
    current->Data = col1down;
    current = current->Right; 
    current->Data = col2down;
    current = current->Right; 
    current->Data = col3down;
    current = current->Right; 
    current->Data = col4down;

    // Left column 
    current = StartPoint00->Down; 
    current->Data = row1left;
    current = current->Down; 
    current->Data = row2left;
    current = current->Down; 
    current->Data = row3left;
    current = current->Down; 
    current->Data = row4left;

    // Right column
    current = StartPoint00->Right->Right->Right->Right->Right; 
    current = current->Down; 
    current->Data = row1right;
    current = current->Down; 
    current->Data = row2right;
    current = current->Down; 
    current->Data = row3right;
    current = current->Down; 
    current->Data = row4right;
    
    //corne = null
    StartPoint00->Data = -1; 
    StartPoint00->Right->Right->Right->Right->Right->Data = -1;
    StartPoint00->Down->Down->Down->Down->Down->Data = -1; 
    StartPoint00->Down->Down->Down->Down->Down->Right->Right->Right->Right->Right->Data = -1;
}


int calculate_view(Node *node, int isRow, int isReverse)
{
    int MaxHeight = 0;
    int CountVisibleBuilding = 0;
    for (int i = 0; i < 4; i++)
    {
        if (node->Data > MaxHeight)
        {
            MaxHeight = node->Data;
            CountVisibleBuilding++;
        }
        if (isRow)
            node = isReverse ? node->Left : node->Right;
        else
            node = isReverse ? node->Up : node->Down;
    }
    return (CountVisibleBuilding);
}

int check_all_clues(void)
{
    Node *StartNode;
    Node *StartData = StartPoint00->Down->Right;
    
    // Check all 4 rows
    StartNode = StartData;
    for (int i = 0; i < 4; i++)
    {
        if (calculate_view(StartNode, 1, 0) != StartNode->Left->Data) return 0;
        
        Node *EndNodeRow = StartNode->Right->Right->Right;
        if (calculate_view(EndNodeRow, 1, 1) != EndNodeRow->Right->Data) return 0;
        
        StartNode = StartNode->Down; 
    }
    
    // Check all 4 columns
    StartNode = StartData;
    for (int i = 0; i < 4; i++)
    {
        if (calculate_view(StartNode, 0, 0) != StartNode->Up->Data) return 0;
        
        Node *ColumnEndNode = StartNode->Down->Down->Down;
        if (calculate_view(ColumnEndNode, 0, 1) != ColumnEndNode->Down->Data) return 0;
        
        StartNode = StartNode->Right; 
    }
    return (1);
}

int is_valid_placement(Node *node, int num)
{
    Node *current;

    // Check row
    current = node;
    while (current->Left != NULL && current->Left->Data != -1)
    {
        current = current->Left;
    }
    current = current->Right;
    for (int i = 0; i < 4; i++)
    {
        if (current->Data == num) 
            return (0);
        current = current->Right;
    }

    // Check column
    current = node;
    while (current->Up != NULL && current->Up->Data != -1)
    {
        current = current->Up;
    }
    current = current->Down;
    for (int i = 0; i < 4; i++)
    {
        if (current->Data == num) 
            return (0);
        current = current->Down;
    }
    
    return (1);
}

Node *find_empty_cell(void)
{
    Node *row_start = StartPoint00->Down->Right; 
    
    for (int i = 0; i < 4; i++)
    {
        Node *current = row_start;
        for (int j = 0; j < 4; j++)
        {
            if (current->Data == 0)
                return (current);
            current = current->Right;
        }
        row_start = row_start->Down; 
    }
    return (NULL); 
}

// --- The Core Backtracking Solver ---

int solve(void)
{
    Node *empty_node = find_empty_cell();

    if (empty_node == NULL)
    {
        return (check_all_clues());
    }

    for (int num = 1; num <= 4; num++)
    {
        if (is_valid_placement(empty_node, num))
        {
            empty_node->Data = num;
            if (solve())
            {
                return (1);
            }
            empty_node->Data = 0;
        }
    }
    return (0);
}

// --- Input/Output Functions ---

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
            return (0);
        }
        str++;
    }
    return (count == 16);
}

void print_grid(void)
{
    char buffer[8];
    Node *row_start = StartPoint00->Down->Right;
    
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
        
        row_start = row_start->Down;
    }
}



int main(int argc, char **argv)
{
    int clues[16];

    parse_clues(argv[1], clues);

    InitialValue(clues[0], clues[1], clues[2], clues[3],
                 clues[4], clues[5], clues[6], clues[7],
                 clues[8], clues[9], clues[10], clues[11],
                 clues[12], clues[13], clues[14], clues[15]);
   
    solve();
    print_grid();

    
    return (0);
}

