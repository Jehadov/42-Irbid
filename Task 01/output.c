/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Norminette.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtaha <jtaha@learner.42.tech>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 09:47:04 by jtaha             #+#    #+#             */
/*   Updated: 2025/10/25 09:47:15 by jtaha            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h> 

typedef struct s_Node
{
    int           Data;
    struct s_Node   *Right; 
    struct s_Node   *Left;
    struct s_Node   *Up;
    struct s_Node   *Down;
} Node;

Node *StartPoint00 = NULL;

void    GeneratNodeStructure(int col1up, int col2up, int col3up, int col4up, int col1down, int col2down, int col3down, int col4down, int row1left, int row2left, int row3left, int row4left, int row1right, int row2right, int row3right, int row4right)
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

            // Link Lef            if (j < 3)  Digit[j * 2 + 1] = ' ';t / Right
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
    
    //corne != null
    StartPoint00->Data = -1; 
    StartPoint00->Right->Right->Right->Right->Right->Data = -1;
    StartPoint00->Down->Down->Down->Down->Down->Data = -1; 
    StartPoint00->Down->Down->Down->Down->Down->Right->Right->Right->Right->Right->Data = -1;
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

void AddingNumber(void)
{
  
    Node *rowStart = StartPoint00->Down->Right;
    Node *top = StartPoint00->Right;
    Node *end = top->Down->Down->Down->Down->Down; 

    Node *start = StartPoint00->Down;
    Node *atend = top->Right->Right->Right->Right->Right;   

    int i = 0;
    while (i < 4)
    {
        Node *current = rowStart;
        int j = 0;
        while (j < 4)
        {
           current->Data = (j+i)%4 +1 ;



            current = current->Right;
            j++;
        }
        
        rowStart = rowStart->Down;
        i++;
    }
    i = 0;
    Node *top = StartPoint00->Right;
    Node *end = top->Down->Down->Down->Down->Down; 

    Node *start = StartPoint00->Down;
    Node *atend = top->Right->Right->Right->Right->Right; 
    while( i < 4)
    {
        // column
        if( 4 == end->Data)
        {
            Node *ptr =  end->Up;
            ptr->Data = 1; ptr=ptr->Up;
            ptr->Data = 2; ptr=ptr->Up;
            ptr->Data = 3; ptr=ptr->Up;
            ptr->Data = 4; 

        }

        if( 4 == top->Data)
        {
            Node *ptr =  top->Down;
            ptr->Data = 1; ptr=ptr->Down;
            ptr->Data = 2; ptr=ptr->Down;
            ptr->Data = 3; ptr=ptr->Down;start = start->Right;
            ptr->Data = 4; 

        }
        

        if( 1 == end->Data)
        {
            Node *ptr =  end->Up;
            ptr->Data = 4;
        }

        if( 1 == top->Data)
        {
            Node *ptr =  top->Down;
            ptr->Data = 4;

        }

               if( 3 == top->Data)
        {
            Node *ptr =  top->Down;
            ptr->Data = (ptr->Data > ptr->Down->Data)?ptr->Data--:ptr->Down->Data++; ptr=ptr->Down;
            ptr->Data = (ptr->Data < ptr->Down->Data)?ptr->Data--:ptr->Down->Data++; ptr=ptr->Down;
            ptr->Data = 3; ptr=ptr->Down;
            ptr->Data = 4; 

        }
        // i++;
        // top = top->Right; 
        // end = end->Right;
        // start = start->Down;
        // atend = atend->Down;
    }
}


