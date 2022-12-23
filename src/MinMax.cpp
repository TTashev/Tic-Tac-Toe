#include "MinMax.h"

#include <algorithm>
#include <limits>

static const char player = 'x';
static const char opponent = 'o';

int MinMax::checkResult(char* board)
{
    /* check the horizontals */
    if(board[0] == board[1] && board[1] == board[2])
    {
        if(board[2] == 'x')
        {
            return 10;
        }
        else if(board[2] == 'o')
        {
            return -10;
        }

    }
    if(board[3] == board[4] && board[4] == board[5])
    {
        if(board[5] == 'x')
        {
            return 10;
        }
        else if(board[5] == 'o')
        {
            return -10;
        }

    }
    if(board[6] == board[7] && board[7] == board[8])
    {
        if(board[8] == 'x')
        {
            return 10;
        }
        else if(board[8] == 'o')
        {
            return -10;
        }
    }

    /* check the vertical */
    if(board[0] == board[3] && board[3] == board[6])
    {
        if(board[6] == 'x')
        {
            return 10;
        }
        else if(board[6] == 'o')
        {
            return -10;
        }
    }
    if(board[1] == board[4] && board[4] == board[7])
    {
        if(board[7] == 'x')
        {
            return 10;
        }
        else if(board[7] == 'o')
        {
            return -10;
        }
    }
    if(board[2] == board[5] && board[5] == board[8])
    {
        if(board[8] == 'x')
        {
            return 10;
        }
        else if(board[8] == 'o')
        {
            return -10;
        }
    }

    /* check the diagonals */
    if(board[0] == board[4] && board[4] == board[8])
    {
        if(board[8] == 'x')
        {
            return 10;
        }
        else if(board[8] == 'o')
        {
            return -10;
        }

    }
    if(board[2] == board[4] && board[4] == board[6])
    {
        if(board[6] == 'x')
        {
            return 10;
        }
        else if(board[6] == 'o')
        {
            return -10;
        }
    }

    return 0;
}

bool MinMax::movesLeft(char* board)
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == '-')
        {
            return true;
        }
    }
    return false;
}

int MinMax::minimax_algo(char* board, int depth, bool isMax)
{
    int score = checkResult(board);

    if (score == 10)
    {
        return score;
    }

    if (score == -10)
    {
        return score;
    }

    // If there are no more moves and no winner then
    // it is a tie
    if (movesLeft(board)==false)
    {
        return 0;
    }

    // If this maximizer's move
    if (isMax)
    {
        int best = INT_MIN;

        // Traverse all cells
        for (int i = 0; i < 9; i++)
        {
            // Check if cell is empty
            if (board[i]=='-')
            {
                // Make the move
                board[i] = player;

                // Call minimax recursively and choose
                // the maximum value
                best = std::max(best, minimax_algo(board, depth+1, false));

                // Undo the move
                board[i] = '-';
            }
        }
        return best - depth;
    }

    // If this minimizer's move
    else
    {
        int best = INT_MAX;

        // Traverse all cells
        for (int i = 0; i < 9; i++)
        {
            // Check if cell is empty
            if (board[i]=='-')
            {
                // Make the move
                board[i] = opponent;

                // Call minimax recursively and choose
                // the minimum value
                best = std::min(best, minimax_algo(board, depth+1, true));

                // Undo the move
                board[i] = '-';
            }
        }
        return best + depth;
    }
}

int MinMax::findBestMove(char* board)
{
    int bestVal = INT_MAX;
    int bestMove = -1;

    // Traverse all cells, evaluate minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (int i = 0; i < 9; i++)
    {
        // Check if cell is empty
        if (board[i]=='-')
        {
            // Make the move
            board[i] = opponent;

            // compute evaluation function for this move
            int moveVal = minimax_algo(board, 0, true);

            // Undo the move
            board[i] = '-';

            // If the value of the current move is
            // less than the best value, then update
            // best
            if (moveVal < bestVal)
            {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }

    return bestMove;
}

int MinMax::pickRandomMove(char* board)
{
    if (movesLeft(board)==false)
    {
        return -1;
    }

    int index = rand() % 9;

    if(board[index] != '-')
    {
        while(board[index] != '-')
        {
            index = rand() % 9;
            if(board[index] == '-')
            {
                break;
            }
        }
    }

    return index;
}
