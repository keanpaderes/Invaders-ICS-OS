/********************************************************************************
*   .___ ___________   _________  ________  _____________________  _________    *
*   |   |\      \   \ /   /  _  \ \______ \ \_   _____/\______   \/   _____/    *
*   |   |/   |   \   Y   /  /_\  \ |    |  \ |    __)_  |       _/\_____  \     *
*   |   /    |    \     /    |    \|    `   \|        \ |    |   \/        \    *
*   |___\____|__  /\___/\____|__  /_______  /_______  / |____|_  /_______  /    *
*               \/              \/        \/        \/         \/        \/     *
*********************************************************************************
                    AN ASCII SPACE INVADERS CLONE FOR ICS-OS
*********************************************************************************
                    PORTED BY KEAN PADERES AND IAN ALONZO
********************************************************************************/

#include "../../sdk/dexsdk.h"

#define SIZEY 23
#define SIZEX 40

char world[SIZEY][SIZEX];
char player = 'A';
int playerX, playerY;
char playerLaser = '^';
char enemy = 'M';
char enemyShielded = 'O';
char enemyLaser = 'U';
char explosion = 'X';
int abrupt, score, victory, laserReady, enemyReady, totalEnemies;
int x, y, yi;

void introScene(){
    char ch;
    /*welcome screen*/
    clrscr();
    textcolor(YELLOW);
    printf("\n .___ ___________   _________  ________  _____________________  _________ ");
    printf("\n |   |\\      \\   \\ /   /  _  \\ \\______ \\ \\_   _____/\\______   \\/   _____/ ");
    printf("\n |   |/   |   \\   Y   /  /_\\  \\ |    |  \\ |    __)_  |       _/\\_____  \\  ");
    printf("\n |   /    |    \\     /    |    \\|    `   \\|        \\ |    |   \\/        \\ ");
    printf("\n |___\\____|__  /\\___/\\____|__  /_______  /_______  / |____|_  /_______  / ");
    printf("\n             \\/              \\/        \\/        \\/         \\/        \\/ ");

    textcolor(WHITE);
    printf("\n \n \n \n \t \t \t \t \t \t Press any key to start...");
    ch = getch();
    clrscr();
}

void initEnemies(){
    totalEnemies = 0;
    textcolor(LIGHTRED);
    for (x = 0; x < SIZEX; x ++) {
        for (y = 0; y < SIZEY; y ++) {
            if ((y+1) % 2 == 0 && y < 7 && x > 4
            && x < SIZEX - 5 && x % 2 ==0) {
                world[y][x] = enemy;
                totalEnemies ++;
            }
            else if ((y+1) % 2 == 0 && y >= 7 && y < 9 && x > 4
            && x < SIZEX - 5 && x % 2 ==0){
                world[y][x] = enemyShielded;
                totalEnemies = totalEnemies + 2;
            }
            else {
                world[y][x] = ' ';
            }
        }
    }
    world[SIZEY - 1][SIZEX / 2] = player;
    playerY = SIZEY - 1; playerX = SIZEX / 2;
}

void printWorld(){
    for (y = 0; y < SIZEY; y ++) {
        textcolor(YELLOW);
        printf("|");
        for (x = 0; x < SIZEX; x ++) {
            if(y == playerY && x == playerX )
                textcolor(LIGHTCYAN);
            else textcolor(LIGHTRED);
            printf("%c",world[y][x]);
        }
        textcolor(YELLOW);
        printf("|");
        printf("\n");
    }
}

int main() {

    introScene();
    char ch;
    do {
        abrupt = 0;
        score = 0;
        victory = 1;
        laserReady = 1;
        enemyReady = 0;

        srand(time());
        initEnemies();
        int i = 1;
        char direction = 'l';
        char keyPress;
        int currentEnemies = totalEnemies;

        do {
            int drop = 0;
            int enemySpeed = 1 + 30 * currentEnemies / totalEnemies;
            laserReady ++;

            /*Prints score*/
            clrscr();
            printf("     SCORE:    %d", score);
            printf("\n");

            printWorld();

            /*Computes next frame*/
            //Laser Logic
            for (x = 0; x < SIZEX; x ++) {
                for (y = SIZEY-1; y >= 0; y --) {
                    if (i%2 == 0 && world[y][x] == enemyLaser
                    && (world[y+1][x] != enemy & world[y+1][x] != enemyShielded)){
                    if((y+1) != SIZEY) world[y+1][x] = enemyLaser;
                    world[y][x] = ' ';
                    }
                    else if (i%2 == 0 && world[y][x] == enemyLaser
                    && (world[y+1][x] == enemy | world[y+1][x] == enemyShielded)){
                        world[y][x] = ' ';
                    }
                }
            }
            for (x = 0; x < SIZEX; x ++) {
                for (y = 0; y < SIZEY; y ++) {
                    if ((i % 5) == 0 && (world[y][x] == enemyShielded
                    | world[y][x] == enemy) && (rand() % 15) > 13
                    && world[y+1][x] != playerLaser) {
                        for (yi = y+1; yi < SIZEY; yi ++) {
                            if (world[yi][x] == enemy
                            | world[yi][x] == enemyShielded) {
                                enemyReady = 0;
                                break;
                            }
                            enemyReady = 1;
                        }
                        if (enemyReady) {
                            if((y+1) != SIZEY) world[y+1][x] = enemyLaser;
                        }
                    }
                    if (world[y][x] == playerLaser && world[y-1][x] == enemy) {
                        world[y][x] = ' ';
                        if((y-1) != 0) world[y-1][x] = explosion;
                        currentEnemies --;
                        score = score + 50;
                    }
                    else if (world[y][x] == playerLaser
                    && world[y-1][x] == enemyShielded) {
                        world[y][x] = ' ';
                        if((y-1) != 0) world[y-1][x] = enemy;
                        currentEnemies --;
                        score = score + 50;
                    }
                    else if (world[y][x] == playerLaser
                    && world[y-1][x] == enemyLaser) {
                        world[y][x] = ' ';
                    }
                    else if (world[y][x] == explosion) {
                        world[y][x] = ' ';
                    }
                    else if ((i+1) % 2 == 0 && world[y][x] == enemyLaser
                    && world[y+1][x] == player) {
                        if((y+1) != SIZEY) world[y+1][x] = explosion;
                        world[y][x] = ' ';
                        victory = 0;
                    }
                    else if (world[y][x] == playerLaser
                    && world[y-1][x] != enemyLaser) {
                            world[y][x] = ' ';
                            if((y-1) != 0) world[y-1][x] = playerLaser;
                    }
                }
            }

            //Enemy direction
            for (y = 0; y < SIZEY; y ++) {
                if (world[y][0] == enemy) {
                    direction = 'r';
                    drop = 1;
                    break;
                }
                if (world[y][SIZEX-1] == enemy){
                    direction = 'l';
                    drop = 1;
                    break;
                }
            }

            /*Update board for next frame*/
            if (i % enemySpeed == 0) {
                if (direction == 'l') {
                    for (x = 0; x < SIZEX - 1; x ++) {
                        for (y = 0; y < SIZEY; y ++) {
                            if (drop && (world[y-1][x+1] == enemy
                                || world[y-1][x+1] == enemyShielded)){
                                world[y][x] = world[y-1][x+1];
                                world[y-1][x+1] = ' ';
                            }
                            else if (!drop && (world[y][x+1] == enemy
                                || world[y][x+1] == enemyShielded)) {
                                world[y][x] = world[y][x+1];
                                world[y][x+1] = ' ';
                            }
                        }
                    }
                }
                else {
                    for (x = SIZEX; x > 0; x --) {
                        for (y = 0; y < SIZEY; y ++) {
                            if (drop && (world[y-1][x-1] == enemy
                                || world[y-1][x-1] == enemyShielded)) {
                                world[y][x] = world[y-1][x-1];
                                world[y-1][x-1] = ' ';
                            }
                            else if (!drop && (world[y][x-1] == enemy
                                || world[y][x-1] == enemyShielded)) {
                                world[y][x] = world[y][x-1];
                                world[y][x-1] = ' ';
                            }
                        }
                    }
                }
                for (x = 0; x < SIZEX; x ++) {
                    if (world[SIZEY - 1][x] == enemy) {
                        victory = 0;
                    }
                }
            }

            /*Controls player*/
            if(kb_ready()){
                keyPress = (char) getch();
            }
            else {
                keyPress = ' ';
            }
            if (keyPress == 'a') {
                for (x = 0; x < SIZEX; x = x+1) {
                    if ( world[SIZEY-1][x+1] == player) {
                        world[SIZEY-1][x] = player;
                        playerY = SIZEY - 1; playerX = x;
                        world[SIZEY-1][x+1] = ' ';
                    }
                }
            }

            if (keyPress == 'd') {
                for (x = SIZEX - 1; x > 0; x = x-1) {
                    if ( world[SIZEY-1][x-1] == player) {
                        world[SIZEY-1][x] = player;
                        playerY = SIZEY - 1; playerX = x;
                        world[SIZEY-1][x-1] = ' ';
                    }
                }
            }
            if (keyPress == 'm' && laserReady > 2) {
                for (x = 0; x < SIZEX; x = x+1) {
                    if ( world[SIZEY-1][x] == player) {
                        world[SIZEY - 2][x] = playerLaser;
                        laserReady = 0;
                    }
                }
            }
            if (keyPress == 'x') {
                victory = 0;
                abrupt = 1;
            }
            i++;
            delay(3);
        } while(currentEnemies > 0 && victory);

        clrscr();
        printf("     SCORE:    %d", score);
        printf("\n");
        for (y = 0; y < SIZEY; y ++) {
            textcolor(YELLOW);
            printf("|");
            for (x = 0; x < SIZEX; x ++) {
                if(y == playerY && x == playerX )
                    textcolor(LIGHTCYAN);
                else textcolor(LIGHTRED);
                printf("%c",world[y][x]);
            }
            textcolor(YELLOW);
            printf("|");
            printf("\n");
        }
        delay(5);

        /*Game Over Screen*/
        clrscr();
        textcolor(WHITE);
        if(!abrupt){
            if (victory != 0) {
                printf("\n \n \n \n               CONGRATULATIONS! \n \n \n \n \n");
                printf("\n \n               Score: %d", score);
                int bonus = totalEnemies*20 - i;
                printf("\n \n               Bonus: %d", bonus);
                printf("\n \n               Total Score: %d", score + bonus);
                printf("\n \n \n \n               Well done");
                printf(", Hero.");
                delay(10);
            }
            else {
                printf("\n \n \n \n \n \n               You have failed.");
                printf("\n \n               Final Score: %d", score);
            }
        }
        printf("\n \n               Restart? (y/n)");
        do ch=getch(); while((ch!='y')&&(ch!='n'));
    } while(ch == 'y');
    clrscr();
}
