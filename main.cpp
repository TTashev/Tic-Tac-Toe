#include <iostream>
#include <map>
#include <functional>
#include <thread>
#include <mutex>

#include "include/MinMax.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>

char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
    {
        perror("tcsetattr()");
    }
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
    {
        perror("tcsetattr ICANON");
    }
    if (read(0, &buf, 1) < 0)
    {
        perror ("read()");
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
    {
        perror ("tcsetattr ~ICANON");
    }
    return (buf);
}
#endif

void drawChooseDifficulty();

enum GAME_STATE
{
    CHOOSE_DIFFICULTY = 0,
    PLAY,
};

enum PLAYERS_TURN_STATE
{
    PLAYER_TURN = 0,
    AI_TURN,
};

PLAYERS_TURN_STATE turn_state = PLAYERS_TURN_STATE::PLAYER_TURN;
GAME_STATE game_state = GAME_STATE::CHOOSE_DIFFICULTY;

uint32_t player_score, ai_score;

char game[] = {'-','-','-',
               '-','-','-',
               '-','-','-'};

std::map<char, int> playerMoves = {{'q', 0}, {'w', 1}, {'e', 2},
                                   {'a', 3}, {'s', 4}, {'d', 5},
                                   {'z', 6}, {'x', 7}, {'c', 8},};

MinMax minMax;

std::function<void()> drawGamePtr = drawChooseDifficulty;
std::function<int(MinMax, char* board)> aiPlayTurn;

std::string levelClear =
"###############################\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"###############################\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"###############################\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"###############################\n";

std::string level =
"###############################\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"###############################\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"###############################\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"#         #         #         #\n"
"###############################\n";

void clearScreen()
{
#ifdef _WIN32
	HANDLE hOut;
	// set x and y to 0 (upper left corner)
	COORD Position = {0, 0};

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(hOut, Position);
#else
	std::cout << "\033[H\033[2J\033[3J";
#endif
}

void drawChooseDifficulty()
{
    std::cout << "CHOOSE DIFFICULTY:\n";
    std::cout << "EASY - F  HARD - J\n";
}

void drawGame()
{
    std::cout << "  PLAYER - "<<player_score<<"   :   COMPUTER - "<<ai_score<<"\n";
    std::cout << "\n";

    for(int i = 0; i < 9; i++)
    {
        if(game[i] == 'x')
        {
            if(i >= 0 && i <= 2)
            {
                level.replace(32 + (i * 10), 10, "#X       X");
                level.replace(64 + (i * 10), 10, "#  X   X  ");
                level.replace(96 + (i * 10), 10, "#    X    ");
                level.replace(128 + (i * 10),10, "#  X   X  ");
                level.replace(160 + (i * 10),10, "#X       X");
            }
            else if(i >= 3 && i <= 5)
            {
                level.replace(224 + ((i - 3) * 10), 10, "#X       X");
                level.replace(256 + ((i - 3) * 10), 10, "#  X   X  ");
                level.replace(288 + ((i - 3) * 10), 10, "#    X    ");
                level.replace(320 + ((i - 3) * 10), 10, "#  X   X  ");
                level.replace(352 + ((i - 3) * 10), 10, "#X       X");
            }
            else if(i >= 6 && i <= 8)
            {
                level.replace(416 + ((i - 6) * 10), 10, "#X       X");
                level.replace(448 + ((i - 6) * 10), 10, "#  X   X  ");
                level.replace(480 + ((i - 6) * 10), 10, "#    X    ");
                level.replace(512 + ((i - 6) * 10), 10, "#  X   X  ");
                level.replace(544 + ((i - 6) * 10), 10, "#X       X");
            }
        }
        else if(game[i] == 'o')
        {
            if(i >= 0 && i <= 2)
            {
                level.replace(32 + (i * 10), 10, "#  O O O  ");
                level.replace(64 + (i * 10), 10, "# O     O ");
                level.replace(96 + (i * 10), 10, "#O       O");
                level.replace(128 + (i * 10),10, "# O     O ");
                level.replace(160 + (i * 10),10, "#  O O O  ");
            }
            else if(i >= 3 && i <= 5)
            {
                level.replace(224 + ((i - 3) * 10), 10, "#  O O O  ");
                level.replace(256 + ((i - 3) * 10), 10, "# O     O ");
                level.replace(288 + ((i - 3) * 10), 10, "#O       O");
                level.replace(320 + ((i - 3) * 10), 10, "# O     O ");
                level.replace(352 + ((i - 3) * 10), 10, "#  O O O  ");
            }
            else if(i >= 6 && i <= 8)
            {
                level.replace(416 + ((i - 6) * 10), 10, "#  O O O  ");
                level.replace(448 + ((i - 6) * 10), 10, "# O     O ");
                level.replace(480 + ((i - 6) * 10), 10, "#O       O");
                level.replace(512 + ((i - 6) * 10), 10, "# O     O ");
                level.replace(544 + ((i - 6) * 10), 10, "#  O O O  ");
            }
        }
    }

    std::cout << level << "\n";
}

void processPlayerInput()
{
    char pos;
    std::mutex mtx;

    while(1)
    {
        pos = getch();

        if(game_state == GAME_STATE::CHOOSE_DIFFICULTY)
        {
            if(pos == 'f' || pos == 'j')
            {
                drawGamePtr = &drawGame;
                system("cls");

                if(pos == 'f')
                {
                    // easy difficulty
                    aiPlayTurn = &MinMax::pickRandomMove;
                }
                else if(pos == 'j')
                {
                    // hard difficulty
                    aiPlayTurn = &MinMax::findBestMove;
                }
            }
        }

        if(playerMoves.find(pos) != playerMoves.end())
        {
            if(game[playerMoves[pos]] == '-' && turn_state == PLAYERS_TURN_STATE::PLAYER_TURN)
            {
                mtx.lock();
                game[playerMoves[pos]] = 'x';

                turn_state = PLAYERS_TURN_STATE::AI_TURN;
                mtx.unlock();
            }
        }
    }
}

void clearGame()
{
    std::this_thread::sleep_for (std::chrono::seconds(1));
    std::fill_n(game, 9, '-');
    level = levelClear;
}

void checkForEndGameCondition()
{
    // check for gameOver condition
    if(minMax.checkResult(game) == 10  ||
       minMax.checkResult(game) == -10 ||
       !minMax.movesLeft(game))
    {
        if(minMax.checkResult(game) == 10)
        {
            player_score++;
        }
        else if(minMax.checkResult(game) == -10)
        {
            ai_score++;
        }

        clearGame();
    }
}

int main()
{
    std::thread inputThread (processPlayerInput);
    std::mutex mtx;

    while(1)
    {
        std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();

        if(turn_state == PLAYERS_TURN_STATE::AI_TURN)
        {
            mtx.lock();
            int res = std::invoke(aiPlayTurn, minMax, game);
            turn_state = PLAYERS_TURN_STATE::PLAYER_TURN;
            if(res >= 0 && res < 9)
            {
                game[res] = 'o';
            }
            mtx.unlock();
        }

        drawGamePtr();

        checkForEndGameCondition();

        clearScreen();

        t += std::chrono::milliseconds(33);
        std::this_thread::sleep_until(t);
    }

    inputThread.join();

    return 0;
}
