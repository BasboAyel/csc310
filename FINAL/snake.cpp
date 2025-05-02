#include <iostream>
#include <unistd.h> // for usleep
#include <termios.h> // for non-blocking input
#include <fcntl.h>
#include <cstdlib>
#include <ctime>
#include "heap.h"
#include "hashtable.h"
#include "avltree.h"
#include "splaytree.h"

using namespace std;

const int SIZE = 15;
int dir = 0;
int score = 0;
int fruitX, fruitY;
bool gameOver = false;

struct Node {
    int x, y;
    Node* next;
    Node(int x, int y) : x(x), y(y), next(nullptr) {}
};

Node* Snake = nullptr;
HashTable snakeTable;
MinHeap fruitHeap(50);
AVLTree fruitScoreTree;
SplayTree snakeHeadTracker;

char nonBlockingGetChar();
void clearScreen();
void initializeBoard(int board[SIZE][SIZE]);
void drawBoard(int board[SIZE][SIZE]);
void updateBoard(Node* snake, int board[SIZE][SIZE]);
void moveSnake(Node*& snake, int board[SIZE][SIZE]);
void generateFruits(Node* snake, int board[SIZE][SIZE]);
void makeSnake(Node*& snake, int x, int y, int length);

int main() {
    srand(time(0));
    int board[SIZE][SIZE];

    makeSnake(Snake, 7, 7, 3); // Start snake in center
    initializeBoard(board);
    fruitX = 5;
    fruitY = 5;
    snakeHeadTracker.insert(7, 7);

    clearScreen();
    updateBoard(Snake, board);
    drawBoard(board);

    while (!gameOver) {
        clearScreen();
        cout << "Controls: W = Up, S = Down, A = Left, D = Right\n\n";

        updateBoard(Snake, board);
        drawBoard(board);

        char input = nonBlockingGetChar();
        if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
            if (input == 'w') dir = 4;     // Up
            else if (input == 's') dir = 2; // Down
            else if (input == 'a') dir = 1; // Left
            else if (input == 'd') dir = 3; // Right
        }

        if (dir != 0) {
            moveSnake(Snake, board);
        }
        usleep(300000); // Control game speed
    }

    cout << "\nFinal Fruits Eaten (Ordered by Time):\n";
    fruitScoreTree.display();

    return 0;
}


void makeSnake(Node*& snake, int x, int y, int length) {
    for (int i = 0; i < length; i++) {
        Node* newNode = new Node(x, y + i);
        newNode->next = snake;
        snake = newNode;
        snakeTable.insert(x, y + i);
    }
}

void initializeBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            board[i][j] = 0;
}

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void updateBoard(Node* snake, int board[SIZE][SIZE]) {
    initializeBoard(board);

    // place fruit
    board[fruitX][fruitY] = 1;

    Node* ptr = snake;
    if (ptr) {
        board[ptr->x][ptr->y] = 3; // head
        ptr = ptr->next;
    }
    while (ptr) {
        board[ptr->x][ptr->y] = 2; // body
        ptr = ptr->next;
    }
}

void drawBoard(int board[SIZE][SIZE]) {
    cout << "                  Snake Game\n";
    cout << "                   Score: " << score << "\n";
    cout << " ";
    for (int i = 0; i < SIZE; ++i)
        cout << "___";
    cout << "\n";

    for (int i = 0; i < SIZE; ++i) {
        cout << "|";
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0)
                cout << " • ";
            else if (board[i][j] == 1)
                cout << " X ";
            else if (board[i][j] == 2)
                cout << " S ";
            else if (board[i][j] == 3)
                cout << " H ";
        }
        cout << "|\n";
    }

    cout << " ";
    for (int i = 0; i < SIZE; ++i)
        cout << "___";
    cout << "\n";
}

char nonBlockingGetChar() {
    char input = 0;
    int flags;
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    input = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, flags);

    return input;
}

void moveSnake(Node*& snake, int board[SIZE][SIZE]) {
    if (!snake) return;
    int newX = snake->x;
    int newY = snake->y;

    if (dir == 1) --newY;
    else if (dir == 2) ++newX;
    else if (dir == 3) ++newY;
    else if (dir == 4) --newX;

    // Check for wall collision
    if (newX < 0 || newX >= SIZE || newY < 0 || newY >= SIZE) {
        cout << "GAME OVER! You hit the wall.\n";
        gameOver = true;
        return;
    }

    // Check for self collision
    if (snakeTable.search(newX, newY)) {
        cout << "GAME OVER! You hit yourself.\n";
        gameOver = true;
        return;
    }

    // Move snake
    Node* newHead = new Node(newX, newY);
    newHead->next = snake;
    snake = newHead;
    snakeTable.insert(newX, newY);
    snakeHeadTracker.insert(newX, newY);

    // Check if fruit eaten
    if (newX == fruitX && newY == fruitY) {
        score++;
        int timeCollected = static_cast<int>(time(nullptr));
        fruitScoreTree.insert(timeCollected, score);
        generateFruits(snake, board);
    } else {
        // Remove tail
        Node* temp = snake;
        while (temp->next && temp->next->next)
            temp = temp->next;
        snakeTable.remove(temp->next->x, temp->next->y);
        delete temp->next;
        temp->next = nullptr;
    }
}

void generateFruits(Node* snake, int board[SIZE][SIZE]) {
    int tries = 0;
    while (tries < 100) {
        int randX = rand() % SIZE;
        int randY = rand() % SIZE;
        if (board[randX][randY] == 0) {
            fruitX = randX;
            fruitY = randY;
            return;
        }
        tries++;
    }
}
