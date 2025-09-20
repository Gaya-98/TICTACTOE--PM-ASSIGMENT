#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables
char **board;  //double pointer is used to let the compiler know the memory size before running the program  
int SIZE;     // use this variable because the user choose the board size
FILE *logFile;  

//function prototypes
void initializeBoard();
void freeBoard();
void showGrid();
int checkForValidMove(int row, int col);
void markBoard(int row, int col, char mark);
int checkWin();
int checkDraw();
void acceptInput(int *row ,int *col);
void getPlayerMove(char mark, int *row, int *col);
char switchPlayer(char current);
int tempMoveCheck(int row,int col,char mark);
void computerMoves(int *row,int *col);// computer
void gameState(int moveNumber, char player, int row, int col);
void gameResult(int result, char winner) ;
void initializeLogFile();


// Main function
int main() {
    int row, col, moveNumber = 1 ,num,result ;
    char currentPlayer = 'X';

    do {
        printf("Enter grid size (3-10): ");
        scanf("%d", &SIZE);
        if (SIZE < 3 || SIZE > 10) {
		printf("Invalid size! Try again.\n");
	}	
    } while (SIZE < 3 || SIZE > 10);

    initializeBoard();  // create the board to user requirments
			
    logFile = fopen("tictactoe_log.txt", "w");  //open the text file in write mode
    if (!logFile) {     //file didin't create
	    printf("Error creating log file!\n");
	    freeBoard();  //free the allocated memory 
	    return 1;
    }

    initializeLogFile();  //write the changes to the text file 
    printf("Select the Game mode:\n ");
    printf("     1. Two Players (User to user)\n");
    printf("     2. User vs Computer\n");
    printf("     3. Three players\n");

    printf("Enter choice (1-3):");
    scanf("%d",&num);

    switch(num){
    case 1:
           //run the two player game 
	   printf("\n=== TIC TAC TOE %dx%d ===\n", SIZE, SIZE);
           printf("Player X starts.\n");

           //game started 
             while (1) {
                 showGrid();
                 getPlayerMove(currentPlayer, &row, &col);
                 markBoard(row, col, currentPlayer);
                 gameState(moveNumber++, currentPlayer, row, col);

                 if (checkWin()) {   //after each move check the winning condition
                      showGrid();
                      printf("Player %c wins!\n", currentPlayer);
                      gameResult(1, currentPlayer);
                      break;
                 } else if (checkDraw()) {  //after each move check the winning condition
                      showGrid();
                      printf("It's a draw!\n");
                      gameResult(0, ' ');
                      break;
                }

                currentPlayer = switchPlayer(currentPlayer);  //switch the player 
            }
 
            break;
        
    case 2:
            //run the user vs computer game
	
          printf("You are the player X and computer is the player O\n");
          printf("\n=== TIC TAC TOE %dx%d ===\n", SIZE, SIZE);
          printf("Player X starts.\n");
	  showGrid();
          srand(time(NULL)); //gave a seeding value as a starting value for the random nuber generator 
          while(1){

               
	       if (currentPlayer=='X'){
		     getPlayerMove('X',&row,&col); //since the user is x
	       }else{
		      computerMoves(&row,&col);  //get the input from computer
	       }

               markBoard(row, col, currentPlayer);  //mark movements on the beard
               gameState(moveNumber++, currentPlayer, row, col); //add game status to the file
               showGrid(); //after each move show tj=he game status
               
	      if (checkWin()) {   //after each move check the winning condition
                      showGrid();
                      printf("Player %c wins!\n", currentPlayer);
                      gameResult(1, currentPlayer);
                      break;
                 } else if (checkDraw()) {  //after each move check the winning condition
                      showGrid();
                      printf("It's a draw!\n");
                      gameResult(0, ' ');
                      break;
                }
	        currentPlayer=switchPlayer(currentPlayer);
           }
      

           break; 

    case 3:
           //run the multiplayer game 
           break;
    default:
           printf("The number is not a valid mode\n");
  }
     printf("Game log saved to 'tictactoe_log.txt'\n");  //after switching before the next player start the game wirte the previous moves to thetxt file 
     fclose(logFile); //file is closed 
     freeBoard(); //memory allocated  to the board is freed 

 
return 0;
}


//  allocate memory and initialize  the board

void initializeBoard() {
    board = (char**)malloc(SIZE * sizeof(char*));  //create the memory for the array 
    for (int i = 0; i < SIZE; i++) {
        board[i] = (char*)malloc(SIZE * sizeof(char)); //create each row by allocating memory 
        for (int j = 0; j < SIZE; j++) { //name each meory block with a index 
            board[i][j] = ' ';
        }
    }
}

// Function to free memory
void freeBoard() {
    for (int i = 0; i < SIZE; i++) {  
        free(board[i]);  //release the memory allocated to each row
    }
    free(board);//release the memory allocated for the board before leak happens
}

// Function to display the board
void showGrid() {
    printf("\n    "); //print newline with 4 spaces 
    for (int i = 0; i < SIZE; i++){
	    printf("%2d  ", i);   //number each columns and used  %2d to align the numbers when given two digit values
    } 
    printf("\n  ");//print newline with 2 spaces
    for (int i = 0; i < SIZE * 4 + 1; i++){  //count the number of dashes needed to print the top boarder
	    printf("-");
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d |", i);  //print the row number
        for (int j = 0; j < SIZE; j++) {  
		printf(" %c |", board[i][j]);  //after printing the row number this will print the row data 
	}
        printf("\n  ");
        for (int k = 0; k < SIZE * 4 + 1; k++){
	       	printf("-");//sfter printing the row to close teh row the bottom boarder will be printed
	} 
        printf("\n");
    }
    printf("\n");
}

// Check if move is valid
int checkForValidMove(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) { //not in  the valid range 
	    return 0;  //invalid move
    }
    if (board[row][col] != ' ') {  //if the position is already taken 
	    return 0;//invalid move
    }
    return 1;  //valid move
}

// Mark the board
void markBoard(int row, int col, char mark) {
    board[row][col] = mark;  //access the cell chosssen by the player and write the value( 'x'/ 'o')
}

// Check for win condition
int checkWin() {
    // Check rows
    for (int i = 0; i < SIZE; i++) {
        char first = board[i][0];  //get the cell
        if (first != ' ') {  // if the cell is not empty
            int win = 1; //have a chance to win 
            for (int j = 1; j < SIZE; j++) {  
                if (board[i][j] != first) {    //if the  cell is not empty check whether it match another cell
		       	win = 0; //since the values doesn't match the row values  it is not a win 
		       	break; //stop checking that row 
	        }
	    } 
            if (win) 
		    return 1; //if matches  winner is found
        }
    }

    // Check columns
    for (int j = 0; j < SIZE; j++) {
        char first = board[0][j];  //get the cell
        if (first != ' ') {  //check the cell is not empty
            int win = 1;//have a chance to win 
            for (int i = 1; i < SIZE; i++) {
                if (board[i][j] != first) {  //check for not matching values
		       	win = 0; //no winning
		       	break; //stop checking the column
	       	}
            }
            if (win) 
		    return 1; //win
        }
    }

    // Check main diagonal
    char firstDiag = board[0][0];
    if (firstDiag != ' ') { //the first cel is not empty 
        int win = 1; //have a chance to win 
        for (int i = 1; i < SIZE; i++) { 
            if (board[i][i] != firstDiag) { //check the next mainn diagonal value is identical 
		    win = 0; //not identical
		    break; //stop cheking the main diagonal
	    }
        }
        if (win) 
		return 1; //matching values so win
    }

    // Check anti-diagonal
    char firstAnti = board[0][SIZE-1]; //get the first  anti-diagonal cell 
    if (firstAnti != ' ') {//check whether it is not empty 
        int win = 1; //have a chance to win
        for (int i = 1; i < SIZE; i++) {
            if (board[i][SIZE-1-i] != firstAnti) { //check for identical values 
		    win = 0;  //no identical values 
		    break; //stop checking the anti-diagonal
	    }
        }
        if (win) 
		return 1; //win 
    }

    return 0; // No win
}

// Check if board is full (draw)
int checkDraw() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {  
            if (board[i][j] == ' ') 
		    return 0; // Empty cell exist not draw
        }
    }
    return 1; // Draw
}

//accept user input 
void acceptInput(int *row ,int *col){
	 printf("Enter row and column (0  %d): ", SIZE - 1);
         scanf("%d %d", row, col);
}

// Get player move
void getPlayerMove(char mark, int *row, int *col) {
    do {
        printf("Player %c, 's turn.\n ", mark); 
        acceptInput(row, col);  //ask user for input 
        if (!checkForValidMove(*row, *col)) {   //not a valid move and write directly to the vaiable as we have used pointes
            printf("Invalid move! Try again.\n");
        }
    } while (!checkForValidMove(*row, *col));
}


// making computer understand the user's winning posibility before making the next move otherwise it take long time to finish the game when it comes to larger boards

int tempMoveCheck(int row,int col,char mark){
	board[row][col]=mark;     //Temperarily place the mark
	int victory=checkWin();     //check all the winning posibilities of the user 
	board[row][col]=' '; 	//remove the predicted move  
	return victory;             // if victory is 1 it has a chance to win if it is 0 the move doesn't lead to win 
}

//this contain two behaviors of the computer 

// 1.Try to block if the user has a  chance to win 
void computerMoves(int*row,int*col){
  
    for(int r=0;r<SIZE;r++){
	  for (int c=0;c<SIZE;c++){
		  if(checkForValidMove(r,c)){
			  if(tempMoveCheck(r,c,'X')){
				  *row=r;
				  *col=c;
				  printf("Computer moved to (%d,%d) to block the user .\n",*row,*col);
				  return;
			  }
		  
		  }
	  
	  }
    }
 
 
//2.If there is no winning posibility of the user ,computer will get random numbers 

   do{
         *row=rand()%SIZE; // range will be from 0 to (SIZE-1)
         *col=rand()%SIZE; 
   }while(!checkForValidMove(*row,*col)); //run until a valid position is given

   printf("Computer moved to : %d %d \n",*row,*col);
}


// Switch player
char switchPlayer(char current) {
    if (current == 'X') {
	   return 'O' ;
    }else{
	  return  'X';
    }
}

// log the current state of the board 
void gameState(int moveNumber, char player, int row, int col) { //mover number ,current player ,row and column
    fprintf(logFile, "Move %d: Player %c placed at (%d, %d)\n", moveNumber, player, row, col);
    for (int i = 0; i < SIZE; i++) { //loop through each row 
        for (int j = 0; j < SIZE; j++) {  //loop through each cell in that row
            fprintf(logFile, "%c ", board[i][j]);  //print each cell to the text file seperated by  space
        }
        fprintf(logFile, "\n"); //move to the next line after finishing one row
    }
    fprintf(logFile, "\n"); 
    fflush(logFile);  //ensure all data to be written to the file immediately  otherwise if the program crashes all data will be lost 
}


//log the final result of the game 
void gameResult(int result, char winner) {
    if (result == 1) {  //checkWin() return 1
	    fprintf(logFile, "GAME OVER: Player %c wins!\n\n", winner);
    } else{  
	    fprintf(logFile, "GAME OVER: It's a draw!\n\n");
    }
    fflush(logFile);  //immediately write the result
}

//header to the log file 

void initializeLogFile() {
    time_t now = time(NULL);  //get the current system time 
    fprintf(logFile, "=== TIC-TAC-TOE GAME LOG ===\n");  //title
    fprintf(logFile, "Date: %s", ctime(&now));    //convert the current time ti readable format 
    fprintf(logFile, "Grid Size: %dx%d\n\n", SIZE, SIZE);  //board size user chose 
    fflush(logFile); 
}


