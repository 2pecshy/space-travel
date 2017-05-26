#include <pspkernel.h>
#include <oslib/oslib.h>
#include "lib_tick_draw.h"

//author: lolo06

PSP_MODULE_INFO("space travel", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12*1024);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    initOSLib();

    //Loads image:
    OSL_IMAGE *bkg = oslLoadImageFilePNG("picture/asteroide.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    OSL_IMAGE *shipimg = oslLoadImageFilePNG("picture/space_ship.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    OSL_IMAGE *bg = oslLoadImageFilePNG("picture/bg.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    OSL_IMAGE *mi = oslLoadImageFilePNG("picture/mi.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    OSL_IMAGE *se = oslLoadImageFilePNG("picture/se.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    OSL_IMAGE *pa = oslLoadImageFilePNG("picture/pause.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

    int label = 0;

    while(1){

        label = menu_game(bg, mi, se, label);
        if(label == 0){
            run_game(bg, shipimg, bkg, pa, se);
        }
        else if(label == 2){
            credit( bg);
        }
        else {break;}
    }
    //Quit OSL:
    oslEndGfx();

    sceKernelExitGame();
    return 0;
}
