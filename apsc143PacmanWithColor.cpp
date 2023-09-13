#include <stdio.h>
#include "colours.h"
#include <stdlib.h>
#include <string.h>
#include "conio.h"

#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'
#define ROWS 11
#define COLUMNS 11

//global variables:
char** mapFile;
char replacedTile;//stores the value of the replaced tile
char g1ReplacedTile = DOT;//stores the value of the replaced tile by ghost 1, the ghosts start on a dot
char g2ReplacedTile = DOT;//stores the value of the replaced tile by ghost 2, the ghosts start on a dot
char end = '\0';
char mapP [11][11];
int ghost1Row = 0;
int ghost1Column = 0;
int ghost2Row = 0;
int ghost2Column = 0;
char g1Direction;
int g1Increment;
char g2Direction;
int g2Increment;

// Below are the required functions for this program, but you will need to create your own as well.
// Make sure to specify the required parameters, if any.

void pacmanMovement(char direction, int increment);
void addWalls();
void cpyPointertoArray();
void readMap();
void printMap();
int winCheck();
int loseCheck();
int isWall(int row, int column);
void playerInput();
void pacmanMovement(char direction, int increment);

//functions for movement for ghost 1
void ghost1Movement();
void ghost1Follow();
void ghost1Random();
int ghost1DetermineMovement();

//functions for movement for ghost 2
void ghost2Movement();
void ghost2Follow();
void ghost2Random();
int ghost2DetermineMovement();

//for all ghosts
void ghostLocation();


void readMap() {//read the map and find out where all the elements are in dynamic memory allocation.
    //creating a pointer to read the file and reading map.txt
    FILE *fp = fopen("map.txt", "r");
    mapFile = (char **) malloc(11 * 11 * sizeof(char *));

    if (fp == NULL) {
        printf("File not found.");
    }

    for (int i = 0; i < 11; ++i) {
        mapFile[i] = (char *) malloc(11 * sizeof(char));
        for (int j = 0; j < 11; ++j) {
            fscanf(fp, "%c  ", &mapP[i][j]);
        }
    }
    addWalls();
    cpyPointertoArray();
}

void addWalls(){
    for (int i = 0; i < 11; ++i) {
        mapP[0][i] = WALL;
        mapP[10][i] = WALL;
        mapP[i][0] = WALL;
        mapP[i][10] = WALL;
    }
}

void cpyPointertoArray() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            mapP[i+1][j+1] = mapFile[i][j];
        }
    }
}

void printMap(){
    //the for loop prints each row into the file
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; ++j) {
            printf("%c  ", mapP[i][j]);
            if(mapP[i][j]==PACMAN)
                colourChange(YELLOW);
            else if(mapP[i][j]==GHOST)
                colourChange(PINK);
            else if(mapP[i][j] == DOT)
                colourChange(WHITE);
            else if (mapP[i][j]==WALL)
                colourChange(BLUE);
        }
        printf("\n");
    }
    printf("\n");
}

int winCheck(){
    int dotCounter = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (mapP[i][j] == DOT){
                dotCounter++;
            }
        }
    }
    if (dotCounter == 0){
        printMap();
        printf("Congratulations! You win! Press any key to exit the game\n");
        end = getch();
        return 1;
    }
    return 0;
}

int loseCheck(){
    if (replacedTile == GHOST){
        printMap();
        printf("Sorry, you lose. Press any key to exit the game\n");
        end = getch();
        return 1;
    }
    else if (g1ReplacedTile == PACMAN){
        printMap();
        printf("Sorry, you lose. Press any key to exit the game\n");
        end = getch();
        return 1;
    }
    else if (g2ReplacedTile == PACMAN){
        printMap();
        printf("Sorry, you lose. Press any key to exit the game\n");
        end = getch();
        return 1;
    }
    else{
        return 0;
    }
}

int isWall(int row, int column){//checking to see if the location with the specified coordinates and direction
    if (mapP[row][column] == WALL){
        return 1;//return true if there is a wall
    }
    else{
        return 0;//return false if there is no wall
    }
}

void playerInput(){
    //getting user input
    char userInput;
    userInput = getch();
    fflush(stdin);//removing any extra inputs which may be inputted in the console
    //depending on user input the increment and direction are being passed
    //for example a movement up means a movement along the rows, where the pacman is moving to the row above it,
    //therefore it has a values of r for row and -1 for moving to the previous row
    if (userInput == UP){
        pacmanMovement('r', -1);
    }
    else if (userInput == DOWN){
        pacmanMovement('r', +1);
    }
    else if (userInput == RIGHT){
        pacmanMovement('c', +1);
    }
    else if (userInput == LEFT){
        pacmanMovement('c', -1);
    }
    else{//this error should never happen since all cases are taken care of
        printf("Error in player input function\n");
    }
}

void pacmanMovement(char direction, int increment) {
    //finding current location of the pacman
    int row, column;
    int rRow = 0;
    int rColumn = 0; //these variables are where the player is requesting (r) to move the pacman
    for (int i=0; i<11; i++){
        for(int j=0; j<11; j++){
            if (mapP[i][j] == 'P'){
                row = i;
                column = j;
                break;//breaking out of the loop because it found pacman
            }
        }
    }
    //moving pacman to new location as specified by the users input:
    //noting where the move is being attempted
    if (direction == 'r'){
        rRow = row + increment;
        rColumn = column;
    }
    else if (direction == 'c'){
        rColumn = column + increment;
        rRow = row;
    }
    else{
        printf("Error in finding where the pacman is trying to move");
    }
    //first checking to make sure that there are no walls in that direction
    //wall is a boolean variable if there is a wall it will equal 1, if not then it will equal 0
    int wall = isWall(rRow,rColumn);
    if (wall == 0){
        replacedTile = mapP[rRow][rColumn]; //copying down replaced tile for later reference
        mapP[rRow][rColumn] = PACMAN;//moving pacman to the spot if all criteria is met
        mapP[row][column] = ' ';//removing the old character at that spot and replacing it with a space
    }
}

void ghostLocation(){
    //finding ghost
    //resetting the ghost index
    ghost1Row = 0;
    ghost1Column = 0;
    ghost2Row = 0;
    ghost2Column = 0;
    //finding location of ghost 1 and 2
    for (int i=0; i < 11; i++){
        for (int j=0; j < 11; j++){
            if (mapP[i][j] == GHOST){
                //if ghost1 doesn't have values then this must be the first ghost to be read
                //therefore the values go to ghost1, same logic applies for ghost 2
                if(ghost1Row == 0 && ghost1Row == 0){
                    ghost1Row = i;
                    ghost1Column = j;
                }
                else if (ghost2Row == 0 && ghost2Column == 0){
                    ghost2Row = i;
                    ghost2Column = j;
                }
                else{
                    printf("Error in finding ghost for loop\n");
                }
            }
        }
    }
}

void ghost1Movement(){
    ghostLocation();
    //check what type of movement is required (Random or Follow)
    //storing the direction and increment so the host can follow if needed

    int movementTypeG1 = ghost1DetermineMovement();

    //random movement cannot see pacman
    if (movementTypeG1 == 0){
        ghost1Random();
    }
        //follow pacman because the ghost can see the pacman
    else if(movementTypeG1 == 1){
        ghost1Follow();
    }
}

int ghost1DetermineMovement(){
    //boolean values for if ghost 1 sees pacman in the specified direction
    int down1 = 0, up1 = 0, left1 = 0, right1 = 0;
    g1Direction = ' ';
    g1Increment = ' ';
    //checking vertically down for ghost1 from its current location
    for(int i=ghost1Row; i<(11-ghost1Row); i++){
        if (mapP[i][ghost1Column] == WALL){
            down1 = 0;
        }
        else if (mapP[i][ghost1Column] == PACMAN){
            down1 = 1;
            g1Direction = 'r';
            g1Increment = 1;
            return 1;
        }
    }
    //checking vertically up for ghost 1 from its current location
    for(int i=ghost1Row; i<(1-(11-ghost1Row)); i--){
        if (mapP[i][ghost1Column] == WALL){
            up1 = 0;
        }
        else if (mapP[i][ghost1Column] == PACMAN){
            up1 = 1;
            g1Direction = 'r';//the pacman was found in the vertical direction
            g1Increment = -1;//the pacman is above
            return 1;
        }
    }
    //checking horizontally right for ghost 1 from its current location
    for(int i=ghost1Column; i<(11-ghost1Column); i++){
        if (mapP[ghost1Row][i] == WALL){
            right1 = 0;
        }
        else if (mapP[ghost1Row][i] == PACMAN){
            right1 = 1;
            g1Direction = 'c';//the pacman was found in the vertical direction
            g1Increment = 1;//the pacman is above
            return 1;
        }
    }
    //checking horizontally left for ghost1 from its current location
    for(int i=ghost1Column; i<(1-(11-ghost1Column)); i--){
        if (mapP[ghost1Row][i] == WALL){
            left1 = 0;
        }
        else if (mapP[ghost1Row][i] == PACMAN){
            left1 = 1;
            g1Direction = 'r';//the pacman was found in the vertical direction
            g1Increment = -1;//the pacman is above
            return 1;
        }
    }
    //if statement is checking if pacman was found in any directions
    //if not then return 0, as in ghost 1 cannot see pacman
    if((up1 + down1 + left1 + right1) == 0){
        return 0;
    }
    else{
        printf("Error in ghost 1 finding pacman");
    }
}

void ghost1Random(){
    int g1Wall = 0; //boolean value storing if there is a wall where the ghost is trying to move
    int rG1Row = 0;
    int rG1Column = 0;
    //the loop will repeat as long there is an obstacle in the ghosts way
    do{
        rG1Row = 0;
        rG1Column = 0;
        //value of 0 - up, 1 - down, 2 - left, 3 - right
        int rNum = rand()%4;
        if (rNum == 0){//move up
            rG1Row = ghost1Row - 1;
            rG1Column = ghost1Column;
        }
        else if (rNum == 1){//move down
            rG1Row = ghost1Row + 1;
            rG1Column = ghost1Column;
        }
        else if (rNum == 2){//move left
            rG1Row = ghost1Row;
            rG1Column = ghost1Column-1;
        }
        else if (rNum == 3){//move right
            rG1Row = ghost1Row;
            rG1Column = ghost1Column + 1;
        }
        else{
            printf("Error in random movement of ghost 1");
        }
        g1Wall = isWall(rG1Row, rG1Column);//determining if there is wall where the ghost 1 is trying to move
        if (mapP [rG1Row][rG1Column] == GHOST){//checking if there is a ghost where the pacman is trying to move
            g1Wall = 1;
        }
    }while (g1Wall == 1);
    //actually moving the ghost to the spot since all checks and coordinates have been determined
    //likely an error in the next three line
    g1ReplacedTile = mapP[rG1Row][rG1Column];
    mapP[rG1Row][rG1Column] = GHOST;
    mapP[ghost1Row][ghost1Column] = g1ReplacedTile;
}

void ghost1Follow(){
    int rG1Row = 0, rG1Column = 0;
    if (g1Direction == 'r'){
        rG1Row = ghost1Row + g1Increment;
        rG1Column = ghost1Column;
    }
    else if (g1Direction == 'c'){
        rG1Row = ghost1Row;
        rG1Column = ghost1Column + g1Increment;
    }
    int wall = isWall(rG1Row,rG1Column);
    if (wall == 0){
        //likely an error in the next three line
        g1ReplacedTile = mapP[rG1Row][rG1Column];
        mapP[ghost1Row][ghost1Column] = g1ReplacedTile;
        mapP[rG1Row][rG1Column] = GHOST;
    }
}

void ghost2Movement(){
    ghostLocation();
    //check what type of movement is required (Random or Follow)
    //storing the direction and increment so the host can follow if needed

    int movementTypeG2 = ghost2DetermineMovement();

    //random movement cannot see pacman
    if (movementTypeG2 == 0){
        ghost2Random();
    }
        //follow pacman because the ghost can see the pacman
    else if(movementTypeG2 == 1){
        ghost2Follow();
    }
    else{
        printf("Error in determining ghost movement type for ghost 2");
    }
}

int ghost2DetermineMovement(){
    //boolean values for if ghost 1 and 2 see pacman in the specified direction
    int down2 = 0, up2 = 0, left2 = 0, right2 = 0;
    g2Direction = ' ';
    g2Increment = ' ';
    //checking vertically down for ghost1 from its current location
    for(int i=ghost2Row; i<(11-ghost2Row); i++){
        if (mapP[i][ghost2Column] == WALL){
            down2 = 0;
        }
        else if (mapP[i][ghost2Column] == PACMAN){
            down2 = 1;
            g2Direction = 'r';
            g2Increment = 1;
            return 1;
        }
    }
    //checking vertically up for ghost 1 from its current location
    for(int i=ghost2Row; i<(1-(11-ghost2Row)); i--){
        if (mapP[i][ghost2Column] == WALL){
            up2 = 0;
        }
        else if (mapP[i][ghost2Column] == PACMAN){
            up2 = 1;
            g2Direction = 'r';//the pacman was found in the vertical direction
            g2Increment = -1;//the pacman is above
            return 1;
        }
    }
    //checking horizontally right for ghost 1 from its current location
    for(int i=ghost2Column; i<(11-ghost2Column); i++){
        if (mapP[ghost2Row][i] == WALL){
            right2 = 0;
        }
        else if (mapP[ghost2Row][i] == PACMAN){
            right2 = 1;
            g2Direction = 'c';//the pacman was found in the vertical direction
            g2Increment = 1;//the pacman is above
            return 1;
        }
    }
    //checking horizontally left for ghost1 from its current location
    for(int i=ghost2Column; i<(1-(11-ghost2Column)); i--){
        if (mapP[ghost2Row][i] == WALL){
            left2 = 0;
        }
        else if (mapP[ghost2Row][i] == PACMAN){
            left2 = 1;
            g2Direction = 'r';//the pacman was found in the vertical direction
            g2Increment = -1;//the pacman is above
            return 1;
        }
    }
    //if statement is checking if pacman was found in any directions
    //if not then return 0, as in ghost 2 cannot see pacman
    if((up2 + down2 + left2 + right2) == 0){
        return 0;
    }
    else{
        printf("Error in ghost 2 finding pacman");
    }
}

void ghost2Random(){
    int g2Wall = 0; //boolean value storing if there is a wall where the ghost is trying to move
    int rG2Row = 0;
    int rG2Column = 0;
    //the loop will repeat as long there is an obstacle in the ghosts way
    do{
        rG2Row = 0;
        rG2Column = 0;
        //value of 0 - up, 1 - down, 2 - left, 3 - right
        int rNum = rand()%4;
        if (rNum == 0){//move up
            rG2Row = ghost2Row - 1;
            rG2Column = ghost2Column;
        }
        else if (rNum == 1){//move down
            rG2Row = ghost2Row + 1;
            rG2Column = ghost2Column;
        }
        else if (rNum == 2){//move left
            rG2Row = ghost2Row;
            rG2Column = ghost2Column-1;
        }
        else if (rNum == 3){//move right
            rG2Row = ghost2Row;
            rG2Column = ghost2Column + 1;
        }
        else{
            printf("Error in random movement of ghost 2");
        }
        g2Wall = isWall(rG2Row, rG2Column);//determining if there is wall where the ghost 2 is trying to move
        if (mapP [rG2Row][rG2Column] == GHOST){//checking if there is a ghost where the pacman is trying to move
            g2Wall = 1;
        }
    }while (g2Wall == 1);
    //actually moving the ghost to the spot since all checks and coordinates have been determined
    //likely an error in the next three line
    g1ReplacedTile = mapP[rG2Row][rG2Column];
    mapP[rG2Row][rG2Column] = GHOST;
    mapP[ghost2Row][ghost2Column] = g2ReplacedTile;
}

void ghost2Follow(){
    int rG2Row = 0, rG2Column = 0;
    if (g2Direction == 'r'){
        rG2Row = ghost2Row + g2Increment;
        rG2Column = ghost2Column;
    }
    else if (g2Direction == 'c'){
        rG2Row = ghost2Row;
        rG2Column = ghost2Column + g2Increment;
    }
    int wall = isWall(rG2Row,rG2Column);
    if (wall == 0){
        //likely an error in the next three line
        g2ReplacedTile = mapP[rG2Row][rG2Column];
        mapP[ghost2Row][ghost2Column] = g2ReplacedTile;
        mapP[rG2Row][rG2Column] = GHOST;
    }
}

int main() {
    //reading the map which fills the 2d character pointer array
    readMap();
    while (end == '\0') {
        printMap();
        playerInput();
        ghost1Movement();
        ghost2Movement();
        winCheck();
        loseCheck();
    }
    return 0;
}