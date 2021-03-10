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
Bridge bridge = initBridge(5, &gui);
CarQueue northB = initCarQueue(&bridge, &controller);
CarQueue southB = initCarQueue(&bridge, &controller);
Controller controller = initController(&northB, &southB);


int main(void)
{

}

