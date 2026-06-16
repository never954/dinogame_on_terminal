#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <ctime>
#include <signal.h>

using namespace std;

void initTermios() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void resetTermios() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Signal handler to restore terminal settings if the user presses Ctrl+C
void handleSignal(int sig) {
    resetTermios();
    exit(sig);
}

bool kbhit() {
    int old_flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);
    int c = getchar();
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
    if (c != EOF) {
        ungetc(c, stdin);
        return true;
    }
    return false;
}

char getch() {
    return getchar();
}

int main() {
    // Register signal handlers
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    srand(time(0));
    initTermios();
    
    bool jumping = false;
    int jumpTime = 0;
    const int charpo = 5;      // Player's horizontal position
    const int screenWidth = 80; // Standardized screen width to prevent wrapping

    // Initialize obstacle positions to start off-screen
    int position = 80;         // Ground obstacle 1
    int position1 = 110;       // Ground obstacle 2
    int position2 = 135;       // Airborne obstacle (bird)
    
    bool game = true;
    int gamescore = 0;
    
    cout << "Press SPACE to jump, 'q' to quit\n";
    this_thread::sleep_for(chrono::milliseconds(1000));
    
    while (game) {
        if (kbhit()) {
            char ch = getch();
            if (ch == ' ' && !jumping) {  
                jumping = true;
                jumpTime = 8; 
            }
            else if (ch == 'q') {
                break;  
            } 
        }
        
        if (jumping) {
            jumpTime--;
            if (jumpTime <= 0) {
                jumping = false;
            }
        }
        
        // Move obstacles left
        position--;
        if (position <= 0) {
            position = 85;
        }
        
        position1--;
        if (position1 <= 0) {
            position1 = 110;
        }
        
        position2--;
        if (position2 <= 0) {
            position2 = 95;
        }

        // Collision detection
        if (position == charpo && !jumping) {
            game = false;
        }
        if (position1 == charpo && !jumping) {
            game = false;
        }
        if (position2 == charpo && jumping) {
            game = false;
        }
        
        // Clear screen and render
        system("clear");
        
        // 1. Render Air Row (contains player if jumping, and airborne obstacles)
        for (int i = 0; i < screenWidth; i++) {
            if (i == charpo && jumping) {
                cout << "@";
            }
            else if (i == position2) {
                cout << "7";
            }
            else {
                cout << " ";
            }
        }
        cout << "\n";
        
        // 2. Render Ground Row (contains player if on ground, and ground obstacles)
        for (int i = 0; i < screenWidth; i++) {
            if (i == charpo && !jumping) {
                cout << "@";
            }
            else if (i == position) {
                cout << "#";
            }
            else if (i == position1) {
                cout << "#";
            }
            else {
                cout << " ";
            }
        }
        cout << "\n";
      
        // 3. Render Ground Separator
        for (int i = 0; i < screenWidth; i++) {
            cout << "=";
        }
        cout << "\n";

        gamescore++;
        
        if (!game) {
            cout << "GAME OVER" << endl;
            cout << "SCORE: " << gamescore << endl;
        }
        
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    
    resetTermios();
    return 0;
}