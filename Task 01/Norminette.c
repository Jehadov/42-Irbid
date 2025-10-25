#include <unistd.h>
#include <stdlib.h> // For NULL and malloc

// Struct definition
typedef struct s_Node
{
    int           Data;
    struct s_Node   *Right;
    struct s_Node   *Left;
    struct s_Node   *Up;
    struct s_Node   *Down;
} Node;


Node *StartPoint00 = NULL;


void    print(void);
int     parseOutSideDigit(char *str, int *OutSideDigit);
int     solve(void);
void    InitialValue(int col1up, int col2up, int col3up, int col4up,
                     int col1down, int col2down, int col3down, int col4down,
                     int row1left, int row2left, int row3left, int row4left,
                     int row1right, int row2right, int row3right, int row4right);
int     calculate(Node *node, int is_row, int is_reverse);
int     checkAllOutSideDigit(void);
int     isValid(Node *node, int num);
Node    *findEmptyCell(void);
void    freeGrid(void); /* Added prototype for freeGrid */



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

    int i = 0; // Rows
    while (i < 6)
    {
        NodeLeft = NULL;
        CurrentRowStart = NULL;
        NodeUp = prevRowStart; 

        int j = 0; // Columns
        while (j < 6)
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
            j++;
        }
        prevRowStart = CurrentRowStart; 
        i++;
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


int calculate(Node *node, int isRow, int isReverse)
{
    int MaxHeight = 0;
    int CountVisibleBuilding = 0;
    int i = 0;
    while (i < 4)
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
        i++;
    }
    return (CountVisibleBuilding);
}

int checkAllOutSideDigit(void)
{
    Node *StartNode;
    Node *StartData = StartPoint00->Down->Right;
    
    // Check all 4 rows
    StartNode = StartData;
    int i_row = 0;
    while (i_row < 4)
    {
        if (calculate(StartNode, 1, 0) != StartNode->Left->Data) return 0;
        
        Node *EndNodeRow = StartNode->Right->Right->Right;
        if (calculate(EndNodeRow, 1, 1) != EndNodeRow->Right->Data) return 0;
        
        StartNode = StartNode->Down; 
        i_row++;
    }
    
    // Check all 4 columns
    StartNode = StartData;
    int i_col = 0;
    while (i_col < 4)
    {
        if (calculate(StartNode, 0, 0) != StartNode->Up->Data) return 0;
        
        Node *ColumnEndNode = StartNode->Down->Down->Down;
        if (calculate(ColumnEndNode, 0, 1) != ColumnEndNode->Down->Data) return 0;
        
        StartNode = StartNode->Right; 
        i_col++;
    }
    return (1);
}

int isValid(Node *node, int num)
{
    Node *current;

    // Check row
    current = node;
    while (current->Left != NULL && current->Left->Data != -1)
    {
        current = current->Left;
    }
    current = current->Right;
    int i_row = 0;
    while (i_row < 4)
    {
        if (current->Data == num) 
            return (0);
        current = current->Right;
        i_row++;
    }

    // Check column
    current = node;
    while (current->Up != NULL && current->Up->Data != -1)
    {
        current = current->Up;
    }
    current = current->Down;
    int i_col = 0;
    while (i_col < 4)
    {
        if (current->Data == num) 
            return (0);
        current = current->Down;
        i_col++;
    }
    
    return (1);
}

Node *findEmptyCell(void)
{
    Node *rowStart = StartPoint00->Down->Right; 
    
    int i = 0;
    while (i < 4)
    {
        Node *current = rowStart;
        int j = 0;
        while (j < 4)
        {
            if (current->Data == 0)
                return (current);
            current = current->Right;
            j++;
        }
        rowStart = rowStart->Down; 
        i++;
    }
    return (NULL); 
}


int solve(void)
{
    Node *EmptyNode = findEmptyCell();

    if (EmptyNode == NULL)
    {
        return (checkAllOutSideDigit());
    }

    int num = 1;
    while (num <= 4)
    {
        if (isValid(EmptyNode, num))
        {
            EmptyNode->Data = num;
            if (solve())
            {
                return (1);
            }
            EmptyNode->Data = 0;
        }
        num++;
    }
    return (0);
}

int parseOutSideDigit(char *str, int *OutSideDigit)
{
    int count = 0;
    while (*str && count < 16)
    {
        if (*str >= '1' && *str <= '4')
        {
            OutSideDigit[count] = *str - '0';
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

void print(void)
{
    char Digit[8];
    Node *rowStart = StartPoint00->Down->Right;
    
    int i = 0;
    while (i < 4)
    {
        Node *current = rowStart;
        int j = 0;
        while (j < 4)
        {
            Digit[j * 2] = current->Data + '0';

            if (j < 3)  Digit[j * 2 + 1] = ' ';

            current = current->Right;
            j++;
        }
        Digit[7] = '\n';
        write(1, Digit, 8);
        
        rowStart = rowStart->Down;
        i++;
    }
}

/* Added function to free all allocated memory */
void freeGrid(void)
{
    Node *rowStart = StartPoint00;
    Node *current;
    Node *nextNode;
    int i = 0;
    while (i < 6)
    {
        current = rowStart;
        rowStart = rowStart->Down; /* Get next row before freeing current */
        int j = 0;
        while (j < 6)
        {
            nextNode = current->Right; /* Get next node before freeing current */
            free(current);
            current = nextNode;
            j++;
        }
        i++;
    }
}

int main(int argc, char **argv)
{
    int OutSideDigit[16];

    parseOutSideDigit(argv[1], OutSideDigit);

    InitialValue(OutSideDigit[0], OutSideDigit[1], OutSideDigit[2], OutSideDigit[3],
                 OutSideDigit[4], OutSideDigit[5], OutSideDigit[6], OutSideDigit[7],
                 OutSideDigit[8], OutSideDigit[9], OutSideDigit[10], OutSideDigit[11],
                 OutSideDigit[12], OutSideDigit[13], OutSideDigit[14], OutSideDigit[15]);
    
    solve();
    print();
    freeGrid(); 
}

