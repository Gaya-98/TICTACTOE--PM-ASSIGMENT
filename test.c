#include<stdio.h>
// createing a function to display the board properly 

int main(){
	int size;

	printf("Enter size:");
	scanf("%d",&size);

	printf("\n    ");   //keep proper spacing before priting column numbers 
	for(int i=0; i<size;i++){
	   printf(" %2d ",i); //used to align the column number to a proper space
	}


        printf("\n    ");// keep space before the row start  
        for(int i=0;i<((size*5)-size+1);i++){  // this is the mechanism I used to create the proper  
               printf("-");
       }	       
      

       printf("\n");

       //print row by row  with all the structure  
      for(int i=0;i<size;i++){  
	    printf("  %2d|",i); //total size 3 .This will print the row number with |
	    for(int j=0;j<size;j++){
	        printf(" %c |",board[i][j]); //print player  input 
	    }   
	    printf("\n    ");
             
            for(int i=0;i<((size*5)-size+1);i++){  // this is the mechanism I used to create the proper  
               printf("-");
	    }
            printf("\n");	       
      }
     
      return 0;
}

