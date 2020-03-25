#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const int TYPE_OF_SHIP = 5;
const int BOARD_LENGTH = 8;
const int STRING_LINE_LENGTH = 50;

struct ship{
    char *name;
    int type;
    int length;
}ships[TYPE_OF_SHIP];

void initialShipList(){
    struct ship s1, s2, s3, s4, s5;
    s1.name = "Carrier";
    s2.name = "Battleship";
    s3.name = "Destroyer";
    s4.name = "Submarine";
    s5.name = "Patrol Boat";
    
    s1.length = 5;
    s2.length = 4;
    s3.length = 3;
    s4.length = 2;
    s5.length = 2;
    
    s1.type = 1;
    s2.type = 2;
    s3.type = 3;
    s4.type = 4;
    s5.type = 5;
    
    ships[0] = s1;
    ships[1] = s2;
    ships[2] = s3;
    ships[3] = s4;
    ships[4] = s5;
}

bool outOfBounds(int x, int y, int length, bool verticle){
    if(verticle){
        if(x+length-1 > BOARD_LENGTH || x<0)
            return true;
        else
            return false;
    } else{
        if(y+length-1 > BOARD_LENGTH || y<0)
            return true;
        else
            return false;
    }
}

bool checkCollide(int board[BOARD_LENGTH][BOARD_LENGTH], int x, int y, int len, bool verticle){
    if(verticle){
        for(int i=0; i<len; i++){
            if(board[x+i][y]!=0)
                return true;
        }
        return false;
    }else{
        for(int i=0; i<len; i++){
            if(board[x][y+i]!=0)
                return true;
        }
        return false;
    }
}

bool checkValid(int board[BOARD_LENGTH][BOARD_LENGTH], int x, int y, int len, bool verticle){
    if(!outOfBounds(x, y, len, verticle)){
        
        if(!checkCollide(board, x, y, len, verticle)){
            return true;
        }
        return false;
    }else{
        return false;
    }
}

bool checkConfigCmd(char *x, char y, char p){
    if((*x-'0') < 17 || (*x-'0') > 25)
        return false;
    if((y-'0') > 8 || (y-'0') < 1)
        return false;
    if(p!='H' && p!='V')
        return false;
    
    *x-=17;
    return true;
}

void getConfigurationForPlayer(int board[BOARD_LENGTH][BOARD_LENGTH]){
    for(int i=0; i<TYPE_OF_SHIP;){
        char x, y, p;
        printf("%s: ", ships[i].name);
        scanf(" %c %c %c", &x, &y, &p);
        if(checkConfigCmd(&x, y, p)){
            
            int startX = x - '0';
            int startY = y - '1';
            int len = ships[i].length;
            // printf("startX: %d, startY: %d, len: %dA\n", startX, startY, len);
            bool vert = false;
            if(p == 'V')
                vert = 1;
            // before marking, we need to test if the placement is out of bounds
            if(checkValid(board, startX, startY, len, vert)){
                for(int j=0; j<len; j++){
                    // when its verticle, mark the value from top to down
                    if(vert)
                        board[startX+j][startY] = ships[i].type;
                    else
                        board[startX][startY+j] = ships[i].type;
                }
                i++;
            }else{
                printf("Invalid ship configuration\n");
            }
        }else{
            printf("Invalid ship configuration\n");
        }
    }
}

void showPlayerBoard(int board[BOARD_LENGTH][BOARD_LENGTH]){
    printf("\n");
    printf("  ");
    for(int i=1; i<=BOARD_LENGTH; i++){
        if(i == BOARD_LENGTH)
            printf("%d", i);
        else
            printf("%d ", i);
    }
    printf("\n");
    char c = 'A';
    for(int i=0; i<BOARD_LENGTH; i++){
        printf("%c|", c);
        for(int j=0; j<BOARD_LENGTH; j++){
            printf("%d|", board[i][j]);
        }
        printf("\n");
        c++;
    }
    printf("\n");
}

void showShotsBoard(int board[BOARD_LENGTH][BOARD_LENGTH]){
    printf("\n");
    printf("  ");
    for(int i=1; i<=BOARD_LENGTH; i++){
        if(i == BOARD_LENGTH)
            printf("%d", i);
        else
            printf("%d ", i);
    }
    printf("\n");
    char c = 'A';
    for(int i=0; i<BOARD_LENGTH; i++){
        printf("%c|", c);
        for(int j=0; j<BOARD_LENGTH; j++){
            char mark = ' ';
            if(board[i][j] == 1)
                mark = 'X';
            else if(board[i][j] == -1)
                mark = '0';
            printf("%c|", mark);
        }
        printf("\n");
        c++;
    }
    printf("\n");
}

bool checkValidTarget(char *x, char y){
    if((*x-'0') < 17 || (*x-'0') > 25)
        return false;
    if((y-'0') > 8 || (y-'0') < 0)
        return false;
    
    *x-=17;
    return true;
}

bool checkIfSunk(int targetType, int targetBoard[BOARD_LENGTH][BOARD_LENGTH], int targetX, int targetY){
    int top = targetX - 1;
    if(top >= 0 && targetBoard[top][targetY] == targetType)
        return false;
    
    int bottom = targetX + 1;
    if(top < BOARD_LENGTH && targetBoard[bottom][targetY] == targetType)
        return false;
    
    int left = targetY - 1;
    if(left >= 0 && targetBoard[targetX][left] == targetType)
        return false;
    
    int right = targetY + 1;
    if(right < BOARD_LENGTH && targetBoard[targetX][right] == targetType)
        return false;
    
    return true;
}

int fireOperation(int shot_board[BOARD_LENGTH][BOARD_LENGTH], int targetBoard[BOARD_LENGTH][BOARD_LENGTH], char x, char y){
    if(!checkValidTarget(&x, y))
       return -1;
    
    // if the command is valid, do the shooting!
    int targetX = x - '0';
    int targetY = y - '1';
    
    if(shot_board[targetX][targetY]!=0){
        printf("You have already fired at this location\n\n");
        return -2;
    }
    else{
        int targetType = targetBoard[targetX][targetY];
        // if there is no object in the position of target board
        if(targetType == 0){
            shot_board[targetX][targetY] = -1;
            printf("You have missed!\n\n");
        } else{
            bool sunk = checkIfSunk(targetType, targetBoard,  targetX, targetY);
            shot_board[targetX][targetY] = 1;
            targetBoard[targetX][targetY] = 0;
            if(!sunk)
                printf("We have hit the target!\n\n");
            else{
                printf("We have sunk your %s\n\n", ships[targetType-1].name);
                return targetType;
            }
            
        }
        return 0;
        
    }
}

bool checkGameEnd(bool survivorShips[TYPE_OF_SHIP]){
    for(int i=0; i<TYPE_OF_SHIP; i++)
        if(survivorShips[i])
            return false;
    
    return true;
}

int playerMakeMoves(int shot_board[BOARD_LENGTH][BOARD_LENGTH], int targetBoard[BOARD_LENGTH][BOARD_LENGTH], bool targetSurvivorShips[TYPE_OF_SHIP]){
    while(true){
        char line[STRING_LINE_LENGTH];
        scanf(" %[^\n]",line);
        char * cmd = strtok(line, " ");
        if(strcmp(cmd, "shots") == 0){
            showShotsBoard(shot_board);
            continue;
        } else if(strcmp(cmd, "fire") == 0){
            char *tx = strtok(NULL, " ");
            if(strlen(tx) != 1){
                printf("X axis should be one character\n\n");
                continue;
            }
                
            char targetX = tx[0];
            char *ty = strtok(NULL, " ");
            if(strlen(ty) != 1){
                printf("Y axis should be a single digit\n\n");
                continue;
            }
            char targetY = ty[0];
            
            int respond = fireOperation(shot_board, targetBoard, targetX, targetY);
            if(respond == -1){
                printf("Invalid coordinates\n\n");
            }else if(respond == -2){
                // if we have shot at this pos before, repeat cmd
                continue;
            }else if(respond == 0){
                // if respond == 0 which means its have shot successfully (missed or shot but the ship not sunk)
                // switch turns and break the loop
                break;
            } else{
                targetSurvivorShips[respond-1] = false;
                break;
            }
        } else if(strcmp(cmd, "exit") == 0){
            return -1;
        } else{
            printf("Unrecognised command\n");
        }
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    initialShipList();
    // create 2 boards for two players respectively
    int board1[BOARD_LENGTH][BOARD_LENGTH];
    int board2[BOARD_LENGTH][BOARD_LENGTH];
    bool p1SurviveShips[TYPE_OF_SHIP];
    bool p2SurviveShips[TYPE_OF_SHIP];
    for(int i=0; i<TYPE_OF_SHIP; i++){
        p1SurviveShips[i] = true;
        p2SurviveShips[i] = true;
    }
    
    // create 2 shot boards for two players
    int shot_board1[BOARD_LENGTH][BOARD_LENGTH];
    int shot_board2[BOARD_LENGTH][BOARD_LENGTH];
    
    for(int i=0; i<BOARD_LENGTH; i++){
        for(int j=0; j<BOARD_LENGTH; j++){
            board1[i][j] = 0;
            board2[i][j] = 0;
            shot_board1[i][j] = 0;
            shot_board2[i][j] = 0;
        }
    }
    
    
    // get the configuration
    printf("Player 1, please set up your ships (x, y, orientation)\n");
    getConfigurationForPlayer(board1);

    printf("\n");
    printf("Player 2, please set up your ships (x, y, orientation)\n");
    getConfigurationForPlayer(board2);
    printf("\n");
    
//    showPlayerBoard(board1);
//    printf("\n\n");
//    showPlayerBoard(board2);
//    printf("\n\n");
    


    while(true){
        // player1's move
        printf("Player 1's turn\n\n");
        int status = playerMakeMoves(shot_board1, board2, p2SurviveShips);
        bool gameOver = checkGameEnd(p2SurviveShips);
        if(gameOver){
            printf("\nPlayer 1 wins!\n");
            break;
        }
        if(status == -1){
            break;
        }

        // player2's turn
        printf("Player 2's turn\n\n");
        status = playerMakeMoves(shot_board2, board1, p1SurviveShips);
        gameOver = checkGameEnd(p1SurviveShips);
        if(gameOver){
            printf("\nPlayer 2 wins!\n");
            break;
        }
        if(status == -1){
            break;
        }
    }
    
}
    
