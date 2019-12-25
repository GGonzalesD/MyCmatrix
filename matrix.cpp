#include<ncurses.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

using namespace std;

int mxx, mxy;

char KEY = '\0';
void load_key(){
    nodelay(stdscr, true);
    move(0,0);
    KEY = getch();
}

void gotoxy(int x, int y){
    move(y,x);
}

// ------------------------------------------
class Ast{
private:
	float x,y;
	int l=rand()%10 + 20;
	bool k = false;
	float v = 0.5 + ((rand()%200)/100);
public:
	Ast(float, float);
	void draw();
	void loop();
	bool isdead();
};

Ast::Ast(float x_, float y_):x(x_), y(y_){};

bool Ast::isdead(){return k;};

void Ast::draw(){
	k = true;
	for(int i=0; i<l; i++){
		if(y-i>=0 && (y-i)<mxy){
			move(y-i,x);
			if(i==0){
				int c = rand()%3 + 1;
				attron(COLOR_PAIR(c));
				printw("|");
				attroff(COLOR_PAIR(c));
			}else{
			int c = rand()%2 + 1;
			attron(COLOR_PAIR(c));
			switch(rand()%3){
			case 0:
				printw("%c",rand()%('z'-'a') + 'a');
				break;
			case 1:
				printw("%c",rand()%10 + '0');
				break;
			case 2:
				printw("%c",rand()%('Z'-'A') + 'A');
				break;
			}
			attroff(COLOR_PAIR(c));
			}
			k = false;
		}
	}
}

void Ast::loop(){
	for(int i=0; i<l; i++){
		if(y-i>=0 && (y-i)<mxy){
			move(y-i,x);
			printw(" ");
		}
	}

	y += v;
}

// ------------------------------------------

int main(int argc, char ** argv){
    srand(time(NULL));
    initscr();
    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_BLACK,COLOR_GREEN);
    init_pair(3,COLOR_WHITE,COLOR_BLACK);

    curs_set(0);
    noecho();

    int vol = 100;
    int t = 0;
    float tx = 0.05;

    int LEN = 60;
    Ast *A_[LEN];
    for(int i=0; i<LEN; i++){
	A_[i] = NULL;
    }

    while(1){
	getmaxyx(stdscr, mxy, mxx);
        load_key();

	t++;

	if(t >= 33*tx){
		t = 0;
		for(int i=0; i<LEN; i++){
			if(A_[i]==NULL){
				A_[i] = new Ast(rand()%mxx, 1);
				break;
			}
		}
	}
	for(int i=0; i<LEN; i++){
		if(A_[i]!=NULL){
			A_[i]->loop();
			A_[i]->draw();
			if(A_[i]->isdead()){
				delete A_[i];
				A_[i] = NULL;
			}
		}
	}


        usleep(30000);
        //clear();
    }

    getch();

    endwin();
    // eliminar memoria usada por ncurses

    return 0;
}
