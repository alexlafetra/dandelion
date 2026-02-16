//Functions for drawing and working with 7 segment numbers
void drawEmptySegment(uint8_t x1, uint8_t y1, uint8_t seg){
  switch(seg){
    //top
    case 0:
      display.drawBitmap(x1+2,y1,seg_empty_h,9,3,SSD1306_WHITE);
      break;
    //top left
    case 1:
      display.drawBitmap(x1,y1+2,seg_empty_v,3,9,SSD1306_WHITE);
      break;
    //top right
    case 2:
      display.drawBitmap(x1+10,y1+2,seg_empty_v,3,9,SSD1306_WHITE);
      break;
    //middle
    case 3:
      display.drawBitmap(x1+2,y1+10,seg_empty_h,9,3,SSD1306_WHITE);
      break;
    //bottom left
    case 4:
      display.drawBitmap(x1,y1+12,seg_empty_v,3,9,SSD1306_WHITE);
      break;
    //bottom right
    case 5:
      display.drawBitmap(x1+10,y1+12,seg_empty_v,3,9,SSD1306_WHITE);
      break;
    //bottom
    case 6:
      display.drawBitmap(x1+2,y1+20,seg_empty_h,9,3,SSD1306_WHITE);
      break;
  }
}

void drawFullSegment(uint8_t x1, uint8_t y1, uint8_t seg){
  switch(seg){
    //top
    case 0:
      display.drawBitmap(x1+2,y1,seg_h,9,3,SSD1306_WHITE);
      break;
    //top left
    case 1:
      display.drawBitmap(x1,y1+2,seg_v,3,9,SSD1306_WHITE);
      break;
    //top right
    case 2:
      display.drawBitmap(x1+10,y1+2,seg_v,3,9,SSD1306_WHITE);
      break;
    //middle
    case 3:
      display.drawBitmap(x1+2,y1+10,seg_h,9,3,SSD1306_WHITE);
      break;
    //bottom left
    case 4:
      display.drawBitmap(x1,y1+12,seg_v,3,9,SSD1306_WHITE);
      break;
    //bottom right
    case 5:
      display.drawBitmap(x1+10,y1+12,seg_v,3,9,SSD1306_WHITE);
      break;
    //bottom
    case 6:
      display.drawBitmap(x1+2,y1+20,seg_h,9,3,SSD1306_WHITE);
      break;
  }
}

void drawFullSegmentItalic(uint8_t x1, uint8_t y1, uint8_t seg){
  switch(seg){
    //top
    case 0:
      display.drawBitmap(x1+4,y1,seg_h,9,3,SSD1306_WHITE);
      break;
    //top left
    case 1:
      display.drawBitmap(x1+1,y1+2,seg_italic,4,9,SSD1306_WHITE);
      break;
    //top right
    case 2:
      display.drawBitmap(x1+11,y1+2,seg_italic,4,9,SSD1306_WHITE);
      break;
    //middle
    case 3:
      display.drawBitmap(x1+3,y1+10,seg_h,9,3,SSD1306_WHITE);
      break;
    //bottom left
    case 4:
      display.drawBitmap(x1,y1+12,seg_italic,4,9,SSD1306_WHITE);
      break;
    //bottom right
    case 5:
      display.drawBitmap(x1+10,y1+12,seg_italic,4,9,SSD1306_WHITE);
      break;
    //bottom
    case 6:
      display.drawBitmap(x1+2,y1+20,seg_h,9,3,SSD1306_WHITE);
      break;
  }
}
uint8_t get7SegLength(int16_t number){
  String num = stringify(number);
  uint8_t l = num.length()*16;
  return l;
}

void print7SegItalic(uint8_t x1, uint8_t y1, char letter){
    bool segments[7] = {false,false,false,false,false,false,false};
    switch(letter){
      case ' ':
        return;
      case '-':
        segments[3] = true;
        break;
      case '0':
        segments[0] = true;
        segments[1] = true;
        segments[2] = true;
        segments[4] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '1':
        segments[2] = true;
        segments[5] = true;
        // x1-=10;
        break;
      case '2':
        segments[0] = true;
        segments[2] = true;
        segments[3] = true;
        segments[4] = true;
        segments[6] = true;
        break;
      case '3':
        segments[0] = true;
        segments[2] = true;
        segments[3] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '4':
        segments[1] = true;
        segments[2] = true;
        segments[3] = true;
        segments[5] = true;
        break;
      case '5':
        segments[0] = true;
        segments[1] = true;
        segments[3] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '6':
        segments[0] = true;
        segments[1] = true;
        segments[3] = true;
        segments[4] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '7':
        segments[0] = true;
        segments[2] = true;
        segments[5] = true;
        break;
      case '8':
        segments[0] = true;
        segments[1] = true;
        segments[2] = true;
        segments[3] = true;
        segments[4] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '9':
        segments[0] = true;
        segments[2] = true;
        segments[1] = true;
        segments[3] = true;
        segments[5] = true;
        segments[6] = true;
        break;
    }
    for(uint8_t i = 0; i<7; i++){
      if(segments[i])
        drawFullSegmentItalic(x1,y1,i);
    }
}

void print7SegItalic(uint8_t x1, uint8_t y1, String num){
  for(uint8_t character = 0; character<num.length(); character++){
    char letter = num.charAt(character);
      print7SegItalic(x1+character*16,y1,letter);
  }
}
void print7SegItalic(uint8_t x1, uint8_t y1, int num){
  print7SegItalic(x1,y1,stringify(num));
}

void print7SegItalic_wiggly(uint8_t x1, uint8_t y1, String num){
  for(uint8_t i = 0; i<num.length(); i++){
    print7SegItalic(x1+i*16, y1+float(3.0*float(sin(millis()/200.00+2*i))),num.charAt(i));
  }
}


void print7SegString(uint8_t x1, uint8_t y1, String num,bool withOutlines){
  for(uint8_t character = 0; character<num.length(); character++){
    bool segments[7] = {false,false,false,false,false,false,false};
    unsigned char letter = num.charAt(character);
    switch(letter){
      case ' ':
        continue;
      case '-':
        segments[3] = true;
        break;
      case '0':
        segments[0] = true;
        segments[1] = true;
        segments[2] = true;
        segments[4] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '1':
        segments[2] = true;
        segments[5] = true;
        // x1-=10;
        break;
      case '2':
        segments[0] = true;
        segments[2] = true;
        segments[3] = true;
        segments[4] = true;
        segments[6] = true;
        break;
      case '3':
        segments[0] = true;
        segments[2] = true;
        segments[3] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '4':
        segments[1] = true;
        segments[2] = true;
        segments[3] = true;
        segments[5] = true;
        break;
      case '5':
        segments[0] = true;
        segments[1] = true;
        segments[3] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '6':
        segments[0] = true;
        segments[1] = true;
        segments[3] = true;
        segments[4] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '7':
        segments[0] = true;
        segments[2] = true;
        segments[5] = true;
        break;
      case '8':
        segments[0] = true;
        segments[1] = true;
        segments[2] = true;
        segments[3] = true;
        segments[4] = true;
        segments[5] = true;
        segments[6] = true;
        break;
      case '9':
        segments[0] = true;
        segments[2] = true;
        segments[1] = true;
        segments[3] = true;
        segments[5] = true;
        segments[6] = true;
        break;
    }
    for(uint8_t i = 0; i<7; i++){
      if(segments[i])
        drawFullSegment(x1+character*16,y1,i);
      else if(withOutlines)
        drawEmptySegment(x1+character*16,y1,i);
    }
  }
}

void print7SegNumber(uint8_t x1, uint8_t y1, int16_t number,bool withOutlines){
  String num = stringify(number);
  print7SegString(x1,y1,num,withOutlines);
}

//prints a small 7 seg number
void print7SegSmall(int x, int y, String text, uint16_t c){
  for(uint8_t i = 0; i<text.length(); i++){
    uint8_t a = 0;//init a with zero, just in case
    switch(text.charAt(i)){
      case '0':
        a = 0;
        break;
      case '1':
        a = 1;
        break;
      case '2':
        a = 2;
        break;
      case '3':
        a = 3;
        break;
      case '4':
        a = 4;
        break;
      case '5':
        a = 5;
        break;
      case '6':
        a = 6;
        break;
      case '7':
        a = 7;
        break;
      case '8':
        a = 8;
        break;
      case '9':
        a = 9;
        break;
      case '%':
        a = 10;
        break;
      case '-':
        a = 11;
        break;
    }
    display.drawBitmap(x,y,small7seg[a],4,7,c);
    x+=5;
  }
}

//fills string so there are always at least 3 digits
void print007SegSmall(int x, int y, String text, uint16_t c){
#ifndef HEADLESS
  if(text.charAt(0) == '-'){
    while(text.length()<4){
      text.setCharAt(0,'0');
      text = "-"+text;
    }
  }
  else if(text.endsWith("%")){
    while(text.length()<4){
      text = "0"+text;
    }
  }
  else{
    while(text.length()<3){
      text = "0"+text;
    }
  }
#else
    if(text[0] == '-'){
      while(text.length()<4){
        text[0] = '0';
        text = "-"+text;
      }
    }
    else if(text[text.length()-1] == '%'){
      while(text.length()<4){
        text = "0"+text;
      }
    }
    else{
      while(text.length()<3){
        text = "0"+text;
      }
    }
#endif
  print7SegSmall(x,y,text,c);
}
