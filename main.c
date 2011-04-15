#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int WIDTH  = 7;
const int HEIGHT = 6;

int* new_board();

/*
 * Put places the given player number at a specified row and column.  
 * This function always succeeds (even if there is already a value
 * there, so be careful!
 * 
 * $a0 = row
 * $a1 = column
 * $a2 = player
 */
void put(int* board, int row, int column, int value);

/*
 * Get returns the player number at a specified row and column.  Returns
 * a -1 if the row and column are out of bounds, a 0 if the space is 
 * empty, and a 1 or 2 if the space is occupied
 * 
 * $a0 = row
 * $a1 = column
 * 
 * $v0 = -1, 0, or player
 */
int get(int* board, int row, int column);

/*
 * Place puts a given player number at the lowest available row in the
 * specified column.  It returns 1 on sucess and 0 on failure (row full)
 * 
 * $a0 = column
 * $a1 = player
 * 
 * $v0 = 0 or 1
 */
int place(int* board, int column, int player);

/*
 * Returns the number of consecutive pieces before a blank in the
 * position a b c _. This is useful for the diagonal checking.
 *
 * Returns an integer between 1 (e.g. 1 2 1 _) and 3 (e.g. 1 1 1 _)
 *
 * Also, the player with the consecutive streak will be the player
 * which is in c.
 */
int num_consecutive(int a, int b, int c);

/*
 * This function checks the entire board and returns whether or not
 * the given player won the game.  It returns 1 if the player won.
 * 
 * $a0 = player
 * 
 * $v0 = 0 or 1
 */
int check_win(int* board, int player);

/*
 * cThe check_win_* functions take a player number and check to see
 * if that player won in the given direction.  They return 0 if the
 * player did not win and 1 if they did
 * 
 * $a0 = player num
 * $v0 = 0 or 1
 */
int check_win_horz(int* board, int player);
int check_win_vert(int* board, int player);
int check_win_diag(int* board, int player);

/* 
 * This simply prints the board with the column indicies at the bottom
 */
void print_board(int* board);

int get_next_move(int* colvalues);
int* evaluate_board(int* board);

/*
 * This is the main loop of the connect four game.  It first prints a 
 * welcome message, allocates the board, and then begins the game loop
 */
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
				printf("Input a valid choice (0 - 6).\n");
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
        if (check_win(board, 1) != 0)
        {
            print_board(board);
            printf("Player 1 wins!\n");
            break;
        }

		// Do the AI's move...
		int ai_column = get_next_move(evaluate_board(board));
		place(board,ai_column, 2);

		// Did the AI's move cause it to win?
        if (check_win(board, 2) != 0)
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

int check_win(int* board, int player)
{
	if (check_win_horz(board, player) != 0 ||
		check_win_vert(board, player) != 0 ||
		check_win_diag(board, player) != 0)
	{
		return 1;
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
			if (get(board, row, column) == 0) {
				printf("  ");
				continue;
			}
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
int get_next_move(int* colvalues) 
{
	int max = 0;
	int maxcol = -1;
	int sum = 0;
	for (int i=0; i < WIDTH; i++) {
		sum += abs(colvalues[i]);
		if (abs(colvalues[i]) > max) {
			maxcol = i;
			max = abs(colvalues[i]);
		}
	}
	printf("Picked %d with confidence %.0f%%\n", maxcol, floor(100*max/(double)sum));
	return maxcol;
}


// Given a board, this will objectively determine how good it is.
int* evaluate_board(int* board) 
{
	int* colvalues = malloc(sizeof(int) * WIDTH);

	// For each column, find the first available opening, and then find how
	// many pieces are consecutive vertically, horizontally, and diagonally.
	for (int col=0; col < WIDTH; col++)
	{
		///////////////////////////////////////////////////////////////////////
		// Find an open block in each column, then find vertical availabilities
		// in that column, then find the horizontal availablities and diagonal
		// ones too.
		///////////////////////////////////////////////////////////////////////	

		// Find the first available opening in the column.
		int vertical_consec = 0;
		int player = 0;
		int row;
		for (row=0; row < HEIGHT; row++)
		{
			int at = get(board, row, col);
			
			// Go up until we get to a blank space
			if ( at == 0) {  
				break;
			}

			// While going up the column, keep track of consecutive pieces
			if (at == player) { 
				vertical_consec += 1;
			} else {
				player = at;
				vertical_consec = 1;
			}
		}
		// The column is only worth something if there is a blank space
		// at the top.
		if (row < HEIGHT) {
			// Initialize this column to the goodness vertically.
			colvalues[col] = (player == 2) ? (int)pow(10.0, vertical_consec) : -(int)pow(10.0,vertical_consec);
		} else {
			colvalues[col] = 0;
			continue; 	// Try the next column....
		}

		///////////////////////////////////////////////////////////////////////
		// Horizontal Availablities
		///////////////////////////////////////////////////////////////////////	
		// The general algorithm here is to count the consecutive pieces
		// before a blank. Then, add that value to the usefulness of
		// the blank column. Then, continue from the blank space and also add 
		// the next consecutive group to the blank space's column.
		//
		// This is accomplished by sweeping through each row twice, once from
		// left to right and once from right to left.

		int horiz_before_player = 0;
		int horiz_after_player = 0;
		
		int horiz_before_consec = 1;
		int horiz_after_consec = 1;
		
		// Before the found gap.
		for (int col_horiz=0; col_horiz < col; col_horiz++) {
			int at = get(board, row, col_horiz);

			if (horiz_before_player == at) {
				horiz_before_consec += 1;
			} else {
				horiz_before_consec = 1;
				horiz_before_player = at;
			}	
		}
		if (horiz_before_player == 0) {
			horiz_before_consec = 0;
		}
		
		// After the given gap. (Start at the right edge of the game board and
		// work backwards until the gap)
		for (int col_horiz=WIDTH; col_horiz > col; --col_horiz) {
			int at = get(board, row, col_horiz);

			if (horiz_after_player == at) {
				horiz_after_consec += 1;
			} else {
				horiz_after_consec = 1;
				horiz_after_player = at;
			}
		}
		if (horiz_after_player == 0) {
			horiz_after_consec = 0;
		}
		// Now calculate the horizontal adjancies... keeping in mind that
		// both before _and_ after must be considered, as the board position
		// could be like:  2 1 2 _ 2 2 , which should identify the middle
		// position as more valuable.
		if (horiz_before_player == horiz_after_player) {
			int total_consec = horiz_before_consec + horiz_after_consec;	
			colvalues[col] += (horiz_before_player == 2) ? (int)pow(10.0, total_consec) : -(int)pow(10.0,total_consec);
		} else {
			colvalues[col] += (horiz_before_player == 2) ? (int)pow(10.0, horiz_before_consec) : -(int)pow(10.0,horiz_before_consec);
			colvalues[col] += (horiz_after_player == 2) ? (int)pow(10.0, horiz_after_consec) : -(int)pow(10.0,horiz_after_consec);
		}
		/////////////////////////////////////////////////////////////
		// Find Diagonal Availabilities
		/////////////////////////////////////////////////////////////
		// The general algorithm here is to look in each column, find a
		// blank, and then count the number of adjacent numbers in those 
		// diagonals.
		//
		// Sorry, whoever is converting this to MIPS (oh dear I hope it's
		// not me.)

		int a,b,c,up_player,up_consec,below_consec;
		// Below left diagonal
		a = get(board, row - 3, col - 3);
		b = get(board, row - 2, col - 2);
		c = get(board, row - 1, col - 1);
		up_player = c;
		up_consec = num_consecutive(a, b, c);
		if (up_player == -1 || up_player == 0) {
			up_consec = 0;
		}

		// Above right diagonal
		a = get(board, row + 3, col + 3);
		b = get(board, row + 2, col + 2);
		c = get(board, row + 1, col + 1);
		below_consec = num_consecutive(a, b, c);
		if (c == -1 || c == 0) {
			below_consec = 0;
		}

		if (up_player == c) { // If the streak continues across the gap
			int total_consec = up_consec + below_consec;
			colvalues[col] += (c == 2) ? (int)pow(10.0, total_consec) : -(int)pow(10.0,total_consec);
		} else {
			colvalues[col] += (up_player == 2) ? (int)pow(10.0, up_consec) : -(int)pow(10.0, up_consec);
			colvalues[col] += (c == 2) ? (int)pow(10.0, below_consec) : -(int)pow(10.0, below_consec);
		}

		// Below right diagonal
		a = get(board, row - 3, col + 3);
		b = get(board, row - 2, col + 2);
		c = get(board, row - 1, col + 1);
		up_player = c;
		up_consec = num_consecutive(a, b, c);
		if (up_player == -1 || up_player == 0) {
			up_consec = 0;
		}

		// Above left diagonal
		a = get(board, row + 3, col - 3);
		b = get(board, row + 2, col - 2);
		c = get(board, row + 1, col - 1);
		below_consec = num_consecutive(a, b, c);
		if (c == -1 || c == 0) {
			below_consec = 0;
		}

		if (up_player == c) { // If the streak continues across the gap
			int total_consec = up_consec + below_consec;
			colvalues[col] += (c == 2) ? (int)pow(10.0, total_consec) : -(int)pow(10.0,total_consec);
		} else {
			colvalues[col] += (up_player == 2) ? (int)pow(10.0, up_consec) : -(int)pow(10.0, up_consec);
			colvalues[col] += (c == 2) ? (int)pow(10.0, below_consec) : -(int)pow(11.0, below_consec);
		}
	}

		// Below left to above right...
	return colvalues;
}

int num_consecutive(int a, int b, int c) {
	// Uses some bitwise ORs because that will be easier in
	// MIPS.
	int a_and_b = 0;
	int b_and_c = 1;  // At the least, this should return 1
	if (b == c) {
		// "c c _" indicates that there are at least two consecutive,
		// but perhaps more.
		b_and_c = 2;  

		if (a == b) {
			a_and_b = 1;
		}
	}
	return (a_and_b | b_and_c);
}
