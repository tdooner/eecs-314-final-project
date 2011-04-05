#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int WIDTH  = 7;
const int HEIGHT = 6;

int* new_board();

void put(int* board, int row, int column, int value);
int get(int* board, int row, int column);

int place(int* board, int column, int player);
/*
 * Returns 1 if players can gravatationally move in a given
 * spot (i.e. if a player drops a piece in a column, will it
 * drop down to the given row)
 */
int can_place(int* board, int row, int column);

int check_win(int* board);
int check_win_horz(int* board, int player);
int check_win_vert(int* board, int player);
int check_win_diag(int* board, int player);

void print_board(int* board);

int get_next_move(double* colvalues);
double* evaluate_board(int* board);

int main()
{
	puts("Welcome to Connect Four\n");

	int* board = new_board();
    
	while (1)
	{
		print_board(board);

		int column = -1;

		// Get human input until they enter a valid choice...
		while (1)
		{
			printf("Enter your move (column number): ");
			scanf("%d", &column);

			if (column < 0 || column >= WIDTH)
			{
				printf("Input a valid choice.\n");
			}
			else if (place(board, column, 1) == 0)
			{
				printf("That column is full.\n");
			}
            else
            {
                break;
            }
		}
        // Did the human input make him or her win?
        if (check_win(board) != 0)
        {
            print_board(board);
            printf("Player 1 wins!\n");
            break;
        }

		// Do the AI's move...
		int ai_column = get_next_move(evaluate_board(board));
		place(board,ai_column, 2);

		// Did the AI's move cause it to win?
        if (check_win(board) != 0)
        {
            print_board(board);
            printf("Player 2 wins!\n");
            break;
        }
	}

	free(board);

	exit(EXIT_SUCCESS);
}

int* new_board()
{
	int* board = malloc(sizeof(int) * WIDTH * HEIGHT);

	if (board == NULL)
	{
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        board[i] = 0;
    }

	return board;
}

void put(int* board, int row, int column, int value)
{
    board[row * WIDTH + column] = value;
}

int get(int* board, int row, int column)
{
    if (row >= 0 && row < HEIGHT &&
        column >= 0 && column < WIDTH)
    {
        return board[row * WIDTH + column];
    }
    else
    {
        return -1;
    }
}

int place(int* board, int column, int player)
{
    for (int row = 0; row < HEIGHT; row++)
    {
        if (get(board, row, column) == 0)
        {
            put(board, row, column, player);
            return 1;
        }
    }
    
    return 0;
}

int check_win(int* board)
{
    for (int player = 1; player <= 2; player++)
    {
        if (check_win_horz(board, player) != 0 ||
            check_win_vert(board, player) != 0 ||
            check_win_diag(board, player) != 0)
        {
            return player;
        }
    }
    
    return 0;
}

int check_win_horz(int* board, int player)
{
    for (int row = 0; row < HEIGHT; row++)
    {
        int consec = 0;
        for (int column = 0; column < WIDTH; column++)
        {
            if (get(board, row, column) == player)
            {
                consec++;
                if (consec == 4)
                {
                    return 1;
                }
            }
            else
            {
                consec = 0;
            }
        }
    }
    
    return 0;
}

int check_win_vert(int* board, int player)
{
    for (int column = 0; column < WIDTH; column++)
    {
        int consec = 0;
        for (int row = 0; row < HEIGHT; row++)
        {
            if (get(board, row, column) == player)
            {
                consec++;
                if (consec == 4)
                {
                    return 1;
                }
            }
            else
            {
                consec = 0;
            }
        }
    }
    
    return 0;
}

int check_win_diag(int* board, int player)
{
    // top left to bottom right
    for (int start_column = 4 - HEIGHT; start_column <= WIDTH - 4; start_column++)
    {
        int consec = 0;
        for (int column = start_column, row = HEIGHT - 1; column < WIDTH && row >= 0; column++, row--)
        {
            if (get(board, row, column) == player)
            {
                consec++;
                if (consec == 4)
                {
                    return 1;
                }
            }
            else
            {
                consec = 0;
            }
        }
    }
    
    // top right to bottom left
    for (int start_column = WIDTH + (HEIGHT - 4) - 1; start_column >= 3; start_column--)
    {
        int consec = 0;
        for (int column = start_column, row = HEIGHT - 1; column >= 0 && row >= 0; column--, row--)
        {
            if (get(board, row, column) == player)
            {
                consec++;
                if (consec == 4)
                {
                    return 1;
                }
            }
            else
            {
                consec = 0;
            }
        }
    }
    
    return 0;
}

void print_board(int* board)
{
    printf("Here is the board in its current state:\n");
    for (int row = HEIGHT - 1; row >= 0; row--)
    {
        for (int column = 0; column < WIDTH; column++)
        {
            printf("%d ", get(board, row, column));
        }
        printf("\n");
    }    
    
    for (int i = 0; i < WIDTH; i++)
    {
        printf("- ");
    }    
    printf("\n");
    
    for (int i = 0; i < WIDTH; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
}

/* ****************************************
 * ****************************************
 *
 * Tom's AI Stuff
 *
 * ****************************************
 *************************************** */

// Determine the best move from the 7 possible moves
int get_next_move(double* colvalues) 
{
	double max = 0;
	int maxcol = -1;
	for (int i=0; i < WIDTH; i++) {
		printf("%f   ", max);
		if (abs(colvalues[i]) > max) {
			maxcol = i;
			max = abs(colvalues[i]);
		}
	}
	printf("Picked %d with heuristic %f", maxcol, max);
	return maxcol;
}


// Given a board, this will objectively determine how good it is.
double* evaluate_board(int* board) 
{

	double* colvalues = malloc(sizeof(double) * WIDTH);

	// Find vertical availabilities
	for (int col=0; col < WIDTH; col++)
	{
		double consec = 0.0;
		int player = 0;
		int row;
		for (row=0; row < HEIGHT; row++)
		{
			int at = get(board, row, col);
			
			// Go up until we get to a blank space
			if ( at == 0) { 
				break;
			}

			// Look for the longest streak by a player
			if (at == player) {
				consec += 1.0;
			} else {
				player = at;
				consec = 1.0;
			}
		}
		// The column is only worth something if there is a blank space
		// at the top.
		if (row < (HEIGHT - 1)) {
			colvalues[col] = (player == 2) ? pow(10.0, consec) : -pow(10.0,consec);
		} else {
			colvalues[col] = 0.0;
		}
	}

	// Find horizontal availabilities
	for (int row=0; row < HEIGHT; row++) {
		// The general algorithm here is to count the consecutive pieces
		// before a blank. Then, add that value to the usefulness of
		// the blank column. Then, continue from the blank space and also add 
		// the next consecutive group to the blank space's column.
		//
		// This is accomplished by sweeping through each row twice, once from
		// left to right and once from right to left.
		int player = 0;
		int consec = 1;

		for (int col=0; col < WIDTH; col++) {
			int at = get(board, row, col);

			if (at  == 0) {
				if (!can_place(board, row, col)) {
					continue;
				}

				if (player != 0) {
					colvalues[col] += (player == 2) ? pow(10.0, consec) : -pow(10.0, consec);
				}
				player = 0;
			} else {
				if (player == at) {
					consec += 1.0;
				} else {
					consec = 1.0;
					player = at;
				}
			}
		}

		for (int col=WIDTH; col > 0; --col) {
			int at = get(board, row, col);

			if (at == 0) {
				if (!can_place(board, row,col)) {
					continue;
				}

				if (player != 0) {
					colvalues[col] += (player == 2) ? pow(10.0, consec) : -pow(10.0, consec);
				}
				player = 0;
			} else {
				if (player == at) {
					consec += 1.0;
				} else {
					consec = 1.0;
					player = at;
				}
			}
		}
	}
	for(int col=0; col < WIDTH; col++) {
		printf("%d: %f\n", col, colvalues[col]);
	}
	return colvalues;
}

int can_place(int* board, int row, int column)
{
    for (int row2 = 0; row2 < HEIGHT; row2++)
    {
        if (get(board, row2, column) == 0)
        {
			return (row == row2) ? 1 : 0;
        }
    }
    
    return 0;
}
