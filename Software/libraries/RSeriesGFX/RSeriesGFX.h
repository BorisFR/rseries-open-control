#ifndef _RSERIES_GFX_H
#define _RSERIES_GFX_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <stdint.h>


// Touch Screen Pin Configuration - Need to change A2 & A3, so as not to share
#define YM 9    // Y- (Minus) digital pin UNO = D9, MEGA = 9   // Orig 9
#define XM A8   // X- (Minus) must be an analog pin, use "An" notation! // Orig A2
#define YP A9   // Y+ (Plus)  must be an analog pin, use "An" notation! // Orig A3
#define XP 8    // X+ (Plus)  digital pin UNO = 8, MEGA = 8   // Orig 9

// These can be adjusted for greater precision 
#define TS_MINX 131 // Orig = 150
#define TS_MINY 120 // Orig = 120
#define TS_MAXX 920 // Orig = 920
#define TS_MAXY 950 // Orig = 940

#define rotation 3  // Which only changes the orientation of the LCD not the touch screen

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0 

#define LCD_RESET A4              // Use A4 to reset the Pin 7 of the TFT Display - Not Optional

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF
#define WINE            0x8888
#define DRKBLUE         0x1111
#define TAN             0xCCCC
#define GRAY            0x038F

#define optionCOLOR RED

// button styles
#define BTN_STYLE_BOX 1
#define BTN_STYLE_ROUND_BOX 2
#define BTN_STYLE_TRIANGLE_UP 3
#define BTN_STYLE_TRIANGLE_DOWN 4
#define BTN_STYLE_CIRCLE 5

#define BTN_INDEX_MAX 10
#define MENU_INDEX_MAX 10


class rsButton{
  public:
  	int x, y, width, height, style, commandCode;
  	uint16_t color;
  	//String label;
    char* label;
  	rsButton();
  	rsButton(int inX, int inY, int inWidth, int inHeight, int inBtnStyle, uint16_t inColor, char* inLabel, int inCommandCode, void (*inCallback)(int cc));
  	void checkButton(Point testPoint);
  	void onButton();
  	void draw(Adafruit_TFTLCD tft);
  private:
  	void (*callback)(int cc);
};


class rsMenuSet{
  public:
  	rsMenuSet()
  	{
  		currentBtnIdx = 0;
  		currentMenuIdx = 0;
  	};

    void setMenuCount(int inMenuCount)
    {
      menuCount = inMenuCount;
      for(int j=0; j<10;j++){
        buttonCounts[j] = 0;
      }
    };

  	void addButton(rsButton *inButton, int menuIndex){
  		buttons[menuIndex][buttonCounts[menuIndex]] = inButton;
      buttonCounts[menuIndex]+=1;
  	};

  	void drawButtons(Adafruit_TFTLCD tft)
  	{
      Serial.println("rsMenuSet::drawButtons");
  		for(int i=0;i<buttonCounts[currentMenuIdx];i++)
  		{
        Serial.println("rsMenuSet::drawButtons - button");
  			buttons[currentMenuIdx][i]->draw(tft);
  		}
  	};

  	void nextBtn()
  	{ 
	  	currentBtnIdx++; 
	  	if(currentBtnIdx >= buttonCounts[currentMenuIdx])
	  		currentBtnIdx = 0;
	  };

  	void prevBtn()
  	{ 
  		currentBtnIdx--; 
  		if(currentBtnIdx < 0)
  	  		currentBtnIdx = (buttonCounts[currentMenuIdx]-1);
	  };

  	void nextMenu()
  	{
  		currentMenuIdx++;
  		if(currentMenuIdx >= menuCount)
	  		currentMenuIdx = 0;
  	};

  	void prevMenu()
  	{
	  	currentMenuIdx--;
	  	if(currentMenuIdx < 0)
	  		currentMenuIdx = (menuCount-1);
	  };

    void setMenuIndex(int inIdx)
    {
      if((inIdx >= 0)&&(inIdx < (menuCount-1) ))
      {
        currentMenuIdx = inIdx;
      }
    };

    void checkButtons(Point testPoint)
    {
      for(int i=0;i<buttonCounts[currentMenuIdx];i++)
      {
        buttons[currentMenuIdx][i]->checkButton(testPoint);
      }
    };

    int getMenuPage(){ return currentMenuIdx; };

  private:	
  	int currentBtnIdx;
  	int currentMenuIdx;
    rsButton* buttons[MENU_INDEX_MAX][BTN_INDEX_MAX];
    int buttonCounts[MENU_INDEX_MAX];
    int menuCount;
};


class RSeriesGFX{
  public:
  	RSeriesGFX();
    void initDisplay();
    void displayOPTIONS();
  	void displaySCROLL();
  	void displaySPLASH(String owner, String version);
  	void displayBATT(float vin, float vinSTRONG, float vinWEAK);
  	void displaySTATUS(
                     uint16_t color, 
                     String mechName, 
                     String radiostatus, 
                     unsigned long RSSIduration,
                     int ProcessStateIndicator,
                     float dmmVCC,
                     float yellowVCC,
                     float redVCC,
                     float dmmVCA,
                     float yellowVCA,
                     float redVCA,
                     float vin, 
                     float vinSTRONG, 
                     float vinWEAK);
    void displayRSSI(unsigned long RSSIduration);
    void clearScreen(){tft.fillScreen(BLACK);};
    void displaySendClear() { tft.fillRect(80, 21, 220, 17, BLACK); };// Clear Trigger Message
    void displaySendMessage(int triggerItem);

    void displayPOSTStage1();
    void displayPOSTStage2();
    void displayPOSTStage3();
    void displayPOSTStage4();
    void displayPOSTStage5();

    void processTouch();

    rsMenuSet menus; //public for now

  private:
  	TouchScreen ts;
  	Adafruit_TFTLCD tft;
  	Point getTouch();

};

#endif