#ifndef MINMAX_H
#define MINMAX_H

class MinMax
{
public:
    MinMax() = default;
    ~MinMax() = default;

    int findBestMove(char* board);
    int pickRandomMove(char* board);

    int checkResult(char* board);
    bool movesLeft(char* board);

private:
    int minimax_algo(char* board, int depth, bool isMax);
};

#endif // MINMAX_H
