################################################################################
# Strings								       #
################################################################################
		.data
str_welcome:	.asciiz "Welcome to Connect Four!\n"
str_prompt:	.asciiz "Enter your move (column number): "
str_invalid:	.asciiz "Input a valid choice (0 - 6).\n"
str_full:	.asciiz "That column is full.\n"
str_one_win:	.asciiz "Player 1 wins!\n"
str_two_win:	.asciiz "Player 2 wins!\n"
		.text
		
		

################################################################################
# This is the main loop of the connect four game.  It first prints a welcome   #
# message, allocates the board, and then begins the game loop		       #
#									       #
# int main();								       #
################################################################################
# {
main:

#   puts("Welcome to Connect Four\n");
		li	$v0, 4
		la	$a0, str_welcome
		syscall

#   int* board = new_board();
		li	$v0, 9
		li	$a0, 42			# we need 7 * 6 = 42 bytes
		syscall
		move	$s0, $v0		# $s0 = the start of the board

#   while(1)
#   {
loop_game:

#     print_board(board);
		jal	print_board

#     int column = -1;
		# column input will be stored in $a0	
		
#     // Get human input until they enter a valid choice...
#     while (1)
#     {
loop_input:

#       printf("Enter your move (column number): ");
		li	$v0, 4
		la	$a0, str_prompt
		syscall

#       scanf("%d%, &column);
		li	$v0, 5
		syscall				# $v0 = column

#       if (column < 0 || column >= WIDTH)
		blt	$v0, $zero, invalid
		li	$t0, 7			# $t0 = WIDTH = 7
		bge	$v0, $t0, invalid
		j	if_place

#       {		
#         printf("Input a valid choice (0 - 6).\n");
invalid:	li	$v0, 4
		la	$a0, str_invalid
		syscall
		j	loop_input

#       }

if_place:
#       else if (place(board, column, 1) == 0)
		move	$a0, $v0
		li	$a1, 1
		jal	place
		bne	$v0, $zero, end_loop_input

#       {
#         printf("that column is full.\n");
		li	$v0, 4
		la	$a0, str_full
		syscall
		j	loop_input
#       }
#     }		
end_loop_input:

#     // Did the human input make him or her win?
#     if (check_win(board, 1) != 0)
		li	$a0, 1
		jal	check_win
		beq	$v0, $zero, skip_one_wins
#     {
#       print_board(board);
		jal	print_board

#       printf("Player 1 wins!\n");
		li	$v0, 4
		la	$a0, str_one_win
		syscall
	
#       break;
		li	$v0, 10
		syscall

#     }
skip_one_wins:

#     // Do the AI's move...
#     int ai_column = get_next_move(board);
		jal	get_next_move

#     // Did the AI's move cause it to win?
#     if (check_win(board, 1) != 0)
		li	$a0, 2
		jal	check_win
		beq	$v0, $zero, skip_two_wins
#     {
#       print_board(board);
		jal	print_board

#       printf("Player 2 wins!\n");
		li	$v0, 4
		la	$a0, str_two_win
		syscall
	
#       break;
		li	$v0, 10
		syscall

#     }
skip_two_wins:
		j	loop_game
		
#   }



################################################################################
# This simply prints the board with the column indicies at the bottom	       #
#									       #
# void print_board(int* board);						       #
################################################################################
print_board:	jr	$ra			# FIXME: Just return, do nothing



################################################################################
# Place puts a given player number at the lowest available row in the 	       #
# specified column.  It returns 1 on success and 0 on failure (row full)       #
#									       #
# $a0 = column								       #
# $a1 = player 								       #
#									       #
# $v0 = 0 or 1								       #
#									       #
# int place(int* board, int column, int player);			       #
################################################################################
place:		
		addi	$sp, $sp, -4
		sw	$ra, 0($sp)
		
		move	$a2, $a1		# $a2 = player
		move	$a1, $a0		# $a1 = column
#   for (int row = 0; row < HEIGHT; row++)
		li	$a0, -1			# $a0 = row
place_for:

#   {
		addi	$a0, $a0, 1
		li	$a3, 6			# $a3 = HEIGHT = 6
		bge	$a0, $a3, place_end_for

#     if (get(board, row, column) == 0)
		jal	get
		bne	$v0, $zero, place_for
		
#     {
#       put(board, row, column, player);
		jal	put

#       return 1;
		li	$v0, 1
		lw	$ra, 0($sp)
		addi	$sp, $sp, 4
		jr	$ra

#     }
#   }
place_end_for:

#   return 0;
		li	$v0, 0
		lw	$ra, 0($sp)
		addi	$sp, $sp, 4
		jr	$ra	
		



################################################################################
# Put places the given player number at a specified row and column.  This      #
# function always writes (even if there is already a non-zero value there) so  #
# be careful!  								       #
# 									       #
# $a0 = row								       #
# $a1 = column								       #
# $a2 = player								       #
# 									       #
# $a3 used internally							       #
# 									       #
# void put(int* board, int row, int column, int value);			       #
################################################################################
put:		
#   board[row * WIDTH + column] = value;
		li	$a3, 7			# $a3 = WIDTH = 7
		mult	$a0, $a3		
		mflo	$a3 			# $a3 = row * WIDTH
		add	$a3, $a3, $a1		# $a3 = row * WIDTH + column
		
		add	$a3, $a3, $s0
		sb	$a2, 0($a3)		# store
		jr	$ra



################################################################################
# Get returns the player number at a specified row and column.  Returns a -1   #
# if the row and column are out of bounds, a 0 if the space is empty, and the  #
# player value if the space is occupied					       #
# 									       #
# $a0 = row								       #
# $a1 = column								       #
# 									       #
# $a3 used internally							       #
# 									       #
# $v0 = -1, 0, or player						       #
################################################################################
get:
#   if (row >= 0 && row < HEIGHT &&
#       column >= 0 && column < WIDTH)
		blt	$a0, $zero, get_oob
		blt	$a1, $zero, get_oob
		li	$a3, 6			# $a2 = HEIGHT = 6
		bge	$a0, $a3, get_oob
		li	$a3, 7			# $a2 = WIDTH = 7
		bge	$a1, $a3, get_oob
		
#   {
#     return board[row * WIDTH + column];
		li	$a3, 7			# $a3 = WIDTH = 7
		mult	$a0, $a3		
		mflo	$a3 			# $a3 = row * WIDTH
		add	$a3, $a3, $a1		# $a3 = row * WIDTH + column
		
		add	$a3, $a3, $s0
		lb	$v0, 0($a3)		# load
		jr	$ra
#   }
get_oob:

#   else
#   {
#     return -1;
		li	$v0, -1
		jr	$ra
#   }



################################################################################
# This function checks the entire board and returns whether or not the given   #
# player has won the game.  It returns 1 if the player won.		       #
# 									       #
# $a0 = player								       #
#									       #
# $V0 = 0 or 1								       #
#									       #
# int check_win(int* board, int player);				       #
################################################################################
check_win:	li	$v0, 0
		jr 	$ra			# FIXME: Just return no winner



################################################################################
# int get_next_move(int* board);					       #
################################################################################
get_next_move:	li	$v0, 0
		jr 	$ra			# FIXME: Just return no winner
