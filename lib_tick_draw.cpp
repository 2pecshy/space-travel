#include <pspkernel.h>
#include <oslib/oslib.h>
#include "lib_tick_draw.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init OSLib:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int initOSLib(){
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
    oslInitAudio();
    oslSetQuitOnLoadFailure(1);
    oslSetKeyAutorepeatInit(1);
    oslSetKeyAutorepeatInterval(1);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

player iniplayer(){
    int i = 0;
    player p;
    p.nb_vie = 100;
    p.score = 0;
    p.x = 240;
    p.y = 220;
    p.v = 5;
    p.power = 100;
    for(i = 0; i < 20; i++){
        p.poslaser[i].y = -50;
    }
    return p;
}

void add_laser(int x,int y,part *tab){
    int i;
    for(i = 0; i < 20; i++){
        if(tab[i].y <= -20){
            tab[i].x = x + 20;
            tab[i].y = y;
            tab[i].v = -2;
            break;
        }
    }
}

void inigame(part *tab, int taille){
    int i = 0;
    for(i = 0; i < taille; i++){
        tab[i].x = oslRandf( 0, 480);
        tab[i].y = 0;
        tab[i].v = oslRandf( 2, 5);
    }
}

int next_tick(part *tab, part *tab2, int taille){
    int i = 0;int score = 0;
    for(i = 0; i < taille; i++){
        if(tab[i].y > 272){
            tab[i].y = 0;
            tab[i].x = oslRandf(0,480);
            tab[i].v = oslRandf(2,5);
            score++;
        }
        else{
            tab[i].y = tab[i].y + tab[i].v;
        }
    }
    for(i = 0; i < 20; i++){
        if(tab2[i].y > -20){
            tab2[i].y = tab2[i].y + tab2[i].v;
        }
    }
    return score;
}

void drawBar(int x, int y, int longeur, int largeur,int val, OSL_COLOR color){
    oslDrawRect(x,y,x + longeur,y + largeur, color);
    oslDrawFillRect( x,y,x + val,y + largeur,color);
}

int colision(player p, part *tab, part *tab2, int taille){
    int i = 0; int j = 0;
    int coli = 0;
    for(i = 0; i < taille; i++){
        if((tab[i].x > (p.x - 5)) and ((p.x + 43) > tab[i].x) and (tab[i].y > (p.y - 5)) and ((p.y + 43) > tab[i].y)){
            coli = 1;
        }
        for(j = 0; j < 20; j++){
            if(tab2[j].y > -20){
                if((tab[i].x > (tab2[j].x - 5)) and ((tab2[j].x + 20) > tab[i].x) and (tab[i].y > (tab2[j].y - 5)) and ((tab2[j].y + 20) > tab[i].y)){
                    tab[i].y = 400;
                    tab2[j].y = -50;
                }
            }
        }
    }
    return coli;
}

int run_game(OSL_IMAGE *bg, OSL_IMAGE *shipimg, OSL_IMAGE *bkg, OSL_IMAGE *pa, OSL_IMAGE *se){
    player p;
    int pause = 0;
    p = iniplayer();int skip = 0;
    int taille = 10; int i;
    part world[taille];
    inigame( world, taille);
    while(p.nb_vie > 0){

        if (!skip){
            oslStartDrawing();//--------------------------------------start drawing
            oslClearScreen(RGB(0,0,0));
            oslDrawImageXY(bg, 0, 0);
            oslDrawImageXY(shipimg, p.x, p.y);
            for(i = 0; i < 20; i++){
                if(i < taille){
                    oslDrawImageXY(bkg, world[i].x, world[i].y);
                }
                if(p.poslaser[i].y > -20){
                    oslDrawImageXY(se, p.poslaser[i].x, p.poslaser[i].y);
                }
            }
            oslDrawStringf(0, 0, "speed = %d  score = %d", p.v, p.score);
            drawBar(350 ,5 ,100 ,10 ,p.nb_vie ,RGB(255,0,0));
            drawBar(350, 20, 100 , 10, p.power, RGB(0,255,0));
            if(pause){oslDrawImageXY(pa, 200, 136);}
            oslEndDrawing();//----------------------------------------end drawing
        }
        oslEndFrame();
        skip = oslSyncFrame();
        oslReadKeys();
        if(!pause){
            p.nb_vie = p.nb_vie - colision( p, world , p.poslaser, taille);
            p.score = p.score + next_tick(world,p.poslaser, taille);
            if (osl_keys->pressed.up){
                p.y = p.y - p.v;
                if(p.y < 0){p.y = 0;}
            }
            else if(osl_keys->pressed.left){
                p.x = p.x - p.v;
                if(p.x < 0){p.x = 479;}
            }
            else if(osl_keys->pressed.down){
                p.y = p.y + p.v;
                if(p.y > 260){p.y = 260;}
            }
            else if(osl_keys->pressed.right){
                p.x = (p.x + p.v)%480;
            }
            else if(osl_keys->pressed.square){
                p.v = p.v + 1;
            }
            else if(osl_keys->pressed.circle){
                p.v = p.v - 1;
            }
            else if(osl_keys->pressed.start){
                pause = 1;
            }
            else if(osl_keys->released.cross){
                if (p.power >= 20) {
                    p.power = p.power - 20;
                    add_laser(p.x ,p.y ,p.poslaser);
                }
            }
            if(p.power <= 100){ p.power = p.power + 0.5;}
        }
        else if(pause and osl_keys->pressed.start){
            pause = 0;
        }
    }
    return 0;
}

int menu_game(OSL_IMAGE *bg, OSL_IMAGE *mi, OSL_IMAGE *se, int label_item){
    int skip = 0;
    while(1){
        if (!skip){
            oslStartDrawing();
            oslClearScreen(RGB(0,0,0));
            oslDrawImageXY(bg, 0, 0);
            oslDrawImageXY(mi, 0, 0);
            oslDrawImageXY(se, 160, 130 + 40*label_item);
            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();
        oslReadKeys();
        if(osl_keys->released.cross){
            return label_item;
        }
        else if(osl_keys->released.up and label_item != 0){
            label_item = label_item - 1;
        }
        else if(osl_keys->released.down and label_item != 2){
            label_item = label_item + 1;
        }

    }
    return -1;
}

int credit(OSL_IMAGE *bg){
    int skip = 0;
    while(1){
        if (!skip){
            oslStartDrawing();
            oslClearScreen(RGB(0,0,0));
            oslDrawImageXY(bg, 0, 0);
            oslDrawStringf(205,136,"Programing by\nLOLO06");
            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();
        oslReadKeys();
        if(osl_keys->released.circle){
            return 2;
        }
    }
    return -1;
}
