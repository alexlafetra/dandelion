// 'a', 7x7px
const unsigned char italic_a []  = {
	0x3e, 0x3e, 0x6c, 0x6c, 0x7c, 0xc8, 0xc8
};
// 'b', 7x7px
const unsigned char italic_b []  = {
	0x3e, 0x3e, 0x74, 0x7c, 0x6c, 0xe8, 0xf8
};
// 'c', 7x7px
const unsigned char italic_c []  = {
	0x3e, 0x3c, 0x60, 0x60, 0x60, 0xc0, 0xf8
};
// 'd', 7x7px
const unsigned char italic_d []  = {
	0x3c, 0x3e, 0x64, 0x64, 0x64, 0xc8, 0xf8
};
// 'e', 7x7px
const unsigned char italic_e []  = {
	0x3e, 0x3c, 0x60, 0x60, 0x7c, 0xc0, 0xf8
};
// 'f', 7x7px
const unsigned char italic_f []  = {
	0x3e, 0x3c, 0x60, 0x7c, 0x60, 0xc0, 0xc0
};
// 'g', 7x7px
const unsigned char italic_g []  = {
	0x3e, 0x3c, 0x60, 0x60, 0x6c, 0xc4, 0xf8
};
// 'h', 7x7px
const unsigned char italic_h []  = {
	0x32, 0x32, 0x64, 0x7c, 0x64, 0xc8, 0xc8
};
// 'i', 7x7px
const unsigned char italic_i []  = {
	0x7e, 0x18, 0x30, 0x30, 0x30, 0x60, 0xf8
};
// 'j', 7x7px
const unsigned char italic_j []  = {
	0x06, 0x06, 0x0c, 0x4c, 0x8c, 0x98, 0xf8
};
// 'k', 7x7px
const unsigned char italic_k []  = {
	0x32, 0x36, 0x7c, 0x70, 0x70, 0xc8, 0xcc
};
// 'l', 7x7px
const unsigned char italic_l []  = {
	0x18, 0x18, 0x30, 0x30, 0x30, 0x60, 0xfc
};
// 'm', 7x7px
const unsigned char italic_m []  = {
	0x76, 0x76, 0x7e, 0x56, 0xc4, 0xcc, 0xcc
};
// 'n', 7x7px
const unsigned char italic_n []  = {
	0x32, 0x32, 0x74, 0x74, 0x7c, 0xd8, 0xd8
};
// 'o', 7x7px
const unsigned char italic_o []  = {
	0x3e, 0x3e, 0x64, 0x64, 0x4c, 0xc8, 0xf8
};
// 'p', 7x7px
const unsigned char italic_p []  = {
	0x3e, 0x3e, 0x64, 0x78, 0x60, 0xc0, 0xc0
};
// 'r', 7x7px
const unsigned char italic_r []  = {
	0x3e, 0x3e, 0x64, 0x78, 0x70, 0xd8, 0xcc
};
// 'q', 7x7px
const unsigned char italic_q []  = {
	0x3e, 0x3e, 0x64, 0x74, 0x4c, 0xcc, 0xfa
};
// 't', 7x7px
const unsigned char italic_t []  = {
	0x7e, 0xfc, 0x30, 0x70, 0x60, 0xe0, 0xc0
};
// 's', 7x7px
const unsigned char italic_s []  = {
	0x1c, 0x38, 0x60, 0x38, 0x18, 0xf8, 0x70
};
// 'w', 7x7px
const unsigned char italic_w []  = {
	0xc6, 0xc6, 0x86, 0xac, 0xec, 0xd8, 0xd8
};
// 'u', 7x7px
const unsigned char italic_u []  = {
	0x6c, 0x6c, 0x48, 0xd8, 0xd8, 0xf0, 0x70
};
// 'y', 7x7px
const unsigned char italic_y []  = {
	0x66, 0x76, 0x3c, 0x18, 0x30, 0x60, 0xc0
};
// 'v', 7x7px
const unsigned char italic_v []  = {
	0x32, 0x32, 0x66, 0x6c, 0x78, 0xf0, 0xe0
};
// 'z', 7x7px
const unsigned char italic_z []  = {
	0x3e, 0x7c, 0x18, 0x30, 0x60, 0xfc, 0xf8
};
// 'x', 7x7px
const unsigned char italic_x []  = {
	0x6c, 0x78, 0x30, 0x30, 0x30, 0x78, 0xd8
};
// 'slash', 7x7px
const unsigned char italic_slash [] = {
	0x0c, 0x1c, 0x18, 0x38, 0x30, 0x70, 0x60
};

const unsigned char* italic_font[27] = {
	italic_a,
	italic_b,
	italic_c,
	italic_d,
	italic_e,
	italic_f,
	italic_g,
	italic_h,
	italic_i,
	italic_j,
	italic_k,
	italic_l,
	italic_m,
	italic_n,
	italic_o,
	italic_p,
	italic_q,
	italic_r,
	italic_s,
	italic_t,
	italic_u,
	italic_v,
	italic_w,
	italic_x,
	italic_y,
	italic_z,
  italic_slash
};
void printItalic_wave(int16_t x, int16_t y, String text, int8_t height, uint8_t timing, uint16_t c){
  for(uint8_t i = 0; i<text.length(); i++){
    int8_t y1 = y-float(height*2)*sin(millis()/timing+float(i)/float(text.length())*PI)+height;
    if(y1>y)
      y1 = y;
    printItalic(x+i*8, y1, text.charAt(i), c);
  }
}
void printItalic(int16_t x, int16_t y, char character, uint16_t c){
  switch(character){
    case ' ':
      break;
    case 'A':
      display.drawBitmap(x,y,italic_font[0],7,7,c);
      break;
    case 'B':
      display.drawBitmap(x,y,italic_font[1],7,7,c);
      break;
    case 'C':
      display.drawBitmap(x,y,italic_font[2],7,7,c);
      break;
    case 'D':
      display.drawBitmap(x,y,italic_font[3],7,7,c);
      break;
    case 'E':
      display.drawBitmap(x,y,italic_font[4],7,7,c);
      break;
    case 'F':
      display.drawBitmap(x,y,italic_font[5],7,7,c);
      break;
    case 'G':
      display.drawBitmap(x,y,italic_font[6],7,7,c);
      break;
    case 'H':
      display.drawBitmap(x,y,italic_font[7],7,7,c);
      break;
    case 'I':
      display.drawBitmap(x,y,italic_font[8],7,7,c);
      break;
    case 'J':
      display.drawBitmap(x,y,italic_font[9],7,7,c);
      break;
    case 'K':
      display.drawBitmap(x,y,italic_font[10],7,7,c);
      break;
    case 'L':
      display.drawBitmap(x,y,italic_font[11],7,7,c);
      break;
    case 'M':
      display.drawBitmap(x,y,italic_font[12],7,7,c);
      break;
    case 'N':
      display.drawBitmap(x,y,italic_font[13],7,7,c);
      break;
    case 'O':
      display.drawBitmap(x,y,italic_font[14],7,7,c);
      break;
    case 'P':
      display.drawBitmap(x,y,italic_font[15],7,7,c);
      break;
    case 'Q':
      display.drawBitmap(x,y,italic_font[16],7,7,c);
      break;
    case 'R':
      display.drawBitmap(x,y,italic_font[17],7,7,c);
      break;
    case 'S':
      display.drawBitmap(x,y,italic_font[18],7,7,c);
      break;
    case 'T':
      display.drawBitmap(x,y,italic_font[19],7,7,c);
      break;
    case 'U':
      display.drawBitmap(x,y,italic_font[20],7,7,c);
      break;
    case 'V':
      display.drawBitmap(x,y,italic_font[21],7,7,c);
      break;
    case 'W':
      display.drawBitmap(x,y,italic_font[22],7,7,c);
      break;
    case 'X':
      display.drawBitmap(x,y,italic_font[23],7,7,c);
      break;
    case 'Y':
      display.drawBitmap(x,y,italic_font[24],7,7,c);
      break;
    case 'Z':
      display.drawBitmap(x,y,italic_font[25],7,7,c);
      break;
    case 'a':
      display.drawBitmap(x,y,italic_font[0],7,7,c);
      break;
    case 'b':
      display.drawBitmap(x,y,italic_font[1],7,7,c);
      break;
    case 'c':
      display.drawBitmap(x,y,italic_font[2],7,7,c);
      break;
    case 'd':
      display.drawBitmap(x,y,italic_font[3],7,7,c);
      break;
    case 'e':
      display.drawBitmap(x,y,italic_font[4],7,7,c);
      break;
    case 'f':
      display.drawBitmap(x,y,italic_font[5],7,7,c);
      break;
    case 'g':
      display.drawBitmap(x,y,italic_font[6],7,7,c);
      break;
    case 'h':
      display.drawBitmap(x,y,italic_font[7],7,7,c);
      break;
    case 'i':
      display.drawBitmap(x,y,italic_font[8],7,7,c);
      break;
    case 'j':
      display.drawBitmap(x,y,italic_font[9],7,7,c);
      break;
    case 'k':
      display.drawBitmap(x,y,italic_font[10],7,7,c);
      break;
    case 'l':
      display.drawBitmap(x,y,italic_font[11],7,7,c);
      break;
    case 'm':
      display.drawBitmap(x,y,italic_font[12],7,7,c);
      break;
    case 'n':
      display.drawBitmap(x,y,italic_font[13],7,7,c);
      break;
    case 'o':
      display.drawBitmap(x,y,italic_font[14],7,7,c);
      break;
    case 'p':
      display.drawBitmap(x,y,italic_font[15],7,7,c);
      break;
    case 'q':
      display.drawBitmap(x,y,italic_font[16],7,7,c);
      break;
    case 'r':
      display.drawBitmap(x,y,italic_font[17],7,7,c);
      break;
    case 's':
      display.drawBitmap(x,y,italic_font[18],7,7,c);
      break;
    case 't':
      display.drawBitmap(x,y,italic_font[19],7,7,c);
      break;
    case 'u':
      display.drawBitmap(x,y,italic_font[20],7,7,c);
      break;
    case 'v':
      display.drawBitmap(x,y,italic_font[21],7,7,c);
      break;
    case 'w':
      display.drawBitmap(x,y,italic_font[22],7,7,c);
      break;
    case 'x':
      display.drawBitmap(x,y,italic_font[23],7,7,c);
      break;
    case 'y':
      display.drawBitmap(x,y,italic_font[24],7,7,c);
      break;
    case 'z':
      display.drawBitmap(x,y,italic_font[25],7,7,c);
      break;
    case '/':
      display.drawBitmap(x,y,italic_font[26],7,7,c);
      break;
  }
}
void printItalic(int16_t x, int16_t y, String text, uint16_t c){
  for(int letter = 0; letter<text.length(); letter++){
    unsigned char character = text.charAt(letter);
    switch(character){
      case ' ':
        x-=2;
        continue;
      case 'A':
        display.drawBitmap(x+8*letter,y,italic_font[0],7,7,c);
        break;
      case 'B':
        display.drawBitmap(x+8*letter,y,italic_font[1],7,7,c);
        break;
      case 'C':
        display.drawBitmap(x+8*letter,y,italic_font[2],7,7,c);
        break;
      case 'D':
        display.drawBitmap(x+8*letter,y,italic_font[3],7,7,c);
        break;
      case 'E':
        display.drawBitmap(x+8*letter,y,italic_font[4],7,7,c);
        break;
      case 'F':
        display.drawBitmap(x+8*letter,y,italic_font[5],7,7,c);
        break;
      case 'G':
        display.drawBitmap(x+8*letter,y,italic_font[6],7,7,c);
        break;
      case 'H':
        display.drawBitmap(x+8*letter,y,italic_font[7],7,7,c);
        break;
      case 'I':
        display.drawBitmap(x+8*letter,y,italic_font[8],7,7,c);
        break;
      case 'J':
        display.drawBitmap(x+8*letter,y,italic_font[9],7,7,c);
        break;
      case 'K':
        display.drawBitmap(x+8*letter,y,italic_font[10],7,7,c);
        break;
      case 'L':
        display.drawBitmap(x+8*letter,y,italic_font[11],7,7,c);
        break;
      case 'M':
        display.drawBitmap(x+8*letter,y,italic_font[12],7,7,c);
        break;
      case 'N':
        display.drawBitmap(x+8*letter,y,italic_font[13],7,7,c);
        break;
      case 'O':
        display.drawBitmap(x+8*letter,y,italic_font[14],7,7,c);
        break;
      case 'P':
        display.drawBitmap(x+8*letter,y,italic_font[15],7,7,c);
        break;
      case 'Q':
        display.drawBitmap(x+8*letter,y,italic_font[16],7,7,c);
        break;
      case 'R':
        display.drawBitmap(x+8*letter,y,italic_font[17],7,7,c);
        break;
      case 'S':
        display.drawBitmap(x+8*letter,y,italic_font[18],7,7,c);
        break;
      case 'T':
        display.drawBitmap(x+8*letter,y,italic_font[19],7,7,c);
        break;
      case 'U':
        display.drawBitmap(x+8*letter,y,italic_font[20],7,7,c);
        break;
      case 'V':
        display.drawBitmap(x+8*letter,y,italic_font[21],7,7,c);
        break;
      case 'W':
        display.drawBitmap(x+8*letter,y,italic_font[22],7,7,c);
        break;
      case 'X':
        display.drawBitmap(x+8*letter,y,italic_font[23],7,7,c);
        break;
      case 'Y':
        display.drawBitmap(x+8*letter,y,italic_font[24],7,7,c);
        break;
      case 'Z':
        display.drawBitmap(x+8*letter,y,italic_font[25],7,7,c);
        break;
      case 'a':
        display.drawBitmap(x+8*letter,y,italic_font[0],7,7,c);
        break;
      case 'b':
        display.drawBitmap(x+8*letter,y,italic_font[1],7,7,c);
        break;
      case 'c':
        display.drawBitmap(x+8*letter,y,italic_font[2],7,7,c);
        break;
      case 'd':
        display.drawBitmap(x+8*letter,y,italic_font[3],7,7,c);
        break;
      case 'e':
        display.drawBitmap(x+8*letter,y,italic_font[4],7,7,c);
        break;
      case 'f':
        display.drawBitmap(x+8*letter,y,italic_font[5],7,7,c);
        break;
      case 'g':
        display.drawBitmap(x+8*letter,y,italic_font[6],7,7,c);
        break;
      case 'h':
        display.drawBitmap(x+8*letter,y,italic_font[7],7,7,c);
        break;
      case 'i':
        display.drawBitmap(x+8*letter,y,italic_font[8],7,7,c);
        break;
      case 'j':
        display.drawBitmap(x+8*letter,y,italic_font[9],7,7,c);
        break;
      case 'k':
        display.drawBitmap(x+8*letter,y,italic_font[10],7,7,c);
        break;
      case 'l':
        display.drawBitmap(x+8*letter,y,italic_font[11],7,7,c);
        break;
      case 'm':
        display.drawBitmap(x+8*letter,y,italic_font[12],7,7,c);
        break;
      case 'n':
        display.drawBitmap(x+8*letter,y,italic_font[13],7,7,c);
        break;
      case 'o':
        display.drawBitmap(x+8*letter,y,italic_font[14],7,7,c);
        break;
      case 'p':
        display.drawBitmap(x+8*letter,y,italic_font[15],7,7,c);
        break;
      case 'q':
        display.drawBitmap(x+8*letter,y,italic_font[16],7,7,c);
        break;
      case 'r':
        display.drawBitmap(x+8*letter,y,italic_font[17],7,7,c);
        break;
      case 's':
        display.drawBitmap(x+8*letter,y,italic_font[18],7,7,c);
        break;
      case 't':
        display.drawBitmap(x+8*letter,y,italic_font[19],7,7,c);
        break;
      case 'u':
        display.drawBitmap(x+8*letter,y,italic_font[20],7,7,c);
        break;
      case 'v':
        display.drawBitmap(x+8*letter,y,italic_font[21],7,7,c);
        break;
      case 'w':
        display.drawBitmap(x+8*letter,y,italic_font[22],7,7,c);
        break;
      case 'x':
        display.drawBitmap(x+8*letter,y,italic_font[23],7,7,c);
        break;
      case 'y':
        display.drawBitmap(x+8*letter,y,italic_font[24],7,7,c);
        break;
      case 'z':
        display.drawBitmap(x+8*letter,y,italic_font[25],7,7,c);
        break;
      case '/':
        display.drawBitmap(x+8*letter,y,italic_font[26],7,7,c);
        break;
    }
  }
}

void printItalic_wiggly(int x, int y, String text, uint16_t c){
  for(uint8_t letter = 0; letter<text.length(); letter++){
    unsigned char character = text.charAt(letter);
    int8_t offset = 3*sin(float(millis())/float(200)+float(letter));
    printItalic(x,y+offset,character,c);
    x+=8;
  }
}
