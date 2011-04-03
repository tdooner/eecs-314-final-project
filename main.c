#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int WIDTH  = 7;
const int HEIGHT = 6;

int* new_board();

void put(int* board, int row, int column, int value);
int get(int* board, int row, int column);

int place(int* board, int column, int player);

int check_win(int* board);
int check_win_horz(int* board, int player);
int check_win_vert(int* board, int player);
int check_win_diag(int* board, int player);

void print_board(int* board);

int get_next_move(int* board);
int evaluate_board(int* board);

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
		int ai_column = get_next_move(board);
		place(board,ai_column, 2);

		// Did the AI's move cause it to win?
        if (check_win(board) != 0)
        {
            print_board(board);
            printf("Player 2 wins!\n");
            break;
        }
		evaluate_board(board);

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
int get_next_move(int* board) 
{
	return 1;
}


// Given a board, this will objectively determine how good it is.
int evaluate_board(int* board) 
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
		}

		printf("%d: %f\n", col, colvalues[col]);
	}

	// Find horizontal availabilities
	for (int row=0; row < HEIGHT; row++) {
		
	}
	
	return 0;
}


