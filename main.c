

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int isMine(char** board,int i,int j); /** function that checks if i,j index is a bomb and returns 1 else 0 **/
int out_range(int row,int col,int n); /** function that checks if indexes row,col are out of range range 0 -> n-1 ,and returns 1 else 0 **/
char** create_board(int n); /** create n*n board return NULL if malloc failed **/
int add_bomb(char** board,int n,int row,int col); /** function to add bomb in row,col index and add to the around squares counter **/
int deterministic_init(char** board,int n,int m); /** deterministic initialize the board with m bombs **/
int random_init(char** board,int n,int m); /** random initialize n*n board with m bombs **/
int InitBoards(char*** board_a,char*** flags_a,int n,int m); /** initialize the board **/
int print_game(char** board,char** flags,int n); /** function to print the main board according to the flags board ( if squares are opened/closed/flagged **/
int open_square(char** board,char** flags,int row,int col,int* opened_sqaures,int n); /** open new square in row,col index and open the around squares recursivly if it includes 0 **/
int set_flag(char** board,char** flags,int row,int col,int n); /** function to set flag return 1 if flagged succefly -1 if the sqaure was a flag and return it to 0 (unflag) , else return 0 (if already flagged) **/
int flag_counter(char** board,char** flags,int n); /** count flagged squares in flags board **/
int play_game(char** board,char** flags,int n,int m); /** play the game according to users inputs **/

/*********************************************************************************************************/

int isMine(char** board,int i,int j){ /** function that checks if i,j index is a bomb and returns 1 else 0 **/
    if(board[i][j]=='X'){
        return 1;
    }
    return 0;
}

int out_range(int row,int col,int n){ /** function that checks if indexes row,col are out of range range 0 -> n-1 ,and returns 1 else 0 **/
    if(row<0||col<0||row>=n||col>=n){
        return 1;
    }
    return 0;
}

char** create_board(int n){ /** create n*n board return NULL if malloc failed **/
    int i;
    char** board=NULL;
    if((board=(char**)malloc(n*sizeof(char*)))==NULL){
        return NULL;
    }
    for(i=0;i<n;i++){
        if((board[i]=(char*)malloc(n*sizeof(char)))==NULL){
            return NULL;
        }
    }
    return board;
}

int add_bomb(char** board,int n,int row,int col){ /** function to add bomb in row,col index and add to the around squares counter **/
    int i,j;
    if(!isMine(board, row, col)){
        board[row][col]='X';
        for(i=row-1;i<row+2 ;i++){
            for(j=col-1;j<col+2 ;j++){
                if(i>=0 && i<n && j>=0 && j<n && isMine(board, i, j)==0){
                    board[i][j]++;
                }
            }
        }
    }else{return 0;}
    return 1;
    
}

int deterministic_init(char** board,int n,int m){ /** deterministic initialize the board with m bombs **/
    int counter=m,row,col;
    while(counter>0){ /** add M bombs **/
        printf("There are %d unselected mines left\n",counter);
        printf("Please enter row and column numbers (i,j)\n");
        if(scanf("%d %d",&row,&col)<2){
            return 0;
        }
        while(out_range(row, col, n)){ /** check if indexes in range **/
            printf("Coordinates out of range! Please try again\n");
            if(scanf("%d %d",&row,&col)<2){
                return 0;
            }
        }
        while(board[row][col]=='X'){ /** check if the square is already open **/
            printf("This square includes a mine, try another one\n");
            if(scanf("%d %d",&row,&col)<2){
                return 0;
            }
        }
        if(add_bomb(board, n, row, col)==0){ /** if the indexes are valid add a bomb and check if adding the bomb succeed **/
            return 0;
        }
        counter--;
    }
    return 1;
}

int random_init(char** board,int n,int m){ /** random initialize n*n board with m bombs **/
    int counter=m,row,col;
    srand(time(NULL));
    while(counter>0){
        do{ /** generate indexes that does not include a mine **/
            row=(rand()%n);
            col=(rand()%n);
        }while(isMine(board, row, col));
        if(add_bomb(board, n, row, col)==0){ /** add bomb and check if adding succeed **/
            return 0;
        }
        counter--;
    }
    return 1;
}


int InitBoards(char*** board_a,char*** flags_a,int n,int m){ /** initialize the board **/
    int i,j,game;
    char** board=NULL,**flags=NULL;
    *board_a=create_board(n); /** create boards **/
    *flags_a=create_board(n);
    board=*board_a;
    flags=*flags_a;
    if(flags==NULL||board==NULL){ /** check if the boards were created succefully **/
        exit(-1);
    }
    for(i=0;i<n;i++){ /** initialize the boards with '0' **/
        for(j=0;j<n;j++){
            flags[i][j]='0';
            board[i][j]='0';
        }
    } /** get input - deterministic or random initialize **/
    printf("would you like to play the random or the deterministic version?\n1.random 2.deterministic\n");
    if(scanf("%d",&game)!=1){
        return 0;
    }
    while(game<1||game>2){ /** check the input **/
        printf("you should enter 1(random) or 2(deterministic)\nplease try again\n");
        if(scanf("%d",&game)!=1){
            return 0;
        }
    }
    switch (game) { /** initialize the boards according to the user's input **/
        case 1:
            if(random_init(board, n, m)){
                return 1; /** init succeed **/
            }
            break;
            
        case 2:
            if(deterministic_init(board, n, m)){
                return 1;
            }
            break;
    }
    return 0;
}

int print_game(char** board,char** flags,int n){ /** function to print the main board according to the flags board ( if squares are opened/closed/flagged **/
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(flags[i][j]=='0'){ /** if closed print # **/
                printf("%c",35);
            }else if(flags[i][j]=='1'){ /** if open print the around mines counter **/
                printf("%c",board[i][j]);
            }else if(flags[i][j]=='F'){ /** if flagged print F **/
                printf("%c",'F');
            }
        }
        printf("\n");
    }
    return 0;
}



int open_square(char** board,char** flags,int row,int col,int* opened_sqaures,int n){ /** open new square in row,col index and open the around squares recursivly if it includes 0 **/
    if(out_range(row, col, n)){ /** check if the indexes are in range **/
        return 0;
    }
    else if(flags[row][col]=='0' && board[row][col]!='0'){ /** if the square is closed and its mines around counter is not 0 the square and update the flags counter **/
        flags[row][col]='1';
        (*opened_sqaures)++;
        return 1;
    }else if(board[row][col]=='0' && (flags[row][col]=='0'||flags[row][col]=='F')){ /** if the square is closed and its mines around counter is 0 the square and the around squares recursively and update the flags counter **/
        flags[row][col]='1';
        (*opened_sqaures)++;
        open_square(board, flags, row+1, col,opened_sqaures, n); /** open up and down square **/
        open_square(board, flags, row-1, col,opened_sqaures, n);
        open_square(board, flags, row, col-1,opened_sqaures, n); /** open right and left squares **/
        open_square(board, flags, row, col+1,opened_sqaures, n);
        open_square(board, flags, row+1, col+1,opened_sqaures, n); /** open squares in diagonal direction **/
        open_square(board, flags, row-1, col-1,opened_sqaures, n);
    }
    return 1;
}



int set_flag(char** board,char** flags,int row,int col,int n){ /** function to set flag return 1 if flagged succefly -1 if the sqaure was a flag and return it to 0 (unflag) , else return 0 (if already flagged) **/
    if(flags[row][col]=='F'){
        flags[row][col]='0';
        return -1;
    }else if(flags[row][col]=='0'){
        flags[row][col]='F';
        return 1;
    }
    return 0;
}

int flag_counter(char** board,char** flags,int n){ /** count flagged squares in flags board **/
    int f=0,i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(flags[i][j]=='F' && board[i][j]=='X'){
                f++;
            }
        }
    }
    return f;
}



int play_game(char** board,char** flags,int n,int m){ /** play the game **/
    int input,opened_squares=0;
    while(opened_squares!=n*n-m){ /** while all squares that does not include a mine are opened **/
        print_game(board, flags, n); /** print board **/
        printf("There are %d mines left.\nWhat would you like to do next?\n1. Open a new square\n2. Flag a square as a mine\n",m-flag_counter(board,flags, n)); /** ask to the next iteration - open/flag a square **/
        if(scanf("%d",&input)!=1){return 0;}
        if(input==1){ /** if 1 - open new square **/
            int row,col;
            printf("Please insert row and column numbers: "); /** ask for the desired index to open **/
            if(scanf("%d %d",&row,&col)<2){return 0;}
            if(out_range(row, col, n)){ /** check if input is in range **/
                printf("Coordinates out of range! Please try again\n");
            }else if(flags[row][col]=='1'){ /** check if the square is already open **/
                printf("This square is already open\n");
            }else{
                if(!open_square(board, flags,row,col,&opened_squares, n)){  /** if the input is valid open the square **/
                    return 0; /** check if the opening succeed **/
                }
                if(board[row][col]=='X'){ /** if the opened square is a mine end the game with losing message **/
                    print_game(board, flags, n);
                    printf("Boom!! You have lost\n");
                    return 1;
                }
            }
        }else if(input==2){ /** if 2 - flag a square **/
            int row,col;
            printf("Please insert row and column numbers: "); /** ask for indexes to flag **/
            if(scanf("%d %d",&row,&col)!=2){return 0;}
            if(out_range(row, col, n)){ /** check if input is in range **/
                printf("Coordinates out of range! Please try again\n");
            }else if(flags[row][col]=='1'){ /** check if the square is already open **/
                printf("This square is already open\n");
            }else if(set_flag(board, flags, row, col, n)==-1){ /** if indexes are valid open the square **/
                opened_squares++;
            }
        }
    }
    print_game(board, flags, n); /** if all the square that does npot include a mine are opened print winning message and end the game **/
    printf("Congratulations!! You have won!!\n");
    return 1;
    
}

int main()
{
    int n,m;
    char** board = NULL,**flags = NULL;
    printf("*****************************************************************\n");
    printf("\nWelcome to MineSweeper!\n");
    printf("\n*****************************************************************\n\n");
    printf("Please enter the size of the board and number of bombs: ");
    if(scanf("%d %d",&n,&m)<2){
        return 0;
    }
    if(InitBoards(&board, &flags, n, m)==0){ /** init board and check if initializing succeed if failed return 0 and end the game **/
        return 0;
    }
    printf("\n*****************************************************************\n");
    printf("\n The boards were initialized successfully! lets start the game\n");
    printf("\n**************************(flags[r][c]***************************************\n\n");
    if(play_game(board, flags, n, m)){ /** play the game if the game was ended successfully (win or lose) return 1 **/
        return 1;
    }
    return 0; /** if the game didn't end successfully return 0 **/
}
