#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variables
char **grid;  //double pointer is used to let the compiler know the memory size before running the program  
int SIZE;     // use this variable because the user choose the board size
FILE *fptr;  // write all movemets of all players and this is accessible by other functions 

//function prototypes

void createTheGrid();
void freeCellsAfterGame();
void displayTheGrid();
int checkForValidMove(int row, int col);
void markOnTheGrid(int row, int col, char value);
int checkForTheWinner();
int checkForDraw();
void acceptInput(int *row ,int *col);
void getPlayerMove(char value, int *row, int *col);
char swapPlayersAfterEachMove(char current,int num);
int tempMoveCheck(int row,int col,char value);
int smartWinMove(int *row,int*col,char value,int num);
int blockOpponentsMoves(int *row,int*col,char value,int num);
void getComputerMoves(int *row,int *col,char value,int num);
void gameState(int moveNumber, char player, int row, int col);
void gameResult(int result, char winner) ;
void createTheFile();


// Main function
int main() {
    int row, col, moveNumber = 1 ,num,result ;
    char currentPlayer = 'X';
    
    printf("   Welcome to TIC TAC TOE game !!!!!\n ");

    do {
        printf("Please enter the  grid size (3-10): ");
        scanf("%d", &SIZE);
        if (SIZE < 3 || SIZE > 10) {
		printf("Invalid size! Try again.\n");
	}	
    } while (SIZE < 3 || SIZE > 10);

    createTheGrid();  // create the board to user requirments
			
    fptr = fopen("gamelog.txt", "w");  //open the text file in write mode
    //if we use the write mode and if the file is not created yet it will create and write the text we want to create 

    if (!fptr) {   
	    printf("Error creating  file!\n");
	    freeCellsAfterGame();  //free the allocated memory for rows 
	    return 1; 
    }

    createTheFile();  //write the changes to the text file 

    printf("Select the Game mode:\n");
    printf("     1. Two Players (User to user)\n");
    printf("     2. User vs Computer\n");
    printf("     3. Three players\n");

    printf("Enter choice (1-3):");
    scanf("%d",&num);

    switch(num){   //used a switch function to organize codde for  each mode 
    
	  case 1:
                  //run the two player game 
	          printf("\n    TIC TAC TOE %dx%d    \n", SIZE, SIZE);
                  printf("Player X starts.\n");

                  //game started 
                   while (1) {  
                          displayTheGrid();
                          getPlayerMove(currentPlayer, &row, &col);
                          markOnTheGrid(row, col, currentPlayer);
                          gameState(moveNumber++, currentPlayer, row, col);

                         if (checkForTheWinner()) {   //after each move check the winning condition
                                displayTheGrid();
                                printf("Player %c wins!\n", currentPlayer);
                                gameResult(1, currentPlayer);
                                break;
                         } else if (checkForDraw()) {  //after each move check the winning condition
                                displayTheGrid();
                                printf("It's a draw!\n");
                                gameResult(0, ' ');
                                break;
                        }

                        currentPlayer = swapPlayersAfterEachMove(currentPlayer,num);  //switch the player 
                   }
 
                    break;
        
        case 2:
            //run the user vs computer game
	
          printf("You are the player X and computer is the player O\n");
          printf("\n=== TIC TAC TOE %dx%d ===\n", SIZE, SIZE);
          printf("Player X starts.\n");
	  displayTheGrid();

          srand(time(NULL)); //gave a seeding value as a starting value for the random nuber generator 
          while(1){

               
	       if (currentPlayer=='X'){
		     getPlayerMove('X',&row,&col); //since the user is x
	       }else{
		      getComputerMoves(&row,&col,currentPlayer,num);  //get the input from computer
	       }

               markOnTheGrid(row, col, currentPlayer);  //mark movements on the beard
               gameState(moveNumber++, currentPlayer, row, col); //add game status to the file
               displayTheGrid(); //after each move show the game status
               
	       if (checkForTheWinner()) {   //after each move check the winning condition
                      displayTheGrid();
                      printf("Player %c wins!\n", currentPlayer);
                      gameResult(1, currentPlayer);
                      break;
                 } else if (checkForDraw()) {  //if there is no winning situation check for the  draw condition
                      displayTheGrid();
                      printf("It's a draw!\n");
                      gameResult(0, ' ');
                      break;
                 }
	         currentPlayer=swapPlayersAfterEachMove(currentPlayer,num);
           }
      

           break; 

    case 3:
           //run the three player game 
	    printf("You are the player 'X' and 'O' and 'Z' are two computer players\n");
            printf("\n    TIC TAC TOE  %dx%d grid   \n", SIZE, SIZE);
            printf("Player X starts.\n");
            displayTheGrid();

            srand(time(NULL)); //gave a seeding value as a starting value for the random nuber generator 
            
	    while(1){
                   if (currentPlayer=='X'){ 
			    getPlayerMove('X',&row,&col); //since the user is x
							  
	           }else if(currentPlayer=='O'){
                             getComputerMoves(&row,&col,currentPlayer,num);  //get the input from computer o

                   }else{
			  getComputerMoves(&row,&col,currentPlayer,num);  //get the input from computer z
                   }

		   markOnTheGrid(row, col, currentPlayer);  //mark movements on the beard
                   gameState(moveNumber++, currentPlayer, row, col); //add game status to the file
                   displayTheGrid(); //after each move show tj=he game status

                   if (checkForTheWinner()) {   //after each move check the winning condition
                        displayTheGrid();
                        printf("Player %c wins!\n", currentPlayer);
                        gameResult(1, currentPlayer);  
                        break;
                   } else if (checkForDraw()) {  //if there is no winning situation check for the draw 
                         displayTheGrid();
                         printf("It's a draw!\n");
                         gameResult(0, ' ');    //output the game result
                         break;
                   }
                   currentPlayer=swapPlayersAfterEachMove(currentPlayer,num);
            }
	    break;
    default:
           printf("The number is not a valid mode\n");
  }
     printf("Game log saved to 'gamelog.txt'\n");  //after switching before the next player start the game wirte the previous moves to thetxt file 
     fclose(fptr); //file is closed 
     freeCellsAfterGame(); //memory allocated  to the board is freed 

 
return 0;
}


//  allocate memory and initialize  the board

void createTheGrid() {
    grid = (char**)malloc(SIZE * sizeof(char*));  //create the memory for the array 
    for (int i = 0; i < SIZE; i++) {
        grid[i] = (char*)malloc(SIZE * sizeof(char)); //create each row by allocating memory 
        for (int j = 0; j < SIZE; j++) { //name each meory block with a index 
            grid[i][j] = ' '; // the space indicat an empty cell 
        }
    }
}

// Used this function to free memory
void freeCellsAfterGame() {
    for (int i = 0; i < SIZE; i++) {  
        free(grid[i]);  //release the memory allocated to each row
    }
    free(grid);//release the memory allocated for the board before leak happens
}

// Function to display the board
void displayTheGrid() {

    printf("\n    ");   //keep proper spacing before printing column numbers
        for(int i=0; i<SIZE;i++){
           printf(" %2d ",i); //used to align the column number to a proper space
        }


        printf("\n    ");// keep space before the row start
        for(int i=0;i<((SIZE*5)-SIZE+1);i++){  // this is the mechanism I used to create the proper
               printf("-");
       }


       printf("\n");

       //print row by row  with all the structure
      for(int i=0;i<SIZE;i++){
            printf("  %2d|",i); //This will print the row number with |
            for(int j=0;j<SIZE;j++){
                printf(" %c |",grid[i][j]); //print player  input
            }
            printf("\n    ");

            for(int i=0;i<((SIZE*5)-SIZE+1);i++){  // this is the mechanism I used to create the proper
               printf("-");
            }
            printf("\n");
      }

}

// Check if move is valid
int checkForValidMove(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {    //check for values not in  the valid range 
	    return 0;  //invalid move
    }
    if (grid[row][col] != ' ') {  //if the position is already taken 
	    return 0;    //invalid move
    }
    return 1;  //valid move
}

// Mark the board
void markOnTheGrid(int row, int col, char value) {
    grid[row][col] = value;  //access the cell chossen by the player and write the value( 'x'/ 'o'/'Z')
}

// Check for win condition
int checkForTheWinner() {
    
    // Check columns
    for (int j = 0; j < SIZE; j++) {
        char first = grid[0][j];  //get the cell value (x /o)
        if (first != ' ') {  //check the cell is not empty
            int win = 1;//have a chance to win 
            for (int i = 1; i < SIZE; i++) {
                if (grid[i][j] != first) {  //check each column  for not matching values
		       	win = 0; //no winning chance
		       	break; //stop checking the column
	       	}
            }
            if (win) 
		    return 1; //win
        }
    }
    
    // Check rows
    for (int i = 0; i < SIZE; i++) {
        char first = grid[i][0];  //get the cell value(x / o) that is why it is char
        if (first != ' ') {  // if the cell is not empty
            int win = 1; //have a chance to win
            for (int j = 1; j < SIZE; j++) {
                if (grid[i][j] != first) {    //confirm  every cell in the perticular row  don't have the same cell value(X /O)
                        win = 0; //since the values doesn't match the row values  it is not a win
                        break; //stop checking that row
                }
            }
            if (win)
                    return 1;     //if matches  winner is found
        }
    }

    // Check main diagonal
    char firstDiag = grid[0][0];
    if (firstDiag != ' ') {   //the first cel is not empty 
        int win = 1;         //have a chance to win 
        for (int i = 1; i < SIZE; i++) { 
            if (grid[i][i] != firstDiag) {    //check the next mainn diagonal value is identical 
		    win = 0;    //not identical
		    break;     //stop cheking the main diagonal
	    }
        }
        if (win) 
		return 1;        //matching values so win
    }

    // Check anti-diagonal
    char firstAnti = grid[0][SIZE-1]; //get the first  anti-diagonal cell 
    if (firstAnti != ' ') {           //check whether it is not empty 
        int win = 1;           //have a chance to win
        for (int i = 1; i < SIZE; i++) {
            if (grid[i][SIZE-1-i] != firstAnti) {     //check for identical values 
		    win = 0;           //no identical values 
		    break;      //stop checking the anti-diagonal
	    }
        }
        if (win) 
		return 1; //win 
    }

    return 0; // No win
}

// Check if board is full (draw)
int checkForDraw() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {  
            if (grid[i][j] == ' ') 
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
void getPlayerMove(char value, int *row, int *col) {
    do {
        printf("Player %c, 's turn.\n ", value); 
        acceptInput(row, col);  //ask user for input 
        if (!checkForValidMove(*row, *col)) {   //not a valid move and write directly to the vaiable as we have used pointes
            printf("Invalid move! Try again.\n");
        }
    } while (!checkForValidMove(*row, *col));
}


// making computer understand the user's winning posibility before making the next move otherwise it take long time to finish the game when it comes to larger boards

int tempMoveCheck(int row,int col,char value){
	grid[row][col]=value;     //Temperarily place the mark
	int victory=checkForTheWinner();     //check all the winning posibilities of the user 
	grid[row][col]=' '; 	//remove the predicted move  
	return victory;             // if victory is 1 user has a chance to win if it is 0 the move doesn't lead to win 
}



//try to win by  the computers

int smartWinMove(int *row,int*col,char value,int num){

  if(num ==2){
    if(value!='O'){   // in the 2 player game it remove other computer Z 
	    return 0;
    }
  }else if (num ==3){
	  if(value!='O' && value!='Z'){   //in the 3 player game accept both o and z
		  return 0;
	  }
  }
  
  for(int r=0; r<SIZE; r++){
        for (int c=0 ;c<SIZE;c++){
              if(checkForValidMove(r,c) && (tempMoveCheck(r,c,value))){
                          
                                  *row=r;
                                  *col=c;
                                  printf("Computer %c moved to (%d,%d) to win  .\n",value,*row,*col);
                                  return 1;
               }

	}

      } 
  
  return 0;  // no winning move 
}
    

// Try to block if the human  has a  chance to win 
//if there are multiple opportunities for the user to win the computer can only block one winnig choice at a time so the game will notbe draw every time

int blockOpponentsMoves(int *row,int*col,char value,int num){
 char opponents[3];
 int numOfOpponents;
 
 if(value=='O' || value =='Z'){
    if (num == 2){  // two player game mode there are only x or o
      opponents[0]='X';
      opponents[1]='O';
      numOfOpponents=2;
      
    }else if(num == 3){
	     opponents[0]='X';
             opponents[1]='O';
             opponents[2]='Z';
             numOfOpponents=3;
   }

    for(int p=0;p<numOfOpponents;p++){
       if(opponents[p] !=value){     //skip the current player
             for(int r=0;r<SIZE;r++){
	          for (int c=0;c<SIZE;c++){
		        if(checkForValidMove(r,c)){
			       if(tempMoveCheck(r,c,opponents[p])){   
				  *row=r;
				  *col=c;
				  printf("Computer %c moved to (%d,%d) to block the player 'X' .\n",value,*row,*col);
				  return 1;
			       }
		  
		      }
	  
	        }
            }
 
       }

    }
 }
 return 0;   
}

void getComputerMoves(int*row,int*col,char value,int num){

  if (num==2){   // the user chose 2nd mode
     if(value=='O'){
        if (!smartWinMove(row,col,value,num)){
             if(!blockOpponentsMoves(row,col,value,num)){
                 

                 do{
                       *row=rand()%SIZE; // range will be from 0 to (SIZE-1)
                       *col=rand()%SIZE;
                 }while(!checkForValidMove(*row,*col)); //run until a valid position is given

                 printf("Computer %c moved to : %d %d \n",value,*row,*col);
             }
       }
    }
  }else if(num ==3){   //user chose the 3rd mode 
      if(value=='O'){
        if (!smartWinMove(row,col,value,num)){
             if(!blockOpponentsMoves(row,col,value,num)){
                 //If there is no winning posibility of the user ,computer will get random numbers

                 do{
                       *row=rand()%SIZE; // range will be from 0 to (SIZE-1)
                       *col=rand()%SIZE;
                 }while(!checkForValidMove(*row,*col)); //run until a valid position is given
                                                                                                                
                 printf("Computer %c moved to : %d %d \n",value,*row,*col);
             }
       }
    }
    if(value=='Z'){
        if (!smartWinMove(row,col,value,num)){
             if(!blockOpponentsMoves(row,col,value,num)){
                 //If there is no winning posibility of the user ,computer will get random numbers

                 do{
                       *row=rand()%SIZE; // range will be from 0 to (SIZE-1)
                       *col=rand()%SIZE;
                 }while(!checkForValidMove(*row,*col)); //run until a valid position is given

                 printf("Computer %c moved to : %d %d \n",value,*row,*col);
             }
       }
    }

 }

}

// function creatded to switch players
char swapPlayersAfterEachMove(char current,int num) {

    if (num==1 || num==2){
        if (current == 'X') {     //if the current player is x the swap to next player o  or vice versa
                   return 'O' ;
        }else{
                  return 'X';    
	}
     } else if (num==3){
	
           if (current == 'X') {       //if the current player is x switch to next playe o after that the last player z
	           return 'O' ;
           }else if(current =='O') {
	          return  'Z';
           }else{
	          return 'X';  //loop back to user

           }    
    }
}

// log the current state of the board
// This will not print the whole board as we see in the gae instead of that print the positions
  
void gameState(int moveNumber, char player, int row, int col) {       
    
    fprintf(fptr, "Move %d: Player %c placed at (%d, %d)\n", moveNumber, player, row, col);
    /*for (int i = 0; i < SIZE; i++) {        //loop through each row 
        for (int j = 0; j < SIZE; j++) {   //loop through each cell in that row
            fprintf(fptr, "%c ", grid[i][j]);  //print each cell to the text file seperated by  space
        }
        fprintf(fptr, "\n"); //move to the next line after finishing one row
    }*/
    
    fprintf(fptr,"\n    ");   //keep proper spacing before printing column numbers
        for(int i=0; i<SIZE;i++){
          fprintf(fptr," %2d ",i); //used to align the column number to a proper space
        }


        fprintf(fptr,"\n    ");// keep space before the row start
        for(int i=0;i<((SIZE*5)-SIZE+1);i++){  // this is the mechanism I used to create the proper
               fprintf(fptr,"-");
       }


       fprintf(fptr,"\n");

       //print row by row  with all the structure
      for(int i=0;i<SIZE;i++){
            fprintf(fptr,"  %2d|",i); //This will print the row number with |
            for(int j=0;j<SIZE;j++){
                fprintf(fptr," %c |",grid[i][j]); //print player  input
            }
            fprintf(fptr,"\n    ");

            for(int i=0;i<((SIZE*5)-SIZE+1);i++){  // this is the mechanism I used to create the proper
               fprintf(fptr,"-");
            }
            fprintf(fptr,"\n");
      }

    fprintf(fptr, "\n"); 
    fflush(fptr);  //ensure all data to be written to the file immediately  otherwise if the program crashes all data will be lost 
}


//log the final result of the game 

void gameResult(int result, char winner) {
    if (result == 1) {                  //checkForTheWinner() return 1
	    fprintf(fptr, "GAME OVER: Player %c wins!\n\n", winner);
    } else{  
	    fprintf(fptr, "GAME OVER: It's a draw!\n\n");
    }
    fflush(fptr);                     //immediately write the result
}

// the log file has details of each move 

void createTheFile() {
    time_t now = time(NULL);              //get the current system time 
    fprintf(fptr, "    TIC-TAC-TOE GAME LOG    \n");  
    fprintf(fptr, "Date: %s", ctime(&now));             //convert the current time to readable format 
    fprintf(fptr, "Grid Size: %dx%d\n\n", SIZE, SIZE);    
    fflush(fptr); // this will write the data from buffer to disk immediately .And also if the the program crash we can prevent data loss
}


