#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <ctime>

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
    srand(time(0));
    initTermios();
    
    bool jumping = false;
    int jumpTime = 0;
    int charpo = 5;

    int position =  100 ; 
    int position1 =position + 10 ; 
    int position2 = position + 20 ;    
    bool game = true ;
    int gamescore = 0 ;
    
    cout << "Press SPACE to jump, 'q' to quit\n";
    this_thread::sleep_for(chrono::milliseconds(1000)) ;
    
    while (game) {
        
        if (kbhit()) {
            char ch = getch() ;
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
        
        
        position--;
        if (position==0) position = 113;  
        position1--;
        if (position1==0) position1 = 103; 
         position2--;
        if (position2==0) position2 = 73;  

        if(position == charpo && !jumping){
            game=false;
        }if(position1==charpo && !jumping){
            game=false;
        }
        if(position2==charpo && jumping){
            game=false;
        }
        
        
       
        system("clear");
        
        
         
        for (int i = 0; i <= 95; i++) {
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
        
        for (int i = 0; i <= 103; i++) {
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
      
        cout<<"\n";
        
    cout <<"========================================================================================================\n";
        gamescore++;
        if(!game ){
            cout<<"GAME OVER"<<endl;
            cout<<"SCORE:"<<gamescore;
        }
        
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    
    resetTermios();
    return 0;
} 
  