/*
* Umbra
* Copyright (c) 2009 Dominik Marczuk
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Dominik Marczuk may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY DOMINIK MARCZUK ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL DOMINIK MARCZUK BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "umbra.hpp"

#define MAXIMISED_MODE_WIDTH 30
#define MAXIMISED_MODE_HEIGHT 8

UmbraModSpeed::UmbraModSpeed (void) : cumulatedElapsed(0.0f), updateTime(0.0f), renderTime(0.0f),
    updatePer(0),renderPer(0),sysPer(0),isMinimised(false) {
    speed = new TCODConsole(MAXIMISED_MODE_WIDTH,MAXIMISED_MODE_HEIGHT);
    rect.set((UmbraConfig::rootWidth/2)-15,(UmbraConfig::rootHeight/2)-3,30,MAXIMISED_MODE_HEIGHT);
	canDrag = true;
	// the title bar is drag-sensible
	dragZone.set(0,0,MAXIMISED_MODE_WIDTH-3,1);
    //the buttons:
    minimiseButton.set(MAXIMISED_MODE_WIDTH-3,0);
    closeButton.set(MAXIMISED_MODE_WIDTH-2,0);
	priority=0; // higher prio for internal modules
}

void UmbraModSpeed::mouse (TCOD_mouse_t &ms) {
	UmbraWidget::mouse(ms);
	if (ms.lbutton_pressed) {
		ms.lbutton_pressed=false; // erase event
		//minimise button is pressed
		if (minimiseButton.is(mousex,mousey)) {
            isMinimised = !isMinimised;
            if (isMinimised) {
                rect.setSize(9,1);
                minimiseButton.set(7,0);
                closeButton.set(8,0);
                dragZone.w = 7;
            } else {
                rect.setSize(MAXIMISED_MODE_WIDTH,MAXIMISED_MODE_HEIGHT);
                minimiseButton.set(MAXIMISED_MODE_WIDTH-3,0);
                closeButton.set(MAXIMISED_MODE_WIDTH-2,0);
                dragZone.w = MAXIMISED_MODE_WIDTH-3;
                // when the widget maximizes, it might cross the screen borders
                rect.x=MIN(UmbraConfig::rootWidth-rect.w,rect.x);
                rect.y=MIN(UmbraConfig::rootHeight-rect.h,rect.y);
            }
		}
		//close button is pressed
		else if (closeButton.is(mousex,mousey)) {
		    getEngine()->deactivateModule(this);
		}
	}
}

bool UmbraModSpeed::update (void) {
    cumulatedElapsed += TCODSystem::getLastFrameLength();

    if ( cumulatedElapsed >= 1.0f ) {
        updatePer = (int)(updateTime * 100.0f / (cumulatedElapsed));
        renderPer = (int)(renderTime * 100.0f / (cumulatedElapsed));
        sysPer = 100 - updatePer - renderPer;
        cumulatedElapsed = updateTime = renderTime = 0.0f;
    }
    if (getStatus() == UMBRA_ACTIVE) return true;
    else return false;
}

void UmbraModSpeed::setTimes(long updateTime, long renderTime) {
    this->updateTime += updateTime * 0.001f;
    this->renderTime += renderTime * 0.001f;
}

void UmbraModSpeed::render (void) {
    speed->setBackgroundColor(TCODColor::black);
    speed->setForegroundColor(TCODColor::white);
	if ( isMinimised ) {
		speed->printLeft(0,0,TCOD_BKGND_SET,"%4dfps ",TCODSystem::getFps());
		TCODConsole::blit(speed,0,0,8,1,TCODConsole::root,rect.x,rect.y,1.0f,0.5f);
	} else {
		speed->printFrame(0,0,MAXIMISED_MODE_WIDTH,MAXIMISED_MODE_HEIGHT,true,"Speed-o-meter");
		speed->printCenter(MAXIMISED_MODE_WIDTH/2,2,TCOD_BKGND_NONE,"last frame: %3d ms",(int)(TCODSystem::getLastFrameLength()*1000));
		speed->printCenter(MAXIMISED_MODE_WIDTH/2,3,TCOD_BKGND_NONE,"frames per second: %3d",TCODSystem::getFps());
		// render time bar
        int x=2;
        int barLength=MAXIMISED_MODE_WIDTH-3;
        // udpate part
        speed->setBackgroundColor(TCODColor::green);
		speed->rect(x,4,barLength*updatePer/100,1,false, TCOD_BKGND_SET);
		x += barLength*updatePer/100;
		// render part
        speed->setBackgroundColor(TCODColor::blue);
		speed->rect(x,4,barLength*renderPer/100,1,false, TCOD_BKGND_SET);
		x += barLength*renderPer/100;
		// system part
        speed->setBackgroundColor(TCODColor::red);
		speed->rect(x,4,barLength*sysPer/100,1,false, TCOD_BKGND_SET);
        speed->setBackgroundColor(TCODColor::black);
        // summary
		speed->printCenter(MAXIMISED_MODE_WIDTH/2,5,TCOD_BKGND_NONE,
            "%c%c%c%cUpd%c %2d%% %c%c%c%cRender%c %2d%% %c%c%c%cSys%c %2d%%",
            TCOD_COLCTRL_FORE_RGB,1,255,1,TCOD_COLCTRL_STOP,
            updatePer,
            TCOD_COLCTRL_FORE_RGB,1,1,255,TCOD_COLCTRL_STOP,
            renderPer,
            TCOD_COLCTRL_FORE_RGB,255,1,1,TCOD_COLCTRL_STOP,
            sysPer);
		if ( dragZone.mouseHover ) {
            speed->setBackgroundColor(TCODColor::lightRed);
		    speed->rect(7,0,15,1,false,TCOD_BKGND_SET);
		}
	}
	speed->setBackgroundColor(TCODColor::black);
	// draw minimize button
	if (minimiseButton.mouseHover) speed->setForegroundColor(TCODColor::white); //button is active
	else speed->setForegroundColor(TCODColor::lightGrey); //button is not active
	speed->putChar(minimiseButton.x,minimiseButton.y, isMinimised ? '+' : '-', TCOD_BKGND_SET);
	//draw close button
	if (closeButton.mouseHover) speed->setForegroundColor(TCODColor::red); //button is active
	else speed->setForegroundColor(TCODColor::lightGrey); //button is not active
	speed->putChar(closeButton.x,closeButton.y, 'X', TCOD_BKGND_SET);
	//blit the console
	TCODConsole::blit(speed,0,0,rect.w,rect.h,TCODConsole::root,rect.x,rect.y,1.0f,0.5f);
}

void UmbraModSpeed::activate (void) {
    fps = TCODSystem::getFps();
    TCODSystem::setFps(0);
}

void UmbraModSpeed::deactivate (void) {
    TCODSystem::setFps(fps);
}
