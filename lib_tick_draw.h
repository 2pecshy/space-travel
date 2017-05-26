struct part
{
	int x;
	int y;
	int v;
};
struct player
{
	int x;
    int y;
    int v;
    part poslaser[20];
	int nb_vie;
	float power;
	int score;
};
int initOSLib();
player iniplayer();
void add_laser(int x,int y,part *tab);
void inigame(part *tab, int taille);
int next_tick(part *tab, part *tab2, int taille);
void drawBar(int x, int y, int longeur, int largeur,int val, OSL_COLOR color);
int colision(player p, part *tab, part *tab2, int taille);
int run_game(OSL_IMAGE *bg, OSL_IMAGE *shipimg, OSL_IMAGE *bkg, OSL_IMAGE *pa, OSL_IMAGE *se);
int menu_game(OSL_IMAGE *bg, OSL_IMAGE *mi, OSL_IMAGE *se, int label_item);
int credit(OSL_IMAGE *bg);
