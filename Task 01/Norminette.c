/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* rush-01-norm.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <norm_bot>                                     +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/10/25 08:00:00 by <norm_bot>          #+#    #+#             */
/* Updated: 2025/10/25 08:00:00 by <norm_bot>         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h> /* For NULL and malloc */

/* Struct definition */
typedef struct s_node
{
	int				data;
	struct s_node	*right;
	struct s_node	*left;
	struct s_node	*up;
	struct s_node	*down;
}	t_node;

/* Function Prototypes */
void	print_grid(t_node *start_point);
int		parse_outside_digit(char *str, int *outside_digit_out);
int		solve(t_node *start_point);
t_node	*initial_value(int *clues);
int		calculate_view(t_node *node, int is_row, int is_reverse);
int		check_all_views(t_node *start_point);
int		is_valid(t_node *node, int num);
t_node	*find_empty_cell(t_node *start_point);
void	free_grid(t_node *start_point);
t_node	*create_row(t_node *prev_row_start);
t_node	*create_grid(void);
void	populate_top(t_node *start_point, int *clues);
void	populate_bottom(t_node *start_point, int *clues);
void	populate_left(t_node *start_point, int *clues);
void	populate_right(t_node *start_point, int *clues);
void	populate_clues(t_node *start_point, int *clues);
int		check_line(t_node *node, int num, int is_row);

/* Creates one row of 6 nodes and links it to the row above */
t_node	*create_row(t_node *prev_row_start)
{
	t_node	*node_left;
	t_node	*current_row_start;
	t_node	*node_up;
	t_node	*node;
	int		j;

	node_left = NULL;
	current_row_start = NULL;
	node_up = prev_row_start;
	j = 0;
	while (j < 6)
	{
		node = (t_node *)malloc(sizeof(t_node));
		node->data = 0;
		node->right = NULL;
		node->down = NULL;
		if (current_row_start == NULL)
			current_row_start = node;
		node->left = node_left;
		if (node_left != NULL)
			node_left->right = node;
		node->up = node_up;
		if (node_up != NULL)
		{
			node_up->down = node;
			node_up = node_up->right;
		}
		node_left = node;
		j++;
	}
	return (current_row_start);
}

/* Creates the full 6x6 grid by creating 6 rows */
t_node	*create_grid(void)
{
	t_node	*start_point;
	t_node	*prev_row;
	int		i;

	prev_row = NULL;
	start_point = NULL;
	i = 0;
	while (i < 6)
	{
		prev_row = create_row(prev_row);
		if (i == 0)
			start_point = prev_row;
		i++;
	}
	return (start_point);
}

/* Helper functions to populate clues on each side */
void	populate_top(t_node *start_point, int *clues)
{
	t_node	*current;

	current = start_point->right;
	current->data = clues[0];
	current = current->right;
	current->data = clues[1];
	current = current->right;
	current->data = clues[2];
	current = current->right;
	current->data = clues[3];
}

void	populate_bottom(t_node *start_point, int *clues)
{
	t_node	*current;

	current = start_point->down->down->down->down->down;
	current = current->right;
	current->data = clues[4];
	current = current->right;
	current->data = clues[5];
	current = current->right;
	current->data = clues[6];
	current = current->right;
	current->data = clues[7];
}

void	populate_left(t_node *start_point, int *clues)
{
	t_node	*current;

	current = start_point->down;
	current->data = clues[8];
	current = current->down;
	current->data = clues[9];
	current = current->down;
	current->data = clues[10];
	current = current->down;
	current->data = clues[11];
}

void	populate_right(t_node *start_point, int *clues)
{
	t_node	*current;

	current = start_point->right->right->right->right->right;
	current = current->down;
	current->data = clues[12];
	current = current->down;
	current->data = clues[13];
	current = current->down;
	current->data = clues[14];
	current = current->down;
	current->data = clues[15];
}

/* Populates all clues and sets corner values */
void	populate_clues(t_node *start_point, int *clues)
{
	populate_top(start_point, clues);
	populate_bottom(start_point, clues);
	populate_left(start_point, clues);
	populate_right(start_point, clues);
	/* Set corner data to -1 */
	start_point->data = -1;
	start_point->right->right->right->right->right->data = -1;
	start_point->down->down->down->down->down->data = -1;
	start_point->down->down->down->down->down->right->right->right
		->right->right->data = -1;
}

/* Creates grid and populates it, returning the start point */
t_node	*initial_value(int *clues)
{
	t_node	*start_point;

	start_point = create_grid();
	populate_clues(start_point, clues);
	return (start_point);
}

int	calculate_view(t_node *node, int is_row, int is_reverse)
{
	int	max_height;
	int	count_visible_building;
	int	i;

	max_height = 0;
	count_visible_building = 0;
	i = 0;
	while (i < 4)
	{
		if (node->data > max_height)
		{
			max_height = node->data;
			count_visible_building++;
		}
		if (is_row)
			node = is_reverse ? node->left : node->right;
		else
			node = is_reverse ? node->up : node->down;
		i++;
	}
	return (count_visible_building);
}

int	check_all_views(t_node *start_point)
{
	t_node	*start_node;
	t_node	*start_data;
	t_node	*end_node_row;
	t_node	*column_end_node;
	int		i_row;
	int		i_col;

	start_data = start_point->down->right;
	start_node = start_data;
	i_row = 0;
	while (i_row < 4)
	{
		if (calculate_view(start_node, 1, 0) != start_node->left->data)
			return (0);
		end_node_row = start_node->right->right->right;
		if (calculate_view(end_node_row, 1, 1) != end_node_row->right->data)
			return (0);
		start_node = start_node->down;
		i_row++;
	}
	start_node = start_data;
	i_col = 0;
	while (i_col < 4)
	{
		if (calculate_view(start_node, 0, 0) != start_node->up->data)
			return (0);
		column_end_node = start_node->down->down->down;
		if (calculate_view(column_end_node, 0, 1) != column_end_node->down->data)
			return (0);
		start_node = start_node->right;
		i_col++;
	}
	return (1);
}

/* Helper for is_valid: checks one row or one column for duplicates */
int	check_line(t_node *node, int num, int is_row)
{
	t_node	*current;
	int		i;

	current = node;
	if (is_row)
	{
		while (current->left != NULL && current->left->data != -1)
			current = current->left;
		current = current->right;
	}
	else
	{
		while (current->up != NULL && current->up->data != -1)
			current = current->up;
		current = current->down;
	}
	i = 0;
	while (i < 4)
	{
		if (current->data == num)
			return (0);
		current = is_row ? current->right : current->down;
		i++;
	}
	return (1);
}

int	is_valid(t_node *node, int num)
{
	if (!check_line(node, num, 1)) /* Check row */
		return (0);
	if (!check_line(node, num, 0)) /* Check column */
		return (0);
	return (1);
}

t_node	*find_empty_cell(t_node *start_point)
{
	t_node	*row_start;
	t_node	*current;
	int		i;
	int		j;

	row_start = start_point->down->right;
	i = 0;
	while (i < 4)
	{
		current = row_start;
		j = 0;
		while (j < 4)
		{
			if (current->data == 0)
				return (current);
			current = current->right;
			j++;
		}
		row_start = row_start->down;
		i++;
	}
	return (NULL);
}

int	solve(t_node *start_point)
{
	t_node	*empty_node;
	int		num;

	empty_node = find_empty_cell(start_point);
	if (empty_node == NULL)
	{
		return (check_all_views(start_point));
	}
	num = 1;
	while (num <= 4)
	{
		if (is_valid(empty_node, num))
		{
			empty_node->data = num;
			if (solve(start_point))
			{
				return (1);
			}
			empty_node->data = 0;
		}
		num++;
	}
	return (0);
}

int	parse_outside_digit(char *str, int *outside_digit_out)
{
	int	count;

	count = 0;
	while (*str && count < 16)
	{
		if (*str >= '1' && *str <= '4')
		{
			outside_digit_out[count] = *str - '0';
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

void	print_grid(t_node *start_point)
{
	char	digit[8];
	t_node	*row_start;
	t_node	*current;
	int		i;
	int		j;

	row_start = start_point->down->right;
	i = 0;
	while (i < 4)
	{
		current = row_start;
		j = 0;
		while (j < 4)
		{
			digit[j * 2] = current->data + '0';
			if (j < 3)
				digit[j * 2 + 1] = ' ';
			current = current->right;
			j++;
		}
		digit[7] = '\n';
		write(1, digit, 8);
		row_start = row_start->down;
		i++;
	}
}

void	free_grid(t_node *start_point)
{
	t_node	*row_start;
	t_node	*current;
	t_node	*next_node;
	int		i;
	int		j;

	row_start = start_point;
	i = 0;
	while (i < 6)
	{
		current = row_start;
		row_start = row_start->down; /* Get next row before freeing current */
		j = 0;
		while (j < 6)
		{
			next_node = current->right; /* Get next node before freeing current */
			free(current);
			current = next_node;
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	int		outside_digit[16];
	t_node	*start_point;

	if (argc != 2)
	{
		write(1, "Error\n", 6);
		return (1);
	}
	if (!parse_outside_digit(argv[1], outside_digit))
	{
		write(1, "Error\n", 6);
		return (1);
	}
	start_point = initial_value(outside_digit);
	if (solve(start_point))
	{
		print_grid(start_point);
	}
	else
	{
		write(1, "Error\n", 6);
	}
	free_grid(start_point);
	return (0);
}


