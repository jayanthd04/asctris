#include <ncurses/ncurses.h>
//#include <ncurses.h>
#include<windows.h>
#include<iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <thread>
#include <mutex>
using namespace std; 

// game board is 10 cells wide by 16 cells height 
class Tetris{
    // game logic: 
    // get random tetrimino 
    // Place block at the top center of the grid 
    // while the tetrimino has not collided with another block 
    // to it's relative bottom, user can shift the block left, right 
    // and rotate the block.
    // atter it has collided, the block is placed at the position and orientation
    // that it has collided at. 
    // if any row is filled with blocks, the row can be cleared 
    // if row is cleared, 
    private: 
        vector<vector<char>> board{16,vector<char>(10,' ')};
        // maintain centers for each tetrim
        // everytime tetrim is rotated, shift center
        // center y ,x 
        vector<pair<vector<vector<char>>,vector<int>>> tetrim = {
            {
                {
                    {'=','=','=','='}
                },
                /*{0,2}*/
                {1,1}
            },
            {
                {
                    {'#',' ',' '},
                    {'#','#','#'}
                },
                {1,1}
            },
            {
                {
                    {' ',' ','*'},
                    {'*','*','*'}
                },
                {1,1}
            },
            {
                {
                    {'@','@'},
                    {'@','@'}
                },
                {1,1}
            },
            {
                {
                    {' ','>','>'},
                    {'>','>',' '}
                },
                {1,1}
            },
            {
                {
                    {' ','+',' '},
                    {'+','+','+'}
                },
                {1,1}
            },
            {
                {
                    {'<','<',' '},
                    {' ','<','<'}
                },
                {1,1}
            },
        };

    public: 
        vector<vector<char>> getState(){
            return board;
        }
        void printTetrim(vector<vector<char>>& tet){
            string ret = "";
            for(int i=0;i<tet.size();i++){
                /*for(int k =0;k<tetrim[i][j].size();k++){
                    ret+=tetrim[i][j][k];
                }*/
                string t = string(begin(tet[i]),end(tet[i]));
                //cout<<t<<endl;
                //printw(&tet[i][0]);
                printw(t.c_str());
                if(i!=tet.size()-1){
                    //ret+="\n";
                    printw("\n");
                    //cout<<endl;
                }
            }
            //printw(ret);
        }
        void printTetrims(){
            int i = -1; 
            int ch;
            while(ch!='q'){
                ch= getch();
                if(ch == KEY_UP) {

                    i++;
                }
                else if(ch == KEY_DOWN) i--; 
                
                if(i==tetrim.size()) i=tetrim.size()-1;
                else if(i<0) i=0;
                clear();
                printTetrim(tetrim[i].first);
                refresh();
            }
            /*for(int i=0;i<tetrim.size();i++){
                printTetrim(i);
                refresh();
                getch();
                clear();
            }*/
        }
        void rotateTetrim(pair<vector<vector<char>>, vector<int>>& orig){
            // update shift center of tetrim when rotating 
            int m = orig.first.size(); 
            int n = orig.first[0].size(); 
            // if original center is x=x, y=y new center is x=y, y=m-1-x
            int origX = orig.second[0];
            orig.second[0] = orig.second[1];
            orig.second[1] = (m-1)-origX;
            vector<vector<char>> ret(n,vector<char>(m));
            // row i in orig is column m-i in ret; 

            for(int j=0;j<n;j++){
                vector<char> curr;
                for(int i =0;i<m;i++){
                    curr.insert(curr.begin(),orig.first[i][j]);
                }
                ret[j]=curr; 
            }
            orig.first = ret; 
            //return ret; 
        }
        void rotateTetrimTest(){
            int ch; 
            int i = 0; 
            pair< vector<vector<char>>, vector<int>> tet = tetrim[i];
            while(ch != 'q'){
                if(ch == ' ') {
                    i++;
                    if(i==tetrim.size()) i=0;
                    tet = tetrim[i];
                }
                else if (ch == KEY_UP){
                    rotateTetrim(tet);
                    //tet = rot;
                }
                erase();
                refresh();
                printTetrim(tet.first);
                //printw("\n");
                //printw(&to_string(i)[0]);
                string cent = to_string(tet.second[0]) +","+to_string(tet.second[1]);
                printw("\n");
                printw(cent.c_str());
                refresh();
                ch = getch();
            }
        }
        pair<vector<vector<char>>, vector<int>> getRandomTetrim(){
            srand(static_cast<unsigned int>(time(0)));
            int i = rand() % 7;
            //cout<< i<<endl;
            return tetrim[i];
        }
        // renders game board onto Window gameWin
        void renderBoard(WINDOW*& gameWin){
            int height; 
            int width; 
            // testing rendering board; 
            /*for(int i=0;i<10;i++){
                board[15][i]='-';
            }*/
            getmaxyx(gameWin,width,height);
            for(int i=0;i<board.size();i++){
                string r = string(begin(board[i]),end(board[i]));
                mvwprintw(gameWin,i+1,1,r.c_str());
            }
            wrefresh(gameWin);
            //cout<< height<<"x"<<width<<endl;
        }

        // renders tetrimino with center at x, y onto Window gameWin based on the current board 
        void renderTetrimFromCenter(WINDOW*& gameWin, pair <vector<vector<char>>,vector<int>>& tetrim, int x, int y){
            renderBoard(gameWin);
            int m = tetrim.first.size(); 
            //int n = tetrim[0].size(); 
            int n = tetrim.first[0].size();
            // shift y by m/2 if y is equal to 1

            // start rendering tetrim starting from the left corner which 
            // is center x, y - center of tetrim 
            int cornerX = x-tetrim.second[1];
            int cornerY = y-tetrim.second[0];
            for(int i=0;i<m;i++){
                string t = string(begin(tetrim.first[i]), end(tetrim.first[i]));
                //cout<< t<<" "<<cornerY+i<<","<<cornerX<<endl;
                mvwprintw(gameWin,cornerY+i,cornerX,t.c_str());
            }
            wrefresh(gameWin);
            /*int leftCorner =  
            for(int i=0;i<m;i++){

            }*/

        }
        //clear tetrimino with center at x, y on Window gameWin 
        void clearTetrimFromCenter(WINDOW*& gameWin, pair< vector<vector<char>>, vector<int>>& tetrim, int x, int y){
            renderBoard(gameWin);
            int m = tetrim.first.size(); 

            int n = tetrim.first[0].size();

            int cornerX = x-tetrim.second[1];
            int cornerY = y-tetrim.second[0];
            vector<vector<char>> erase(m,vector<char>(n,' '));
            for(int i=0;i<m;i++){
                string t = string(begin(erase[i]), end(erase[i]));
                mvwprintw(gameWin,cornerY+i,cornerX,t.c_str());
            }
            wrefresh(gameWin);
        }
        // checks if tetrim collides with any blocks on the board vertically. 
        void renderTetrimTest(WINDOW*& gameWin){
            int ch; 
            int i=0;
            pair< vector<vector<char>>, vector<int>> tet = tetrim[i];
            while(ch != 'q'){
                if(ch == ' '){
                    i++; 
                    if(i==tetrim.size()) i=0;
                    tet = tetrim[i];
                }
                else if (ch == KEY_UP){
                    clearTetrimFromCenter(gameWin,tet,5,1+tet.second[0]);
                    rotateTetrim(tet);
                }
                renderTetrimFromCenter(gameWin,tet,5,1+tet.second[0]);

                ch = getch();
            }
        }
        // checks if tetrim collides with any blocks on the board vertically. 
        bool collisionVert(pair<vector<vector<char>>, vector<int>>& tetrim, int x, int y){
            int m = tetrim.first.size();
            int n = tetrim.first[0].size(); 

            int cornerX = x-tetrim.second[1]-1;
            int cornerY = y-tetrim.second[0]-1; 
            // iterate through each cell in the last row 
            // if cell is not blank, check if block below curr cell on board 
            // is filled or if the cell's position is equal to height 
            // of board if so return true; 
            // if cell is blank, go a cell up till we encounter a non empty 
            // cell and check if block below is filled if so return true; 
            
            for(int j=0;j<n;j++){
                if(tetrim.first[m-1][j]!=' '){
                    //cout<< tetrim.first[m-1][j];
                    // should be placed at board[cornerY+n-1][cornerX+i]
                    if(cornerY+m-1 == board.size()-1|| board[cornerY+m][cornerX+j]!=' '){
                        return true;
                    }
                }
                else{
                    int i= m-1; 
                    while(tetrim.first[i][j]==' '){
                        i--;
                    }
                    if(board[cornerY+i+1][cornerX+j]!=' ')return true;
                }
            }
            return false; 
        }
        // To-do: add horizontal collision detection 
        bool collisionLeft(pair<vector<vector<char>>, vector<int>>& tetrim, int x, int y){
            int m = tetrim.first.size(); 
            int n = tetrim.first[0].size(); 

            int cornerX = x-tetrim.second[1]-1; 
            int cornerY = y-tetrim.second[0]-1; 

            // iterate through each cell in the first col 
            // if cell is not blank, check if block to the left of curr 
            // on board is filled or if the x coord of the 
            // cell is equal to 0, if so return true  
            // if cell is blank, go a cell to the right till we reach 
            // a non-empty cell and check if the block to the left 
            // is filled if so return true; 

            for(int i=0;i<m;i++){
                if(tetrim.first[i][0]!=' '){
                    // should be placed at board[cornerY+i][cornerX]
                    if(cornerX == 0 || board[cornerY+i][cornerX-1]!=' ')
                        return true; 
                }
                else{
                    int j = 0;
                    while(tetrim.first[i][j]==' '){
                        j++; 
                    }
                    if(board[cornerY+i][cornerX+j-1]!=' ')return true; 
                }
            }
            return false; 
        }
        bool collisionRight(pair<vector<vector<char>>, vector<int>>& tetrim, int x, int y){
            int m = tetrim.first.size(); 
            int n = tetrim.first[0].size(); 

            int cornerX = x - tetrim.second[1] - 1;
            int cornerY = y - tetrim.second[0] -1; 

            for(int i=0;i<m;i++){
                if(tetrim.first[i][n-1]!=' '){
                    // should be placed at board[cornerY+i][cornerX+n-1];
                    if(cornerX+n-1 == board[0].size()-1 || board[cornerY+i][cornerX+n]!=' ')
                        return true; 
                }
                else{
                    int j =0;
                    while(tetrim.first[i][j]==' '){
                        j--; 
                    }
                    if(board[cornerY+i][cornerX+j+1]!=' ')return true; 
                }
            }
            return false; 
        }
        // adds tetrimino to board with centered at x, y on the board
        void addTetrimToBoard(pair<vector<vector<char>>, vector<int>>& tetrim,int x,int y){

            int m = tetrim.first.size(); 
            int n = tetrim.first[0].size(); 

            int cornerX = x-tetrim.second[1]-1;
            int cornerY = y-tetrim.second[0]-1;

            //cout<<"["<<cornerY<<"]"<<"["<<cornerX<<"]"<<" ";
            // if board is empty just add tetrim[i][j] to board at board[cornerY+i][cornerX+j]
            // else 
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    if(board[cornerY+i][cornerX+j]==' '){
                        //cout<<"["<<cornerY+i<<"]"<<"["<<cornerX+j<<"]"<<tetrim.first[i][j]<<endl;
                        board[cornerY+i][cornerX+j] = tetrim.first[i][j];
                    }
                }
            }
        }

};
int main(){
    initscr();
    keypad(stdscr, TRUE);    
    cbreak();
    curs_set(0);
    noecho();
    WINDOW *gameWin = newwin(17,12,10,10);
    refresh();
    nodelay(gameWin,true);
    keypad(gameWin,TRUE);
    wborder(gameWin, 0, 0, 0, ' ', 0, 0, 0, 0);
    // render block at x,y+tetrim.second[0]
    //int y=1;
    //int x=1;
    // keep track of tetrim pos based on their center. 
    // expand and render tetrim onto board around center   
    // and check for collisions on the way down. 
    // only have to append block to board if curr block has collided with another block 
    // or the floor 
    // game ends when new block spawn point is blocked by existing blocks on 
    // board;
    
    Tetris tetris;
    int i=0;
    queue<int> acts;
    bool gameOver = false;
    bool input = false; 
    auto func = [&gameWin,&acts,&gameOver](){
        while(!gameOver){
            int key = wgetch(gameWin);
            acts.push(key);
        }
    };
    // std::thread t1(func);
    int x=5; 
    int y=1; 
    pair<vector<vector<char>>, vector<int>> tet = tetris.getRandomTetrim();
    // need to make game more responsive 
    while(i<10){
        int key = wgetch(gameWin);
        // alt path where we perform action first if possible 
        /*if(key==KEY_UP){
            // need to check if rotation collides horizontally as well 
            pair<vector<vector<char>>, vector<int>> next = tet; 
            tetris.rotateTetrim(next);
            if(!tetris.collisionVert(next,x,y+next.second[0]))
                tet = next; 
        }*/
        if(tetris.collisionVert(tet,x,y+tet.second[0])){
            tetris.addTetrimToBoard(tet,x,y+tet.second[0]);
            i++; 
            x=5;
            y=1;
            // update gameOver bool 
            // clear lines
            tet = tetris.getRandomTetrim(); 
        }
        // performing action just before rendering 
        else{
            // perform only one action per frame  
            //int key = wgetch(gameWin);
            if(key==KEY_UP){
                pair<vector<vector<char>>, vector<int>> next = tet; 
                tetris.rotateTetrim(next);
                if(!tetris.collisionVert(next,x,y+next.second[0])&&
                        !tetris.collisionLeft(next,x,y+next.second[0])&&
                        !tetris.collisionRight(next,x,y+next.second[0]))
                    tet = next; 
                //tetris.rotateTetrim(tet);
            }
            if(key==KEY_LEFT){
                if(!tetris.collisionLeft(tet,x,y+tet.second[0]))
                    x--;
            }
            if(key==KEY_RIGHT){
                if(!tetris.collisionRight(tet,x,y+tet.second[0]))
                    x++; 
            }
        }
        tetris.renderTetrimFromCenter(gameWin,tet,x,y+tet.second[0]);
        Sleep(900);
        tetris.clearTetrimFromCenter(gameWin,tet,x,y+tet.second[0]); 
        y++;
    }
    /*while(i<5){
        x=5;
        y=1;
        // have two cases 
        // if tet collides vertically: 
        //  * add tet to game state 
        //  * clear lines and update game state as needed 
        //  * get new tetrim, reset x and y to 5 and 1 respectively 
        // else 
        //  * get user input 
        //  * perform action on tet if possible  
        //  * increment y by 1 
        // regardless of collision we need to render tetrim 
        pair<vector<vector<char>>,vector<int>> tet = tetris.getRandomTetrim();
        while(!tetris.collisionVert(tet,x,y+tet.second[0])){
            // perform actions in acts queue
            // get rid of 
            tetris.renderTetrimFromCenter(gameWin,tet,x,y+tet.second[0]);
            Sleep(900);
            tetris.clearTetrimFromCenter(gameWin,tet,x,y+tet.second[0]);
            y++;
            while(!acts.empty()){
                int ac = acts.front();
                acts.pop();
                if(ac==KEY_UP){
                    tetris.rotateTetrim(tet);
                    // alt impl where we check if rotation is possible 
                    //pair<vector<vector<char>>,vector<int>> rot =tet; 
                    // tetris.rotateTetrim(rot);
                    // if(!tetris.collisionVert(rot,x,y+rot.second[0])){
                    // tet =rot;
                    // }
                }
            }

        }
        // take one more action if it is possible 

        // Create an overlap function that checks if the tetrim at center 
        // x, y+tet.second[0] overlaps with blocks on board 
        // if so decrement y till block doesn't overlap with board 
        // fix bug where just as block is about 
        tetris.addTetrimToBoard(tet,x,y+tet.second[0]);
        i++;
        acts = queue<int>();
    }*/
    tetris.renderBoard(gameWin);
    wrefresh(gameWin);
    gameOver=true;
    //testing keys 
    /*int ch = getch();
    cout<< keyname(ch)<<" "<<ch<<endl;*/
    
    //tetris.printTetrims();
    
    //tetris.rotateTetrimTest();
    //tetris.renderBoard(gameWin);
    //getch();
    getch();
    //t1.join();
    endwin();
}
