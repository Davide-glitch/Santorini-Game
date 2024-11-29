///Santorini.c

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEVEL 3

#define RED     "\x1B[31m"
#define GRN     "\x1B[32m"
#define YEL     "\x1B[33m"
#define BLU     "\x1B[34m"
#define MAG     "\x1B[35m"
#define CYN     "\x1B[36m"
#define WHT     "\x1B[37m"
#define GRY     "\x1B[90m"
#define RESET   "\x1B[0m"
#define PURPLE  "\x1B[35m"
#define GREEN   "\x1B[32m"

#define PINK    "\x1B[95m"
#define LIGHT_BLUE "\x1B[94m"
#define ORANGE  "\x1B[33m"
#define LIGHT_GREEN "\x1B[92m"
#define LIGHT_CYAN "\x1B[96m"
#define LIGHT_RED "\x1B[91m"
#define LIGHT_PURPLE "\x1B[95m"
#define LIGHT_GRAY "\x1B[37m"
#define DARK_GRAY "\x1B[90m"
#define LIGHT_YELLOW "\x1B[93m"


#define DOME_COLOR MAG

typedef struct {
    int x;
    int y;
    int level;
    char worker;
    bool isDome;
} Square;

typedef struct {
    char name[50];
    char workerColor;
    Square workers[2];
} Player;

// Function prototypes
void initMap(Square map[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].level = 0;
            map[i][j].worker = ' ';
            map[i][j].isDome = false;
        }
    }
}


void configurePlayer(Player *player, const char *name, char color) {
    strncpy(player->name, name, sizeof(player->name) - 1);
    player->name[sizeof(player->name) - 1] = '\0'; // Ensure null termination
    player->workerColor = color;
    for (int i = 0; i < 2; i++) {
        player->workers[i].x = -1;
        player->workers[i].y = -1;
        player->workers[i].level = 0;
        player->workers[i].worker = color;
        player->workers[i].isDome = false;
    }
}

// Function to print the map with potential moves in green
void printPossibleMoves(Square map[5][5], Player *currentPlayer, int workerNum) {
    Square worker = currentPlayer->workers[workerNum];
    int currentX = worker.x - 1;
    int currentY = worker.y - 1;

    printf("\nPossible moves for worker %d:\n", workerNum + 1);
    printf("\n   1   2   3   4   5\n\n");
    for (int i = 0; i < 5; i++) {
        printf("%d  ", i + 1);
        for (int j = 0; j < 5; j++) {
            // Check if the square is a valid move location
            if (!map[i][j].isDome && map[i][j].worker == ' ' &&
                abs(i - currentX) <= 1 && abs(j - currentY) <= 1 &&
                map[i][j].level <= map[currentX][currentY].level + 1) {
                printf(GREEN "%d   " RESET, map[i][j].level);
            } else {
                printf("%c   ", map[i][j].isDome ? 'D' : map[i][j].level + '0');
            }
        }
        printf("\n\n");
    }
}

// Function to print the map with potential build locations in yellow
void printPossibleBuilds(Square map[5][5], Player *currentPlayer, int workerNum) {
    Square worker = currentPlayer->workers[workerNum];
    int currentX = worker.x - 1;
    int currentY = worker.y - 1;

    printf("\nPossible builds for worker %d:\n", workerNum + 1);
    printf("\n   1   2   3   4   5\n\n");
    for (int i = 0; i < 5; i++) {
        printf("%d  ", i + 1);
        for (int j = 0; j < 5; j++) {
            // Check if the square is a valid build location
            if (!map[i][j].isDome && map[i][j].worker == ' ' &&
                abs(i - currentX) <= 1 && abs(j - currentY) <= 1) {
                printf(YEL "%d   " RESET, map[i][j].level);
            } else {
                printf("%c   ", map[i][j].isDome ? 'D' : map[i][j].level + '0');
            }
        }
        printf("\n\n");
    }
}

// Function to print the game state with colors
void printGameStateWithColors(Square map[5][5], Player players[], int numPlayers) {
    printf("\n   1   2   3   4   5\n\n");
    for (int i = 0; i < 5; i++) {
        printf("%d  ", i + 1);
        for (int j = 0; j < 5; j++) {
            char domeChar = map[i][j].isDome ? 'D' : ' ';
            char workerChar = map[i][j].worker;
            int workerNum = 0;
            // Determine the worker number if there is a worker on the square
            if (workerChar != ' ') {
                for (int p = 0; p < numPlayers; p++) {
                    for (int w = 0; w < 2; w++) {
                        if (players[p].workers[w].x == i + 1 && players[p].workers[w].y == j + 1) {
                            workerNum = w + 1;
                            break;
                        }
                    }
                }
            }
            // Print the square with worker and level
            if (workerNum > 0) {
                const char *colorCode;
                switch (workerChar) {
                    case 'B': colorCode = BLU; break;
                    case 'W': colorCode = WHT; break;
                    case 'G': colorCode = GRY; break;
                    case 'b': colorCode = BLU; break;
                    case 'w': colorCode = WHT; break;
                    case 'g': colorCode = GRY; break;
                    case 'R': colorCode = RED; break;
                    case 'r': colorCode = RED; break;
                    case 'P': colorCode = PINK; break;
                    case 'p': colorCode = PINK; break;
                    case 'L': colorCode = LIGHT_BLUE; break;
                    case 'l': colorCode = LIGHT_BLUE; break;
                    default: colorCode = RESET;
                }
                printf("%s%d%c%d%s ", colorCode, map[i][j].level, workerChar, workerNum, RESET);
            } else if (domeChar == 'D') {
                printf(DOME_COLOR "%c   " RESET, domeChar);
            } else {
                printf("%d   ", map[i][j].level);
            }
        }
        printf("\n\n");
    }
}



bool moveWorker(Square map[5][5], Player *player, int workerNum, int newX, int newY) {
    // Adjust for 1-based indexing
    newX--;
    newY--;

    // Get the current position of the worker
    int currentX = player->workers[workerNum].x - 1;
    int currentY = player->workers[workerNum].y - 1;

    // Check if the move is within the bounds of the map
    if (newX < 0 || newX >= 5 || newY < 0 || newY >= 5) {
        return false;
    }

    // Check if the move is within one square in any direction
    if (abs(newX - currentX) > 1 || abs(newY - currentY) > 1) {
        return false; // Move is outside the allowed range
    }

    // Check if the destination square is occupied by a dome or another worker
    if (map[newX][newY].isDome || map[newX][newY].worker != ' ') {
        return false;
    }

    // Check if the worker is moving up more than one level
    if (map[newX][newY].level > map[currentX][currentY].level + 1) {
        return false;
    }

    // Move the worker
    map[currentX][currentY].worker = ' ';
    map[newX][newY].worker = player->workerColor;
    player->workers[workerNum].x = newX + 1;
    player->workers[workerNum].y = newY + 1;

    return true;
}

bool build(Square map[5][5], Player *player, int workerNum, int x, int y) {
    // Adjust for 1-based indexing
    x--;
    y--;

    // Get the current position of the worker
    int currentX = player->workers[workerNum].x - 1;
    int currentY = player->workers[workerNum].y - 1;

    // Check if the build coordinates are within the bounds of the map
    if (x < 0 || x >= 5 || y < 0 || y >= 5) {
        return false;
    }

    // Check if the square is adjacent to the worker's position
    if (abs(x - currentX) > 1 || abs(y - currentY) > 1) {
        return false; // Build location is not adjacent
    }

    // Check if the square is occupied by a dome or worker
    if (map[x][y].isDome || map[x][y].worker != ' ') {
        return false;
    }

    // Increase the level of the square
    map[x][y].level++;

    // If the new level is 4, replace it with a dome
    if (map[x][y].level == MAX_LEVEL) {
        map[x][y].isDome = true;
        map[x][y].level = 0; // Reset level to 0 as the dome is now placed
    }

    return true;
}

bool checkWinCondition(Square map[5][5], Player player, bool cronusPowerUsed) {
    // Check for a worker on level 3
    for (int i = 0; i < 2; i++) {
        int x = player.workers[i].x - 1;
        int y = player.workers[i].y - 1;

        if (map[x][y].level == 3) {
            return true; // The player wins if their worker is on level 3
        }
    }

    // Check for Cronus power win condition
    int domeCount = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (map[i][j].isDome) {
                domeCount++;
            }
        }
    }

    // Cronus wins if there are 5 or more domes on the board
    if (domeCount >= 5 && cronusPowerUsed) {
        return true;
    }

    return false; // No win condition met
}


void placeWorkers(Square map[5][5], Player players[], int numPlayers) {
    int numWorkers = 2; // Assuming each player has 2 workers

    // Loop for each worker
    for (int w = 0; w < numWorkers; w++) {
        // Loop for each player
        for (int p = 0; p < numPlayers; p++) {
            int x, y;
            bool validPlacement = false;

            // Ask for worker placement until a valid position is chosen
            while (!validPlacement) {
                printf("Player %s, enter the coordinates to place worker %d (X Y): ", players[p].name, w + 1);
                scanf("%d %d", &x, &y);
                x--; // Adjust for 0-based indexing
                y--; // Adjust for 0-based indexing

                // Check if the coordinates are valid and the square is not occupied
                if (x >= 0 && x < 5 && y >= 0 && y < 5 && map[x][y].worker == ' ') {
                    map[x][y].worker = players[p].workerColor; // Assign worker color based on player
                    players[p].workers[w].x = x + 1; // Store the position with 1-based indexing
                    players[p].workers[w].y = y + 1;
                    validPlacement = true;
                } else {
                    printf("Invalid coordinates. Please try again.\n");
                }
            }
        }
    }
}



bool zeusPower(Square map[5][5], Player *player, int workerNum) {
    int x = player->workers[workerNum].x - 1;
    int y = player->workers[workerNum].y - 1;

    // Zeus's power cannot be used to win the game by moving from level 2 to 3.
    if (map[x][y].level == 2) {
        return false; // Cannot use Zeus's power to move to level 3 and win
    }

    // Zeus's power allows a worker to build under itself if it's not already at max level or covered by a dome.
    if (!map[x][y].isDome && map[x][y].level < MAX_LEVEL) {
        map[x][y].level++;
        return true;
    }

    return false;
}


bool cronusPower(Square map[5][5], Player *player) {
    int domeCount = 0;

    // Count the number of complete towers (dome-covered buildings)
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (map[i][j].isDome && map[i][j].level == MAX_LEVEL) {
                domeCount++;
            }
        }
    }

    // Cronus's power grants a win if there are at least five complete towers on the board.
    return domeCount >= 5;
}

bool canWorkerMove(Square map[5][5], Square worker) {
    int currentX = worker.x - 1;
    int currentY = worker.y - 1;

    // Check all adjacent squares to see if a move is possible
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int newX = currentX + dx;
            int newY = currentY + dy;

            // Check if the new position is within bounds and not the current position
            if (newX >= 0 && newX < 5 && newY >= 0 && newY < 5 && !(dx == 0 && dy == 0)) {
                // Check if the square is not occupied by a dome or another worker
                if (!map[newX][newY].isDome && map[newX][newY].worker == ' ') {
                    // Check if the worker is not moving up more than one level
                    if (map[newX][newY].level <= map[currentX][currentY].level + 1) {
                        return true; // A valid move is possible
                    }
                }
            }
        }
    }
    return false; // No valid moves available
}

int main() {
    printf("                    Press Enter to START...\n");
    while (getchar() != '\n') {
        // Wait for the user to press Enter
    }

    Square gameBoard[5][5];
    Player players[2];
    int currentPlayerIndex = 0;
    bool gameRunning = true;

    initMap(gameBoard);

    // Get player names and configure players
    char player1Name[50], player2Name[50];
    char player1ColorString[50], player2ColorString[50];
    char player1Color, player2Color;

    printf("Enter name for Player 1: ");
    scanf("%49s", player1Name);
    printf("Enter color for Player 1 (Blue, White or Gray) : ");
    scanf("%49s", player1ColorString); // Note the space before %c to consume any leftover newline character

    printf("Enter name for Player 2: ");
    scanf("%49s", player2Name);
    printf("Enter color for Player 2 (Blue, White or Gray) : ");
    scanf("%49s", player2ColorString); // Note the space before %c to consume any leftover newline character

    player1Color=player1ColorString[0];
    player2Color=player2ColorString[0];

    configurePlayer(&players[0], player1Name, player1Color);
    configurePlayer(&players[1], player2Name, player2Color);

    //numarul jucatorilor
    int numPlayers=2;

    // Place workers on the board
    placeWorkers(gameBoard, players, numPlayers);

    bool zeusPowerEnabled = false; // Initialize the flag

    bool zeusPowerAsked = false; // Ask zeus power

    bool cronusPowerUsed = false; int cr=1;// Flag to track if Cronus's power has been used
    char useZeus;
    // Begin the game
    printf("Begin Game.\n");
    printGameStateWithColors(gameBoard, players, 2);


    // Main game loop
    while (gameRunning) {
    Player *currentPlayer = &players[currentPlayerIndex];
    int workerNum, newX, newY, buildX, buildY;
    bool validMove = false, validBuild=false, validChoice = false;


    // Player turn
    printf("%s's turn.\n", currentPlayer->name);


    if (!canWorkerMove(gameBoard, currentPlayer->workers[0])&&!canWorkerMove(gameBoard, currentPlayer->workers[1])) {
            // If neither worker can move, the opponent wins
        printf("Neither worker can move. %s wins!\n", players[(currentPlayerIndex + 1) % 2].name);
        gameRunning = false;
        break;
        }


    while (!validChoice) {
            printf("Choose a worker (1 or 2) to move: ");
            scanf("%d", &workerNum);
            workerNum--; // Adjust for 0-based indexing

            // Check if the chosen worker number is valid
            if (workerNum < 0 || workerNum > 1) {
                printf("Invalid worker number. Please choose worker 1 or 2.\n");
            } else {
                // Check if the chosen worker can move
                if (canWorkerMove(gameBoard, currentPlayer->workers[workerNum])) {
                    validChoice = true; // The choice is valid, proceed with the move
                    printPossibleMoves(gameBoard, currentPlayer, workerNum); // Show possible moves in green
                } else {
                    printf("Worker %d cannot move. Please choose a worker that can move.\n", workerNum + 1);
                }
            }
        }




    // Ask for move coordinates until a valid move is made
    while (!validMove) {
        printf("Enter new X Y coordinates to move worker %d (1-5 for both): ", workerNum + 1);
        scanf("%d %d", &newX, &newY);

        // Move worker
        validMove = moveWorker(gameBoard, currentPlayer, workerNum, newX, newY);
        if (!validMove) {
            printf("Invalid move. Try again.\n");
        }
    }


    printPossibleBuilds(gameBoard, currentPlayer, workerNum);


    // Ask for build coordinates until a valid build is made
    while (!validBuild) {
        printf("Enter X Y coordinates to build (1-5 for both): ");
        scanf("%d %d", &buildX, &buildY);
        validBuild = build(gameBoard, currentPlayer, workerNum, buildX, buildY);
        if (!validBuild) {
            printf("Invalid build. Try again.\n");
        }
    }

        // Ask if the current player wants to use Zeus's power at all and now
        // Here first
        if(currentPlayerIndex==0){
            if(!zeusPowerAsked){

                // Ask Zeus player if they want to use their power at all
                printf("Do you want to use Zeus's power during the game? (Y/N): ");
                char zeusChoice;
                scanf(" %c", &zeusChoice);
                if (zeusChoice == 'Y' || zeusChoice == 'y') {
                zeusPowerEnabled = true;
                }
                zeusPowerAsked=true;

            }
            if(zeusPowerEnabled){
                printf("Do you want to use Zeus's power now? (Y/N): ");
                scanf(" %c", &useZeus);
                if (useZeus == 'Y' || useZeus == 'y') {
                    if (zeusPower(gameBoard, currentPlayer, workerNum)) {
                        printf("%s has used Zeus's power!\n", currentPlayer->name);
                    } else {
                        printf("Invalid use of Zeus's power.\n");
                    }
                }
            }
        }

        if (!cronusPowerUsed && currentPlayerIndex == 1 && cr) {
            printf("Do you want to use Cronus's power? (Y/N): ");
            char useCronus;
            scanf(" %c", &useCronus);
            if (useCronus == 'Y' || useCronus == 'y') {
                    printf("Cronus's power activated!\n");
                    cronusPowerUsed = true; // Mark Cronus's power as used
                    cr=0;
            } else {cr=0;}
        }

        // Check win condition
        if (checkWinCondition(gameBoard, *currentPlayer, cronusPowerUsed)) {
            printf("%s wins!\n", currentPlayer->name);
            gameRunning = false;
            break;
        }

        // Switch to the next player
        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
        printGameStateWithColors(gameBoard, players, 2);
    }

    printf("Game over.\n");
    return 0;

}

