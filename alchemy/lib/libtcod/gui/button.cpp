#include <string.h>
#include "libtcod.hpp"
#include "gui.hpp"

Button::Button(const char *label,const char *tip,widget_callback_t cbk, void *userData) 
	: pressed(false),label(NULL) {
	if ( label != NULL ) {
		setLabel(label);
	}
	if ( tip != NULL ) setTip(tip);
	this->x=0;
	this->y=0;
	this->userData=userData;
	this->cbk=cbk;
}

Button::Button(int x,int y,int width, int height,const char *label,const char *tip,widget_callback_t cbk, void *userData) 
	: pressed(false), label(NULL) {
	if ( label != NULL ) setLabel(label);
	if ( tip != NULL ) setTip(tip);
	w=width;
	h=height;
	this->x=x;
	this->y=y;
	this->userData=userData;
	this->cbk=cbk;
}

Button::~Button() {
	if ( label ) free(label);
}

void Button::setLabel(const char *newLabel) {
	if ( label ) free(label);
	label=strdup(newLabel);
}

void Button::render() {
	con->setBackgroundColor(mouseIn ? backFocus : back);
	con->setForegroundColor(mouseIn ? foreFocus : fore);
	con->rect(x,y,w,h,true,TCOD_BKGND_SET);
	if ( label ) {
		if ( pressed && mouseIn ) {
			con->printCenter(x+w/2,y,TCOD_BKGND_NONE,"-%s-",label);
		} else {
			con->printCenter(x+w/2,y,TCOD_BKGND_NONE,label);
		}
	}
}

void Button::computeSize() {
	if ( label != NULL ) {
		w=strlen(label)+2;
	} else {
		w=4;
	}
	h=1;	
}

void Button::expand(int width, int height) {
	if ( w < width ) w = width;
}

void Button::onButtonPress() {
	pressed=true;
}

void Button::onButtonRelease() {
	pressed=false;
}

void Button::onButtonClick() {
	if ( cbk ) cbk(this,userData);
}



