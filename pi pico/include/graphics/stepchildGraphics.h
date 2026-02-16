//original ChildOS fonts
#include "fonts/7_segment.cpp"
#include "fonts/cursive.cpp"
#include "fonts/small.cpp"
#include "fonts/arp.cpp"
#include "fonts/italic.cpp"
#include "fonts/chunky.cpp"

//Holds all the graphics functions
//Accessed thru Stepchild.graphics.function();
class StepchildGraphics{
  public:
  StepchildGraphics(){}
  //Shades a rectangle
  void shadeArea(int16_t x1, int16_t y1, int16_t len, int16_t height, uint8_t shade);
  void drawDottedLineH(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t dot);
  void drawDottedLineV(uint8_t x1, uint8_t y1, uint8_t y2, uint8_t dot);

  //this one draws a black pixel every dot pixels
  void drawDottedLineV2(unsigned short int x1, unsigned short int y1, unsigned short int y2, unsigned short int dot);
  void drawDottedLineDiagonal(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t dot);

  void shadeRect(int16_t x1, int16_t y1, int16_t len, int16_t height, uint8_t shade);
  void drawBinarySelectionBox(int8_t x1, int8_t y1, String op1, String op2, String title, bool state);
  void shadeLineV(int16_t xPos, int16_t yPos, int16_t len, uint8_t shade);

  void drawDottedRect(int8_t x1, int8_t y1, uint8_t w, uint8_t h, uint8_t dot);
  void drawRotatedRect(uint8_t x1, uint8_t y1, uint8_t length, uint8_t height, float angle, uint16_t c);
  /*-------------------------------------------
                  Fractions!
  -------------------------------------------*/
  //fractions are 7x11 (if it's 1 digit numerator)
  void printFraction(uint8_t x, uint8_t y, uint8_t numerator, uint8_t denominator);
  //feed it a number in the form "1 7/8"
  void printFraction(uint8_t x1, uint8_t y1, String fraction);
  void printFractionCentered(uint8_t x1, uint8_t y1, String fraction);
  //feed it a number in the form "1 7/8"
  uint8_t printFraction_small(uint8_t x1, uint8_t y1, String fraction);
  //centers the fractions
  void printFraction_small_centered(uint8_t x1, uint8_t y1, String fraction);
  void drawCheckmark(int16_t x, int16_t y, uint8_t size, uint16_t c);
  void drawCheckbox(int x, int y, bool checked, bool selected);
  void drawRadian(uint8_t h, uint8_t k, int a, int b, float angle, uint16_t c);
  void drawCircleRadian(uint8_t h, uint8_t k, int r, float angle, uint16_t c);
  void drawPendulum(int16_t x2, int16_t y2, int8_t length, float val, uint8_t r);
  //draws a pendulum where "val" is a the angle of the pendulum
  void drawPendulum(int16_t x2, int16_t y2, int8_t length, float val);
  void fillEllipse(uint8_t h, uint8_t k, int a, int b,uint16_t c);
  void drawEllipse(uint8_t h, uint8_t k, int a, int b, uint16_t c);
  void drawEllipse(uint8_t h, uint8_t k, int a, int b, uint8_t skip, uint16_t c);
  void drawStar(uint8_t centerX, uint8_t centerY, uint8_t r1, uint8_t r2, uint8_t points);
  void printChannel(uint8_t xPos, uint8_t yPos, uint8_t channel, bool withBox);
  //draws a horizontal bar graph -- so far totally unused
  void drawBarGraphH(int xStart, int yStart, int thickness, int length, float progress);
  void drawBarGraphV(uint8_t xStart, uint8_t yStart, uint8_t thickness, uint8_t height, float progress);
  //Draws percentage bar showing how full the sequence is
  void drawSequenceMemoryBar(uint8_t x1, uint8_t y1, uint8_t length);

  // #define JOY_RIGHT 0
  // #define JOY_LEFT 1
  // #define UP 2
  // #define DOWN 3
  void drawArrow(uint8_t pointX, uint8_t pointY, uint8_t size, uint8_t direction, bool full);
  void drawArrow(uint8_t pointX, uint8_t pointY, uint8_t size, uint8_t direction, uint8_t fillColor, uint8_t highlightColor, bool drawHighlight);
  void drawHighlightArrow(uint8_t pointX,uint8_t pointY, uint8_t size, uint8_t direction);
  void drawBanner(uint8_t x1, uint8_t y1, String text);
  void drawCenteredBanner(uint8_t x1, uint8_t y1, String text);
  //draws a VU meter, where val is the angle of the needle
  void drawVU(int8_t x1, int8_t y1, float val);
  void drawLabel_outline(uint8_t x1, uint8_t y1, String text);
  void drawLabel(uint8_t x1, uint8_t y1, String text, bool wOrB);
  void drawOutlinedButton(uint8_t x, uint8_t y, String t, bool wOrB);
  void drawButton(uint8_t x, uint8_t y, String t, bool wOrB);
  void drawSlider(uint8_t x, uint8_t y, vector<String> &options, uint8_t which);
  void drawInputIcon(uint8_t x, uint8_t y, String t);
  //Draws a yes/no slider
  void drawSlider(uint8_t x1, uint8_t y1, uint8_t w, uint8_t h, bool state);
  //draws a yes/no slider w/two text labels
  void drawSlider(uint8_t x1, uint8_t y1, String a, String b, bool state);
  void drawFullKeyBed(uint8_t y1, vector<uint8_t> pressList, vector<uint8_t> mask, uint8_t activeKey,uint8_t octave);

  void drawFullKeyBed(vector<uint8_t> pressList, vector<uint8_t> mask, uint8_t activeKey,uint8_t octave);
  //draws the pram icon, bouncing to the beat of the sequence
  void drawPram(uint8_t x1, uint8_t y1);
  void drawBigPram(uint8_t x, uint8_t y);
  void drawTinyPram(uint8_t x, uint8_t y);
  //draws a play icon shaded according to the sequence clock state
  void drawPlayIcon(int8_t x1, int8_t y1);
  //draws the power icon corresponding to USB/batt and batt level
  void drawPower(uint8_t x1, uint8_t y1);

  //drawing rects w/missing corners (instead of calling roundRect() with radius 2)
  void drawRectWithMissingCorners(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t c);
  void fillRectWithMissingCorners(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t c);
  //gen midi numbers taken from https://usermanuals.finalemusic.com/SongWriter2012Win/Content/PercussionMaps.htm

  void fillSquareVertically(uint8_t x0, uint8_t y0, uint8_t width, uint8_t fillAmount);
  //fill amount is a percent out of maxFill
  void fillSquareDiagonally(uint8_t x0, uint8_t y0, uint8_t width,uint8_t fillAmount, uint8_t maxFill);
  //overload to default to /100
  void fillSquareDiagonally(uint8_t x0, uint8_t y0, uint8_t width,uint8_t fillAmount);

  //prints pitch with a small # and either a large or small Octave number
  void printTrackPitch(uint8_t xCoord, uint8_t yCoord, uint8_t trackID,bool bigOct, bool channel, uint16_t c);

  //prints a formatted pitch and returns the pixel length of the printed pitch
  uint8_t printPitch(uint8_t xCoord, uint8_t yCoord, String pitch, bool bigOct, bool channel, uint16_t c);
  void drawBox(uint8_t cornerX, uint8_t cornerY, uint8_t width, uint8_t height, uint8_t depth, int8_t xSlant, uint8_t fill);
  void drawBracket(uint8_t h, uint8_t k, uint8_t height, uint8_t width, uint8_t style, uint16_t c);
  void drawTetra(uint8_t h, uint8_t k, uint8_t height, uint8_t width, int xDepth, int yDepth, uint8_t style, uint16_t c);
  void fillAroundCircle(float x, float y, float r, uint16_t c);
  void drawMoon(int phase, bool forward);
  void drawMoon_reverse(int phase);
  void drawProgBar(String text, float progress);
  void bootscreen();
  void helloChild_1();
  void helloChild_2();
  void helloChild_3();
  void helloChild_4();
  void helloChild_5();
  void drawLogo(uint8_t x1, uint8_t y1);
  void bootscreen_2();
  void bootscreen_3();
  void drawOSScreen();
  void drawSmallChannelIcon(uint8_t x1, uint8_t y1, uint8_t ch);
  void drawMoveIcon(uint8_t x1, uint8_t y1, bool anim);
  void drawLengthIcon(uint8_t x1, uint8_t y1, uint8_t length, uint8_t animThing, bool anim);
  void drawFxIcon(uint8_t x1,uint8_t y1, uint8_t w, bool anim);
  //two blocks
  void drawQuantIcon(uint8_t x1, uint8_t y1, uint8_t size, bool anim);
  void drawDrumIcon(uint8_t x1, uint8_t y1, uint8_t note);
  //one square, one rotated square
  void drawHumanizeIcon(uint8_t x1, uint8_t y1, uint8_t size, bool anim);
  void drawChopIcon(uint8_t x1, uint8_t y1, uint8_t size, bool anim);
  void drawVelIcon(uint8_t x1, uint8_t y1, uint8_t w, bool anim);
  void drawChanceIcon(uint8_t x1, uint8_t y1, uint8_t w, bool anim);
  //inverting square
  void drawReverseIcon(uint8_t x1, uint8_t y1, uint8_t w, bool anim);
  //concentric circles
  void drawEchoIcon(uint8_t x1, uint8_t y1, uint8_t w, bool anim);
  //die
  void drawRandomIcon(uint8_t x1, uint8_t y1, uint8_t w, bool anim);
  void drawQuickFunctionIcon(uint8_t x1, uint8_t y1, uint8_t w, bool anim);
  void drawQuantBrackets(uint8_t x1, uint8_t y1);
};

#include "stepchildGraphics.cpp"

StepchildGraphics graphics;