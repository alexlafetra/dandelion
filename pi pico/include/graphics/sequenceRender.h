struct SequenceRenderSettings{
    uint16_t start;
    uint16_t end;
    uint8_t startHeight;
    uint8_t maxTracksShown = 6;
    bool onlyWithinLoop;
    bool drawLoopFlags;
    bool drawLoopPoints;

    bool drawSteps = true;

    bool trackLabels;
    bool topLabels;
    bool trackSelection = false;
    bool shadeOutsideLoop = false;

    bool shrinkTopDisplay = false;
    bool drawPram = true;
    bool drawCursor = true;
    bool displayingVel = true;
    bool drawTrackChannel = false;
    bool drawSuperposition = false;
    bool stepSequencerLEDs = true;//controls whether or not drawSeq sets the LEDs to display notes

    SequenceRenderSettings(){
        shadeOutsideLoop = false;
        onlyWithinLoop = false;
        start = sequence.viewStart;
        end = sequence.viewEnd;
        trackLabels = true;
        topLabels = true;
        drawLoopPoints = true;
        trackSelection = false;
        shrinkTopDisplay = sequence.shrinkTopDisplay;
        maxTracksShown = 6;
        startHeight = headerHeight;
        drawLoopFlags = true;
        drawTrackChannel = controls.SHIFT();
        makeViewInBounds();
    }

    void makeViewInBounds(){
        if(end>sequence.sequenceLength){
            end = sequence.sequenceLength;
        }
        if(start>end){
            start = 0;
        }
    }
    uint16_t getViewLength(){
        return end-start;
    }
    void updateView(){
        start = sequence.viewStart;
        end = sequence.viewEnd;
    }
};

void SelectionBox::render(SequenceRenderSettings& settings){
  coords.end = Coordinate(sequence.cursorPos,sequence.activeTrack);

  unsigned short int startX;
  unsigned short int startY;
  unsigned short int len;
  unsigned short int height;

  unsigned short int X1;
  unsigned short int X2;
  unsigned short int Y1;
  unsigned short int Y2;

  if(this->coords.start.x>this->coords.end.x){
    X1 = this->coords.end.x;
    X2 = this->coords.start.x;
  }
  else{
    X1 = this->coords.start.x;
    X2 = this->coords.end.x;
  }
  if(this->coords.start.y>this->coords.end.y){
    Y1 = this->coords.end.y;
    Y2 = this->coords.start.y;
  }
  else{
    Y1 = this->coords.start.y;
    Y2 = this->coords.end.y;
  }

  startX = trackDisplay+(X1-sequence.viewStart)*sequence.viewScale;
  len = (X2-X1)*sequence.viewScale;

  //if box starts before view
  if(X1<sequence.viewStart){
    startX = trackDisplay;//box is drawn from beggining, to this->coords.end.x
    len = (X2-sequence.viewStart)*sequence.viewScale;
  }
  //if box ends past view
  if(X2>sequence.viewEnd){
    len = (sequence.viewEnd-X1)*sequence.viewScale;
  }

  //same, but for tracks
  startY = (Y1-sequence.startTrack)*trackHeight+settings.startHeight;
  height = ((Y2+1-sequence.startTrack)*trackHeight)+settings.startHeight - startY;
  if(Y1<sequence.startTrack){
    startY = settings.startHeight;
    height = ((Y2 - sequence.startTrack + 1)*trackHeight - startY)%(screenHeight-settings.startHeight) + settings.startHeight;
  }
  display.drawRect(startX, startY, len, height, SSD1306_WHITE);
  display.drawRect(startX+1, startY+1, len-2, height-2, SSD1306_WHITE);

  if(len>5 && height>=trackHeight){
    display.fillRect(startX+2,startY+2, len-4, height-4, SSD1306_BLACK);
    graphics.shadeArea(startX+2,startY+2, len-4, height-4,5);
  }
}

//Start = step you're starting on, startheight is the y coord the sequence grid begins at
void drawSeqBackground(SequenceRenderSettings& settings, uint8_t height){
  //drawing the measure bars
  for (uint16_t step = settings.start; step < settings.end; step++) {
    unsigned short int x1 = trackDisplay+int((step-settings.start)*sequence.viewScale);
//    unsigned short int x2 = x1 + (step-settings.start)*sequence.viewScale;

    //shade everything outside the loop
    if(settings.shadeOutsideLoop){
      if(step<sequence.loopData[sequence.activeLoop].start){
        graphics.shadeArea(x1,settings.startHeight,(sequence.loopData[sequence.activeLoop].start-step)*sequence.viewScale,screenHeight-settings.startHeight,3);
        step = sequence.loopData[sequence.activeLoop].start;
        //ok, step shouldn't ever be zero in this case, since that would mean it was LESS than zero to begin
        //with. But, just for thoroughnesses sake, make sure step doesn't overflow when you subtract from it
        if(step != 0){
          step--;
        }
        continue;
      }
      else if(step>sequence.loopData[sequence.activeLoop].end){
        graphics.shadeArea(x1,settings.startHeight,(sequence.viewEnd-sequence.loopData[sequence.activeLoop].end)*sequence.viewScale,screenHeight-settings.startHeight,3);
        break;
      }
    }

    //if the last track is showing
    if(sequence.endTrack == sequence.trackData.size()){
      //measure bars
      if (!(step % sequence.subDivision) && (step%96) && (sequence.subDivision*sequence.viewScale)>1) {
        graphics.drawDottedLineV(x1,settings.startHeight,height,2);
      }
      if(!(step%96)){
        graphics.drawDottedLineV2(x1,settings.startHeight,height,6);
      }
    }
    else{
      //measure bars
      if (!(step % sequence.subDivision) && (step%96) && (sequence.subDivision*sequence.viewScale)>1) {
        graphics.drawDottedLineV(x1,settings.startHeight,height,2);
      }
      if(!(step%96)){
        graphics.drawDottedLineV2(x1,settings.startHeight,height,6);
      }
    }

    //drawing loop points/flags
    if(settings.drawLoopPoints){//check
      if(step == sequence.loopData[sequence.activeLoop].start){
        if(settings.drawLoopFlags){
          if(sequence.movingLoop == MOVING_LOOP_END || sequence.movingLoop == MOVING_BOTH_LOOP_POINTS){
            display.fillTriangle(trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-3-sin(millis()/50), trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-7-sin(millis()/50), trackDisplay+(step-settings.start)*sequence.viewScale+4, settings.startHeight-7-sin(millis()/50),SSD1306_WHITE);
            display.drawFastVLine(trackDisplay+(step-settings.start)*sequence.viewScale,settings.startHeight-3,3,SSD1306_WHITE);
          }
          else{
            if(sequence.cursorPos == step){
              display.fillTriangle(trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-3, trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-7, trackDisplay+(step-settings.start)*sequence.viewScale+4, settings.startHeight-7,SSD1306_WHITE);
              display.drawFastVLine(trackDisplay+(step-settings.start)*sequence.viewScale,settings.startHeight-3,3,SSD1306_WHITE);
            }
            else{
              display.fillTriangle(trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-1, trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-5, trackDisplay+(step-settings.start)*sequence.viewScale+4, settings.startHeight-5,SSD1306_WHITE);
            }
          }
        }
        if((sequence.movingLoop == MOVING_NO_LOOP_POINTS) || (sequence.movingLoop != MOVING_LOOP_START && (millis()/400)%2)){
          display.drawFastVLine(trackDisplay+(step-settings.start)*sequence.viewScale,settings.startHeight,screenHeight-settings.startHeight-(sequence.endTrack == sequence.trackData.size()),SSD1306_WHITE);
          display.drawFastVLine(trackDisplay+(step-settings.start)*sequence.viewScale-1,settings.startHeight,screenHeight-settings.startHeight-(sequence.endTrack == sequence.trackData.size()),SSD1306_WHITE);
        }
      }
      if(step == sequence.loopData[sequence.activeLoop].end-1){
        if(settings.drawLoopFlags){
          if(sequence.movingLoop == MOVING_LOOP_START || sequence.movingLoop == MOVING_BOTH_LOOP_POINTS){
            display.drawTriangle(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale, settings.startHeight-3-sin(millis()/50), trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale-4, settings.startHeight-7-sin(millis()/50), trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale, settings.startHeight-7-sin(millis()/50),SSD1306_WHITE);
            display.drawFastVLine(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale,settings.startHeight-3,3,SSD1306_WHITE);
          }
          else{
            if(sequence.cursorPos == step+1){
              display.drawTriangle(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale, settings.startHeight-3, trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale-4, settings.startHeight-7, trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale, settings.startHeight-7,SSD1306_WHITE);
              display.drawFastVLine(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale,settings.startHeight-3,3,SSD1306_WHITE);
            }
            else{
              display.drawTriangle(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale, settings.startHeight-1, trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale-4, settings.startHeight-5, trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale, settings.startHeight-5,SSD1306_WHITE);
            }
          }
        }
        if((sequence.movingLoop == MOVING_NO_LOOP_POINTS) || (sequence.movingLoop != MOVING_LOOP_END && (millis()/400)%2)){
          display.drawFastVLine(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale+1,settings.startHeight,screenHeight-settings.startHeight-(sequence.endTrack == sequence.trackData.size()),SSD1306_WHITE);
          display.drawFastVLine(trackDisplay+(sequence.loopData[sequence.activeLoop].end-settings.start)*sequence.viewScale+2,settings.startHeight,screenHeight-settings.startHeight-(sequence.endTrack == sequence.trackData.size()),SSD1306_WHITE);
        }
      }
      if(sequence.movingLoop == MOVING_BOTH_LOOP_POINTS){
        if(step>sequence.loopData[sequence.activeLoop].start && step<sequence.loopData[sequence.activeLoop].end && settings.startHeight>8){
          display.drawPixel(trackDisplay+(step-settings.start)*sequence.viewScale, settings.startHeight-7-sin(millis()/50),SSD1306_WHITE);
        }
      }
      if(settings.drawLoopFlags && (step == sequence.loopData[sequence.activeLoop].start+(sequence.loopData[sequence.activeLoop].end-sequence.loopData[sequence.activeLoop].start)/2))
        printSmall(trackDisplay+(step-settings.start)*sequence.viewScale-1,settings.startHeight-7,stringify(sequence.activeLoop),SSD1306_WHITE);
    }
  }
}

//draws pram, other icons (not loop points tho)
void drawTopIcons(SequenceRenderSettings& settings){
  //note presence indicator(if notes are offscreen)
  if(sequence.areThereMoreNotes(true)){
    uint8_t y1 = settings.shrinkTopDisplay?8:headerHeight;
    if(millis()%1000>500){
      display.fillTriangle(trackDisplay-7,y1+3,trackDisplay-3,y1+3,trackDisplay-5,y1+1, SSD1306_WHITE);
    }
    else{
      display.fillTriangle(trackDisplay-7,y1+2,trackDisplay-3,y1+2,trackDisplay-5,y1, SSD1306_WHITE);
    }
  }
  if(sequence.areThereMoreNotes(false)){
    if(millis()%1000>500){
      display.fillTriangle(trackDisplay-7,screenHeight-5,trackDisplay-3,screenHeight-5,trackDisplay-5,screenHeight-3, SSD1306_WHITE);
    }
    else{
      display.fillTriangle(trackDisplay-7,screenHeight-4,trackDisplay-3,screenHeight-4,trackDisplay-5,screenHeight-2, SSD1306_WHITE);
    }
  }
  uint8_t x1 = 32;
  //rec/play icon
  if(sequence.recording()){
    if(sequenceClock.clockSource == EXTERNAL_CLOCK && !sequenceClock.receivedClockMessage){
      if(waitingToReceiveANote){
        if(millis()%1000>500){
          display.drawCircle(trackDisplay+3,3,3,SSD1306_WHITE);
        }
      }
      else
        display.drawCircle(trackDisplay+3,3,3,SSD1306_WHITE);
    }
    else if((sequenceClock.clockSource == EXTERNAL_CLOCK && sequenceClock.receivedClockMessage) || sequenceClock.clockSource == INTERNAL_CLOCK){
      if(waitingToReceiveANote){
        if(millis()%1000>500){
          display.fillCircle(trackDisplay+3,3,3,SSD1306_WHITE);
        }
      }
      else
        display.fillCircle(trackDisplay+3,3,3,SSD1306_WHITE);
    }
    x1+=9;
    switch(sequence.recMode){
      //if one-shot rec
      case ONESHOT:
        printSmall(x1,1,"1",1);
        x1+=4;
        if((millis()/10)%100>50)
          display.drawBitmap(x1,1,caution_bmp,3,5,SSD1306_WHITE);
        x1+=4;
        break;
      //if continuous recording
      case CURRENT_LOOP:
        display.drawBitmap(x1,1,continuous_bmp,9,5,SSD1306_WHITE);
        x1+=10;
        break;
      case LOOP_SEQUENCE:
        break;
    }
    if(overwriteRecording){
      display.drawBitmap(x1,0,((millis()/10)%100>50)?overwrite_1:overwrite_2,7,7,SSD1306_WHITE);
      x1+=8;
    }
  }
  else if(sequence.playing()){
    graphics.drawPlayIcon(trackDisplay+((millis()/200)%2)+1,0);
    x1 += 10;
  }
  //draw a caution sign if the queue is overloaded
  if(MIDI.queueIsFull){
    display.drawBitmap(x1,1,caution_bmp,3,5,SSD1306_WHITE);
    x1+=4;
  }
  //Data track icon
  if(sequence.autotrackData.size()>0){
    if(millis()%1600>800)
      display.drawBitmap(x1,0,autotrack1,10,7,SSD1306_WHITE);
    else{
      display.drawBitmap(x1,0,autotrack2,10,7,SSD1306_WHITE);
    }
    x1+=12;
  }

  //fragment gem
  if(isFragmenting){
    graphics.drawTetra(x1,5,10+sin(float(millis())/float(500)),10+sin(float(millis())/float(500)),6+sin(float(millis())/float(500)),1+sin(float(millis())/float(500)),0,SSD1306_WHITE);
    x1+=8;
  }

  //velocity/chance indicator while shifting
  if(controls.SHIFT()){
    if(settings.displayingVel){
      if(sequence.IDAtCursor() == 0){
        String vel = stringify(sequence.defaultVel);
        printSmall(x1,1,"v:"+vel,SSD1306_WHITE);
        x1+=vel.length()*4+8;;
      }
      else{
        String vel = stringify(sequence.noteData[sequence.activeTrack][sequence.IDAtCursor()].velocity);
        printSmall(x1,1,"v:"+vel,SSD1306_WHITE);
        x1+=vel.length()*4+8;
      }
    }
    else{
      if(sequence.IDAtCursor() == 0){
        printSmall(x1,1,"c:"+stringify(sequence.defaultChance),SSD1306_WHITE);
        x1+=20;
      }
      else{
        printSmall(x1,1,"c:"+stringify(sequence.noteData[sequence.activeTrack][sequence.IDAtCursor()].chance),SSD1306_WHITE);
        x1+=20;
      }
    }
  }
  else{
    //power/battery indicator
    graphics.drawPower(screenWidth-10,0);
  }
  if(arp.isActive){
    display.drawPixel(x1,3+2*sin(float(millis())/float(200)),1);
    display.drawPixel(x1+2,3+2*sin(float(millis())/float(200)+100),1);
    display.drawPixel(x1+4,3+2*sin(float(millis())/float(200)+200),1);
    display.drawBitmap(x1+6,2,tiny_arp_bmp,9,3,1);
    x1+=17;
  }
  if(controls.SHIFT()){
    graphics.drawSequenceMemoryBar(screenWidth-30,0,30);
  }
  else{
    //draw menu text
    printSmall(x1,1,menuText,SSD1306_WHITE);
    x1+=menuText.length()*4+2;
  }
}

uint8_t getVelShade(uint8_t vel){
  int8_t shade = 13-(vel/10);
  if(shade<=0)
    return 1;
  else
    return shade;
}

int8_t getChanceShade(uint8_t odds){
  int8_t shade = 11-(odds/10);
  if(shade<=0)
    return 1;
  else
    return shade;
}

NoteCoords getNoteCoords(Note& note, uint8_t track, SequenceRenderSettings& settings){
  NoteCoords nCoords;
  if(note.startPos>settings.start){
    nCoords.length = (note.endPos - note.startPos)*sequence.viewScale;
    nCoords.x1 = trackDisplay+int16_t((note.startPos-settings.start)*sequence.viewScale);
  }
  else{
    nCoords.length = (note.endPos - settings.start)*sequence.viewScale+1;
    nCoords.x1 = trackDisplay-1;
  }
  nCoords.y1 = (track-sequence.startTrack) * trackHeight + settings.startHeight;
  nCoords.y2 = nCoords.y1+trackHeight;
  return nCoords;
}

NoteCoords getNoteCoords(Note& note, uint8_t track){
  SequenceRenderSettings settings;
  return getNoteCoords(note,track,settings);
}

void drawNoteSprite(NoteCoords& noteCoords, uint8_t shade){
  if(noteCoords.y1>=SCREEN_HEIGHT)
    return;
  if(noteCoords.x1>=SCREEN_WIDTH)
    return;
  if(shade != 1 && shade){//so it does this faster
    display.fillRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_BLACK);//clearing out the note area
    for(uint8_t j = 1; j<trackHeight-2; j++){//shading the note...
      for(uint8_t i = noteCoords.x1+1;i+j%shade<noteCoords.x1+noteCoords.length-1; i+=shade){
        display.drawPixel(i+j%shade,noteCoords.y1+j,SSD1306_WHITE);
      }
    }
    display.drawRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_WHITE);
  }
  //if it's a solid note, fill it quickly
  else if(shade == 1){
    display.fillRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_WHITE);
  }
  else if(!shade){
    display.fillRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_BLACK);//clearing out the note area
    display.drawRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_WHITE);
  }
}

void drawNotePreviewOutline(Note& note, uint8_t track, NoteCoords noteCoords, SequenceRenderSettings& settings){
  if(noteCoords.y1>=SCREEN_HEIGHT)
    return;
  if(noteCoords.x1>=SCREEN_WIDTH)
    return;
  //if the noteCoords.length is less than 3, don't worry about shading it
  if(noteCoords.length<3){
    display.fillRect(noteCoords.x1, noteCoords.y1+1, noteCoords.length+2, trackHeight-2, SSD1306_WHITE);
  }
  else{
    // graphics.drawDottedRect(noteCoords.x1,noteCoords.y1,noteCoords.length,trackHeight-2,2);
    graphics.shadeArea(noteCoords.x1,noteCoords.y1+1,noteCoords.length,trackHeight-2,2);
  }
}

void drawNotePreviewOutline(Note& note, uint8_t track, SequenceRenderSettings& settings){
  drawNotePreviewOutline(note, track, getNoteCoords(note,track,settings), settings);
}

void drawNote(Note& note, uint8_t track, NoteCoords noteCoords, SequenceRenderSettings& settings){
  
  //if the note is currently superpositioned, draw it where it should be, but not if it's out of view
  if(note.isSuperpositioned()){
    noteCoords.offsetY(sequence.trackData[track].pitch - note.superposition.pitch);
  }
  if(noteCoords.y1>=SCREEN_HEIGHT)
    return;
  if(noteCoords.x1>=SCREEN_WIDTH)
    return;
  //if it's not actively in superposition, BUT it has one and the cursor is over it, draw a rounded rect behind it
  if(!note.isSuperpositioned() && note.superposition.pitch != 255 && sequence.cursorPos<note.endPos && sequence.cursorPos >= note.startPos && sequence.activeTrack == track){
    int8_t offset = note.superposition.pitch-sequence.trackData[track].pitch;
    if(sequence.cursorPos == note.startPos){
        offset += (millis()/100)%2;
    }
    graphics.fillRectWithMissingCorners(noteCoords.x1+((abs(offset)>5)?5:offset), noteCoords.y1-offset, noteCoords.length, trackHeight, SSD1306_BLACK);
    graphics.drawRectWithMissingCorners(noteCoords.x1+((abs(offset)>5)?5:offset), noteCoords.y1-offset, noteCoords.length, trackHeight, SSD1306_WHITE);
  }
  uint8_t shade = settings.displayingVel?getVelShade(note.velocity):getChanceShade(note.chance);
  //if the noteCoords.length is less than 3, don't worry about shading it
  if(noteCoords.length<3){
    display.fillRect(noteCoords.x1, noteCoords.y1+1, noteCoords.length+2, trackHeight-2, SSD1306_WHITE);
  }
  else{
    if(note.isMuted()){
      display.fillRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_BLACK);
      display.drawRect(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_WHITE);
      display.drawLine(noteCoords.x1+1,noteCoords.y1+1, noteCoords.x1+noteCoords.length-1, noteCoords.y1+trackHeight-2,SSD1306_WHITE);
      display.drawLine(noteCoords.x1+1,noteCoords.y1+trackHeight-2,noteCoords.x1+noteCoords.length-1,noteCoords.y1+1,SSD1306_WHITE);
      display.drawFastVLine(noteCoords.x1+noteCoords.length,noteCoords.y1+1,trackHeight-2,SSD1306_BLACK);
    }
    else if(note.isSuperpositioned()){
      graphics.fillRectWithMissingCorners(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_BLACK);
      graphics.drawRectWithMissingCorners(noteCoords.x1+1, noteCoords.y1+1, noteCoords.length-1, trackHeight-2, SSD1306_WHITE);
    }
    else{
      drawNoteSprite(noteCoords,shade);
      //line at the end, if there's something at the end
      if(sequence.lookupTable[track][note.endPos] != 0)
        display.drawFastVLine(noteCoords.x1+noteCoords.length,noteCoords.y1+1,trackHeight-2,SSD1306_BLACK);
    }
    if(note.isSelected()){
      display.drawRect(noteCoords.x1,noteCoords.y1+1,noteCoords.length,trackHeight-2,SSD1306_BLACK);
      display.drawRect(noteCoords.x1+2,noteCoords.y1+2,noteCoords.length-3,trackHeight-4,SSD1306_WHITE);
      display.drawRect(noteCoords.x1,noteCoords.y1,noteCoords.length+1,trackHeight,SSD1306_WHITE);
      display.drawRect(noteCoords.x1+1,noteCoords.y1+1,noteCoords.length-1,trackHeight-2,SSD1306_BLACK);
    }
  }
}
void drawNote(Note& note, uint8_t track, SequenceRenderSettings& settings){
  drawNote(note, track, getNoteCoords(note,track,settings), settings);
}

void drawPramIcon(SequenceRenderSettings& settings){
  if(settings.shrinkTopDisplay){
      graphics.drawTinyPram(liveLoop.active?4:5,0);
  }
  else{
      graphics.drawBigPram(liveLoop.active?4:5,0);
  }
  //music symbol while receiving notes
  if(sequence.isReceivingOrSending()){
    display.drawChar(settings.shrinkTopDisplay?13:19+(liveLoop.active?0:1),(millis()/200)%2,0x0E,SSD1306_WHITE,SSD1306_BLACK,1);
  }
  if(liveLoop.active){
    if(settings.shrinkTopDisplay){
      display.fillCircle(20,3,2,1);
    }
    else{
      display.fillCircle(21,11,2,1);
    }
  }
}

//this function is a mess! especially the shrinktopdisplay logic
void drawSeq(SequenceRenderSettings& settings){

  //loading in the settings for the shrunk top display
  if(settings.shrinkTopDisplay){
    settings.startHeight = 8;
    settings.drawLoopFlags = false;
    settings.maxTracksShown = 6;
  }
  //calc track height
  trackHeight = (screenHeight-settings.startHeight)/settings.maxTracksShown;

  //if there are more tracks than you're going to draw (typically the case), figure out which to draw
  if(sequence.trackData.size()>settings.maxTracksShown){
    //if you're already looking at the last track, lower your start track
    if(sequence.endTrack == sequence.trackData.size()){
        sequence.startTrack = sequence.endTrack-settings.maxTracksShown;
    }
    //if not, then increase your end track
    else{
        sequence.endTrack = sequence.startTrack + settings.maxTracksShown;
    }
    //if the active track is beyond the end track
    if(sequence.activeTrack>=sequence.endTrack){
      sequence.endTrack = sequence.activeTrack;
      sequence.startTrack = sequence.endTrack-settings.maxTracksShown+1;
    }
    else if(sequence.activeTrack<sequence.startTrack){
      sequence.startTrack = sequence.activeTrack;
      sequence.endTrack = sequence.startTrack+settings.maxTracksShown;
    }
  }
  else{
      sequence.endTrack = sequence.startTrack + sequence.trackData.size();
  }
  //drawing selection box, since it needs to overlay stepSeq data
  if(selBox.begun){
      selBox.render(settings);
  }
  //calculating height
  uint8_t height = settings.startHeight+trackHeight*settings.maxTracksShown;
  // if(sequence.endTrack == sequence.trackData.size())
  //     height = settings.startHeight+trackHeight*settings.maxTracksShown;
  // else if(sequence.trackData.size()>settings.maxTracksShown)
  //     height = settings.startHeight+trackHeight*(settings.maxTracksShown+1);
  // else
  //     height = settings.startHeight+trackHeight*sequence.trackData.size();

  //drawing measure bars, loop points
  drawSeqBackground(settings, height);

  //drawing cursor
  if(settings.drawCursor){
      uint8_t cPos = trackDisplay+uint8_t((sequence.cursorPos-settings.start)*sequence.viewScale);
      if(cPos>127)
          cPos = 126;
      if(sequence.endTrack == sequence.trackData.size()){
          display.drawFastVLine(cPos, settings.startHeight, height, SSD1306_WHITE);
          display.drawFastVLine(cPos+1, settings.startHeight, height, SSD1306_WHITE);
      }
      else{
          display.drawFastVLine(cPos, settings.startHeight, screenHeight-settings.startHeight, SSD1306_WHITE);
          display.drawFastVLine(cPos+1, settings.startHeight, screenHeight-settings.startHeight, SSD1306_WHITE);
      }
  }

  //drawing active track highlight
  uint8_t y1 = (sequence.activeTrack-sequence.startTrack) * trackHeight + settings.startHeight;
  // display.drawRect(x1, y1, screenWidth, trackHeight, SSD1306_WHITE);
  display.drawFastHLine(trackDisplay,y1,screenWidth-trackDisplay,1);
  display.drawFastHLine(trackDisplay,y1+trackHeight-1,screenWidth-trackDisplay,1);

  //top and bottom bounds
  if(sequence.startTrack == 0){
    display.drawFastHLine(trackDisplay,settings.startHeight,screenWidth,SSD1306_WHITE);
  }
  //if the bottom is in view
  if(sequence.endTrack == sequence.trackData.size()){
    display.drawFastHLine(trackDisplay,settings.startHeight+trackHeight*settings.maxTracksShown,screenWidth,SSD1306_WHITE);
  }
  //tiny trk label
  // display.setRotation(DISPLAY_SIDEWAYS_R);
  // display.drawBitmap(settings.startHeight,125,trk_tiny,10,3,1);
  // display.setRotation(DISPLAY_UPRIGHT);


  //drawin all da steps
  //---------------------------------------------------
  for(uint8_t track = sequence.startTrack; track < sequence.endTrack; track++) {
      unsigned short int y1 = (track-sequence.startTrack) * trackHeight + settings.startHeight;
      uint8_t xCoord = 5;
      //track info display
      if(sequence.activeTrack == track){
          xCoord = 9;
          if(settings.trackLabels)
              graphics.drawArrow(6+((millis()/400)%2),y1+trackHeight/2+1,2,0,true);
      }
      if(settings.trackLabels){
          //printing note names
          if(sequence.pitchesOrNumbers){
              graphics.printTrackPitch(xCoord, y1+trackHeight/2-2,track,false,settings.drawTrackChannel,SSD1306_WHITE);
          }
          //just printing pitch numbers
          else{
              display.setCursor(xCoord,y1+2);
              display.print(sequence.trackData[track].pitch);
          }
      }
      //if you're drawing selected tracks highlight
      if(settings.trackSelection){
        //if this track isn't selected, shade it
        if(!sequence.trackData[track].isSelected()){
          display.fillRect(trackDisplay,y1,screenWidth-trackDisplay,trackHeight,0);
          graphics.shadeArea(trackDisplay,y1,screenWidth-trackDisplay,trackHeight,3);
          continue;
        }
        else{
          display.fillRect(0,y1+1,trackDisplay,trackHeight,2);
        }
      }
      //if the track is muted, just hatch it out (don't draw any notes)
      //if it's solo'd and muted, draw it normal (solo overrules mute)
      else if(sequence.trackData[track].isMuted() && !sequence.trackData[track].isSolo()){
          graphics.shadeArea(trackDisplay,y1,screenWidth-trackDisplay,trackHeight,9);
          continue;
      }
      else if(settings.drawSteps){
          //Check to see if you only want to render the region in the active loop, and shade everything else!
          for (uint16_t step = settings.shadeOutsideLoop?sequence.loopData[sequence.activeLoop].start:settings.start; step < (settings.shadeOutsideLoop?sequence.loopData[sequence.activeLoop].end:settings.end); step++) {
              uint16_t id = sequence.lookupTable[track][step];
              //drawing note
              if (id != 0){
                Note n = sequence.noteData[track][id];
                drawNote(n,track,settings);
                step = n.endPos;//skip to the end of the note
              }
          }
      }
  }
  //all the top icons/tooltips
  if(settings.topLabels){
    drawTopIcons(settings);
  }
  //drawing big or small pram in the corner
  if(settings.drawPram){
    drawPramIcon(settings);
  }
  //playhead/rechead
  if(sequence.playing() && sequence.isInView(sequence.playheadPos))
      display.drawRoundRect(trackDisplay+(sequence.playheadPos-settings.start)*sequence.viewScale,settings.startHeight,3, screenHeight-settings.startHeight, 3, SSD1306_WHITE);
  if(sequence.recording() && sequence.isInView(sequence.recheadPos))
      display.drawRoundRect(trackDisplay+(sequence.recheadPos-settings.start)*sequence.viewScale,settings.startHeight,3, screenHeight-settings.startHeight, 3, SSD1306_WHITE);

  int cursorX = trackDisplay+int((sequence.cursorPos-settings.start)*sequence.viewScale)-8;
  if(!sequence.playing() && !sequence.recording()){
      cursorX = 32;
  }
  else{
      //making sure it doesn't print over the subdiv info
      cursorX = 42;
  }


  //it's ok to call this in here bc the LB checks to make sure it doesn't redundantly write
  if(settings.stepSequencerLEDs)
    sequence.displayMainSequenceLEDs();
}

void drawSeq(){
  SequenceRenderSettings settings;
  drawSeq(settings);
}

void drawNoteBracket(NoteCoords& n, bool animated){
  uint8_t offset = animated?((millis()/400)%2):0;
  n.x1++;
  n.y1++;
  n.length-=2;
  //topL
  display.drawLine(n.x1-2-offset,n.y1-2-offset,n.x1+1-offset,n.y1-2-offset,SSD1306_WHITE);
  display.drawLine(n.x1-2-offset,n.y1-2-offset,n.x1-2-offset,n.y1+1-offset,SSD1306_WHITE);
  //topR
  display.drawLine(n.x1+n.length+2+offset,n.y1-2-offset,n.x1+n.length-1+offset,n.y1-2-offset,SSD1306_WHITE);
  display.drawLine(n.x1+n.length+2+offset,n.y1-2-offset,n.x1+n.length+2+offset,n.y1+1-offset,SSD1306_WHITE);
  //bottomL
  display.drawLine(n.x1-2-offset,n.y2+offset,n.x1+1-offset,n.y2+offset,SSD1306_WHITE);
  display.drawLine(n.x1-2-offset,n.y2+offset,n.x1-2-offset,n.y2-3+offset,SSD1306_WHITE);
  //bottomR
  display.drawLine(n.x1+n.length+2+offset,n.y2+offset,n.x1+n.length-1+offset,n.y2+offset,SSD1306_WHITE);
  display.drawLine(n.x1+n.length+2+offset,n.y2+offset,n.x1+n.length+2+offset,n.y2-3+offset,SSD1306_WHITE);
}

void drawNoteBracket(Note& note, uint8_t track, SequenceRenderSettings& settings){
  NoteCoords n = getNoteCoords(note,track,settings);
  drawNoteBracket(n,true);
}


void drawSelectionBracket(SequenceRenderSettings& settings){
    vector<uint16_t> bounds  = getSelectedNotesBoundingBox();
    //if the left side is in view
    if(bounds[0]>=sequence.viewStart){
      //if the top L corner is in view
      uint8_t x1 = (bounds[0]-sequence.viewStart)*sequence.viewScale+trackDisplay-((millis()/200)%2);
      if(bounds[1]>=sequence.startTrack){
        //y coord relative to the view
        uint8_t y1 = (bounds[1]-sequence.startTrack)*trackHeight+settings.startHeight-((millis()/200)%2);
        display.drawLine(x1,y1,x1+5,y1,SSD1306_WHITE);
        display.drawLine(x1,y1-1,x1+5,y1-1,SSD1306_WHITE);
        display.drawLine(x1,y1,x1,y1+5,SSD1306_WHITE);
        display.drawLine(x1-1,y1,x1-1,y1+5,SSD1306_WHITE);
      }
      //if the bottom L corner is in view
      if(bounds[3]<=sequence.endTrack){
        //y coord relative to the view
        uint8_t y1 = (bounds[3]-sequence.startTrack+1)*trackHeight+settings.startHeight+((millis()/200)%2);
        display.drawLine(x1,y1,x1+5,y1,SSD1306_WHITE);
        display.drawLine(x1,y1+1,x1+5,y1+1,SSD1306_WHITE);
        display.drawLine(x1,y1,x1,y1-5,SSD1306_WHITE);
        display.drawLine(x1-1,y1,x1-1,y1-5,SSD1306_WHITE);
      }
    }
    //if the right corner is in view
    if(bounds[2]<sequence.viewEnd){
      uint8_t x1 = (bounds[2]-sequence.viewStart)*sequence.viewScale+trackDisplay+((millis()/200)%2)+1;
      //top R corner
      if(bounds[1]>=sequence.startTrack){
        uint8_t y1 = (bounds[1]-sequence.startTrack)*trackHeight+settings.startHeight-((millis()/200)%2);
        display.drawLine(x1,y1,x1-5,y1,SSD1306_WHITE);
        display.drawLine(x1,y1-1,x1-5,y1-1,SSD1306_WHITE);
        display.drawLine(x1,y1,x1,y1+5,SSD1306_WHITE);
        display.drawLine(x1+1,y1,x1+1,y1+5,SSD1306_WHITE);
      }
      //bottom R corner
      if(bounds[3]<=sequence.endTrack){
        uint8_t y1 = (bounds[3]-sequence.startTrack+1)*trackHeight+settings.startHeight+((millis()/200)%2);
        display.drawLine(x1,y1,x1-5,y1,SSD1306_WHITE);
        display.drawLine(x1,y1+1,x1-5,y1+1,SSD1306_WHITE);
        display.drawLine(x1,y1,x1,y1-5,SSD1306_WHITE);
        display.drawLine(x1+1,y1,x1+1,y1-5,SSD1306_WHITE);
      }
    }
  }
  void drawSelectionBracket(){
    SequenceRenderSettings settings;
    drawSelectionBracket(settings);
  }
