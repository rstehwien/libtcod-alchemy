#include "libtcod.hpp"
#include "gui.hpp"

void StatusBar::render() {
	con->setBackgroundColor(back);
	con->rect(x,y,w,h,true,TCOD_BKGND_SET);
	if ( focus && focus->tip ) {
		con->setForegroundColor(fore);
		con->printLeftRect(x+1,y,w,h,TCOD_BKGND_NONE,focus->tip);
	}
}


