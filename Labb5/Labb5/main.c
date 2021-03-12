/*
 * Labb5.c
 *
 * Created: 2021-03-08 10:41:56
 * Author : hjall
 */ 

// shaved off är ground DONT FCKING FORGET

#include <avr/io.h>
#include "Bridge.h"
#include "CarQueue.h"
#include "GUI.h"
#include "Inputhandler.h"
#include "PortWriter.h"
#include "TinyTimber.h"
#include "Controller.h"

GUI gui = initGUI();
PortWriter writer = initPortWriter();
Bridge bridge = initBridge(5, &gui);
Controller controller = initController(&bridge);
CarQueue northB = initCarQueue(&bridge, &controller, &writer, &gui);
CarQueue southB = initCarQueue(&bridge, &controller, &writer, &gui);
//&(controller)->northbound = &northB; // fix this
//&(controller)->southbound = &southB; // fix this

int main(void)
{
	connectRoads(&controller, &northB, &southB);
}

