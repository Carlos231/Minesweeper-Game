/******************************************************
 ** PROGRAM: minesweeper.cpp
 ** AUTHOR: Carlos Lopez Molina
 ** DATE: 3/13/16
 ** DESCRIPTION: Minesweeper game
 ** INPUT: number of Rows, columns, and mines
 ** Output: board with mines to avoid
 ******************************************************/
#include <iostream>
#include <cstdlib> //for NULL
#include <ctime>

using namespace std;

void command_values(int argc, char *argv[], int &rows, int &col, int &mines);
void prompt (int &row, int &col, int &mines);
char **dyn_board (int rows, int col);
void print_updated_board (char **board, int rows, int col);
void set_mines (char **board, int rows, int col, int mines);
void check_inputs (char **board, int row, int col, int &uRow, int &uCol);
bool filled_board (char **board, int rows, int col, int uRow, int uCol);
bool End (bool loose, bool &playingAgain, bool &whoiswinner);
bool user_choice (char **board, int &uRow, int &uCol, bool &loose, int row, int col, bool &whoiswinner);
void output_mines_touching (char **board, int uRow, int uCol, int row, int col);
char character (int num);

/*********************************************************************
 ** FUNCTION: main
 ** DESCRIPTION: Recieves the command line values to set number of rows, columns and mines.
 ** PARAMETERS: argc, argv
 ** PRE-CONDITIONS: none
 ** POST-CONDITIONS: none
 ** RETURNS: nothing
 *********************************************************************/
int main (int argc, char *argv[]){ //takes two arguments
	int rows, col, mines, uRow, uCol;
	bool gameover = false, end = false, loose = false, playingAgain = false, whoiswinner;

	system("clear");

	//get num of rows, columns and mines from command line
	command_values (argc, argv, rows, col, mines);

	//check to play again
	while (end != true){
		// only if starting a new game
		if (playingAgain == true){
			prompt (rows, col, mines);
		}

		//set up dynamic array funcition
		char **board;
		//set up board
		board = dyn_board (rows, col);

		//set mines
		set_mines (board, rows, col, mines);

		gameover = false;
		while (gameover != true){
			/**************
			  Actual game
			 **************/
			//output updated board
			print_updated_board (board, rows, col);

			//flag or open
			gameover = user_choice (board, uRow, uCol, gameover, rows, col, whoiswinner);
		}
		//out of loop so check if won or lost and prompt user to play again

		//reallocate memory
		for (int i=0; i < rows; i++){
			delete []board[i];
		}
		delete []board;

		//check if won or not
		end = End (loose, playingAgain, whoiswinner);
	}

	return 0;
}

/*********************************************************************
 ** FUNCTION: command_values
 ** DESCRIPTION: usage check for command arguments and sets number rows, col, and mines.
 ** PARAMETERS: argc, argv, rows, col, mines
 ** PRE-CONDITIONS: rows, col and mines not set
 ** POST-CONDITIONS: rows, col and mines not set 
 ** RETURNS: nothing
 *********************************************************************/
void command_values(int argc, char *argv[], int &rows, int &col, int &mines){

	//make sure they input correct format
	for (int i=1; i<argc; i+=2){ //checking array spots 1, 3 and 5	
		if (!(argv[i][0] == '-' && argv[i][1]=='r')){
			cout << "usage: exe -r number" << endl;
			break;
		}

		else if (!(argv[i][0] == '-' && argv[i][1]=='c')){
			cout << "usage: exe -c number" << endl;
			break;
		}

		else if (!(argv[i][0] == '-' && argv[i][1]=='m')){
			cout << "usage: exe -m number" << endl;
			break;
		}
	}	
	//Receiving inputs from command line
	for (int i=1; i<argc; i+=2){ //checking array spots 1, 3 and 5	
		if (argv[i][0] == '-' && argv[i][1]=='r'){
			//cout << atoi(argv[i+1]) << endl;
			rows = atoi(argv[i+1]);
		}

		else if (argv[i][0] == '-' && argv[i][1]=='c'){
			//cout << atoi(argv[i+1]) << endl;
			col = atoi(argv[i+1]);
		}

		else if (argv[i][0] == '-' && argv[i][1]=='m'){
			//cout << atoi(argv[i+1]) << endl;
			mines = atoi(argv[i+1]);
		}
	}
}

/*********************************************************************
 ** FUNCTION: prompt
 ** DESCRIPTION: asks for number of rows, cols, and mines
 ** PARAMETERS: int row, col, mines
 ** PRE-CONDITIONS: ints declared in main
 ** POST-CONDITIONS: ints declared in main updated with new numbers 
 ** RETURNS: nothing
 *********************************************************************/
void prompt (int &row, int &col, int &mines){
	system("clear");
	cout << "Welcome back to another game of Minesweeper!" << endl;
	cout << "Enter number of rows: ";
	cin >> row;

	cout << "Enter number of columns: ";
	cin >> col;

	cout << "Enter number of mines: ";
	cin >> mines;
}

/*********************************************************************
 ** FUNCTION: dyn_board
 ** DESCRIPTION: Creates a dynamic board with user number of rows and columns
 ** PARAMETERS: rows, col
 ** PRE-CONDITIONS: blank board
 ** POST-CONDITIONS: new board with number of rows, columns and x as a place holder
 ** RETURNS: char 2-d array
 *********************************************************************/
char **dyn_board (int rows, int col){
	char **temp;
	temp = new char *[rows];

	for (int i=0; i < rows; i++){
		temp[i] = new char [col];
	}

	for (int i=0; i<rows; i++){
		for (int j=0; j<col; j++){
			temp[i][j] = 'x';
		}
	}

	return temp;
}

/*********************************************************************
 ** FUNCTION: print_updated_board
 ** DESCRIPTION: prints updated board with flags and open slots
 ** PARAMETERS: board, rows, col
 ** PRE-CONDITIONS: board with x place holders
 ** POST-CONDITIONS: board changes x to flag or open symbols 
 ** RETURNS: nothing
 *********************************************************************/
void print_updated_board (char **board, int rows, int col) {
	int num=0;

	//number of columns
	for (int i=0; i<col; i++){
		cout << i << ' ';
	}cout << endl;

	//board
	for (int i=0; i<rows; i++){
		for (int j=0; j<col; j++){
			num++;
			cout << board[i][j] << " ";
			if (num == col){
				cout << endl;
				num=0;
			}
		}
	}
	cout << endl;
}

/*********************************************************************
 ** FUNCTION: set_mines
 ** DESCRIPTION: Get user number of mines and randomly sets them on board
 ** PARAMETERS: board, rows, col, mines
 ** PRE-CONDITIONS: board with no mines
 ** POST-CONDITIONS: board with number of mines set randomly on board 
 ** RETURNS: nothing
 *********************************************************************/
void set_mines (char **board, int rows, int col, int mines) {
	srand (time(NULL)); //set random seed

	//declare to use outside scope
	int minerow, minecol;

	//use random number to place each mine in different spot
	for (int i=0; i < mines; i++){ //for number amount of mines wanted
		//preset to 0
		minerow = 0;
		minecol = 0;	

		//get random number of row
		minerow = rand() % rows;
		//get random number of col
		minecol = rand() % col;	

		board[minerow][minecol] = '*'; //rand row and col = mine
		cout << "*Mine row: " << minerow << " " << "col: " << minecol << endl;
	}
}

/*********************************************************************
 ** FUNCTION: check_inputs
 ** DESCRIPTION: Make sure user inputs are in range and if already opened spot
 ** PARAMETERS: board, row, col, uRow, uCol
 ** PRE-CONDITIONS: none
 ** POST-CONDITIONS: sets uRow and uCol to user numbers to change board place
 ** RETURNS: nothing
 *********************************************************************/
void check_inputs (char **board, int row, int col, int &uRow, int &uCol) {
	bool goodvalue = false;
	int Row, Col;

	while (goodvalue != true){
		cout << "Enter the row followed by the column: ";
		cin >> Row;
		cin >> Col;

		if (((Row < 0) || (Row > row)) || ((Col < 0) || Col > col)){
			cout << "Invalid inputs! Enter number 0 through " << row << " for rows and 0 through " << col << " for columns." << endl;
			Row = 0; // to avoid sementation fault
			Col = 0;
		}
		if (board [Row][Col] == '0'){
			cout << "You already opened this spot!" << endl;
		}
		if (board [Row][Col] != '0'){		
			goodvalue = true;
		}
	}
	uRow = Row;
	uCol = Col;
}

/*********************************************************************
 ** FUNCTION: fill_board
 ** DESCRIPTION: Checks if board has been filled up in order to win
 ** PARAMETERS: board, rows, col
 ** PRE-CONDITIONS: board filled equal to false so havent won yet
 ** POST-CONDITIONS: board filled eual to true if board is filled
 ** RETURNS: bool boardFilled
 *********************************************************************/
bool filled_board (char **board, int rows, int col, int uRow, int uCol){
	bool boardFilled = true;

	for (int i=0; i<rows; i++){
		for (int j=0; j<col; j++){
			if (board[i][j] == 'x'){
				boardFilled = false;
			}
		}
	}
	return boardFilled;
}

/*********************************************************************
 ** FUNCTION: End
 ** DESCRIPTION: Checks if won or lost game, then asks if would like to play again
 ** PARAMETERS: loose, playingAgain
 ** PRE-CONDITIONS: none
 ** POST-CONDITIONS: changes bools
 ** RETURNS: bool again
 *********************************************************************/
bool End (bool loose, bool &playingAgain, bool &whoiswinner) {
	int choice;
	bool again;
	if (whoiswinner == false){
		cout << "You hit a mine, you loose! :( " << endl;
	}
	else if (whoiswinner == true){
		cout << "You won, congratulations! " << endl;
	}

	cout << "Would you like to play again? (1-yes, 0-no) ";
	cin >> choice;
	if (choice == 1){
		again = false;
		playingAgain = true;
	}
	else if (choice == 0){
		again = true;
		playingAgain = false;
	}
	return again;
}

/*********************************************************************
 ** FUNCTION: user_choice
 ** DESCRIPTION: Gets users choice to whether open or flag spot on board
 ** PARAMETERS: board, uRow, uCol, loose, row, col
 ** PRE-CONDITIONS: declared board with mines
 ** POST-CONDITIONS: changes board with flagged/open spots 
 ** RETURNS: bool loose
 *********************************************************************/
bool user_choice (char **board, int &uRow, int &uCol, bool &loose, int row, int col, bool &whoiswinner) {
	int choice;
	bool temp;

	cout << "Flag(1) or Open(2): ";
	cin >> choice;

	//flag
	if (choice == 1){
		check_inputs (board, row, col, uRow, uCol);
		board[uRow][uCol] = '!';
	}
	//open
	else if (choice == 2){
			//check if already opened
		check_inputs (board, row, col, uRow, uCol);
		//check if game is over

		//if user selects the mine then lose
		if (board[uRow][uCol] == '*') {
			whoiswinner = false; //person lost by hitting mine
			loose = true; //exit loop
		}else output_mines_touching (board, uRow, uCol, row, col);
		
		//lose is false by default
		//check if filled whole board
		temp = filled_board (board, row, col, uRow, uCol);
		if (temp == true){
			loose = true;
			whoiswinner = true; //person won by filling board
		}		
	}
	return loose;
}

/*********************************************************************
 ** FUNCTION: output_mines_touching
 ** DESCRIPTION: Output number of times box touches a mine
 ** PARAMETERS: char **board, int uRow, int uCol
 ** PRE-CONDITIONS: board has been set up and have input for which box in board would like to open
 ** POST-CONDITIONS:  opened board with number of boxes with mines is touching.
 ** RETURNS: nothing
 *********************************************************************/
void output_mines_touching (char **board, int uRow, int uCol, int row, int col){
	//number of mines touching
	int num=0;
	
	
	//topleft
	if ((uRow-1 >= 0) && (uCol-1 >= 0) && (board[uRow-1][uCol-1] == '*')){
		num++;
	}
	
	//top
	if ((uRow-1 >= 0) && (board[uRow-1][uCol] == '*')) {
		num++;
	}
	
	//topright
	if ((uRow-1 >= 0) && (uCol+1 <= row) && (board[uRow-1][uCol+1] == '*')) {
		num++;
	}
	
	//left
	if ((uCol-1 >= 0) && (board[uRow][uCol-1] == '*')) {
		num++;
	}
	
	//Users spot
	
	//right
	if ((uCol+1 <= col) && (board[uRow][uCol+1] == '*')) {
		num++;
	}
	
	//bottomleft
	if ((uRow+1 < row) && (uCol-1 >= 0) && (board[uRow+1][uCol-1] == '*')) {
		num++;
	}
	
	//bottom
	if ((uRow+1 < row) && (board[uRow+1][uCol] == '*')) {
		num++;
	}
	
	//bottomright
	if ((uRow+1 < row) && (uCol+1 < col) && (board[uRow+1][uCol+1] == '*')) {
		num++;
	}
	
	char number;
	number = character (num);
	
	//Convert to char to display on board
	
	board[uRow][uCol] = number;
}

/*********************************************************************
 ** FUNCTION: character
 ** DESCRIPTION: converts int number to char number to replace spot in board with
 ** PARAMETERS: int num
 ** PRE-CONDITIONS: plug in a number
 ** POST-CONDITIONS: changes number into a character
 ** RETURNS: char value 
 *********************************************************************/
char character (int num){
	char value;
	
	//Convert to char to display on board
	if (num == 0){
		value = '0';
	}
	
	else if (num == 1){
		value = '1';
	}
	
	else if (num == 2){
		value = '2';
	}
	
	else if (num == 3){
		value = '3';
	}
	
	else if (num == 4){
		value = '4';
	}
	
	else if (num == 5){
		value = '5';
	}
	
	else if (num == 6){
		value = '6';
	}
	
	else if (num == 7){
		value = '7';
	}
	
	else if (num == 8){
		value = '8';
	}
	
	return value;
}
 
