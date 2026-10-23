#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Arduino_MKRIoTCarrier.h>

void beginDisplay(MKRIoTCarrier& carrier);
void printLine(String text, int y);
void showScreen(String title, String row1, String row2);

#endif