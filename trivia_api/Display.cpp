#include "Display.h"

static MKRIoTCarrier* displayCarrier = nullptr;

void beginDisplay(MKRIoTCarrier& carrier) {
  displayCarrier = &carrier;
}

void printLine(String text, int y) {
  if (displayCarrier == nullptr) return;

  text = text.substring(0, 18);

  int16_t x1, y1;
  uint16_t w, h;

  displayCarrier->display.getTextBounds(
    text, 0, y, &x1, &y1, &w, &h
  );

  displayCarrier->display.setCursor(
    (240 - (int)w) / 2 - x1, y
  );

  displayCarrier->display.print(text);
}

void showScreen(String title, String row1, String row2) {
  if (displayCarrier == nullptr) return;

  displayCarrier->display.fillScreen(ST77XX_BLACK);
  displayCarrier->display.setTextSize(2);
  displayCarrier->display.setTextColor(ST77XX_WHITE);

  printLine(title, 50);
  printLine(row1, 100);
  printLine(row2, 135);

  displayCarrier->display.setTextColor(ST77XX_CYAN);
  printLine("0: new data", 185);
}