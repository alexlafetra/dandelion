/*
  Scrollable viewer for debugging a wireframe design
*/
void viewWireFrame(WireFrame& w){
  display.setTextColor(SSD1306_WHITE);
  while(true){
    controls.readJoystick();
    controls.readButtons();
    if(utils.itsbeen(10)){
      if(controls.joystickX != 0){
        //pan while shifting
        if(controls.SHIFT()){
          if(controls.joystickX == 1){
            w.offset.x--;
          }
          else if(controls.joystickX == -1){
            w.offset.x++;
          }
          lastTime = millis();
        }
        //rotate while not shifting
        else{
          if(controls.joystickX == 1){
            w.rotate(-3,1);
          }
          else if(controls.joystickX == -1){
            w.rotate(3,1);
          }
        }
        lastTime = millis();
      }
      if(controls.joystickY != 0){
        if(controls.joystickY == 1){
          w.rotate(-3,0);
        }
        else if(controls.joystickY == -1){
          w.rotate(3,0);
        }
        lastTime = millis();
      }
    }
    if(utils.itsbeen(200)){
      if(controls.MENU()){
        lastTime = millis();
        return;
      }
    }
    // w.rotate(1,1);
    display.clearDisplay();
    w.render();
    display.display();
  }
}
void animateMonitor(WireFrame& wireframe, float mag, uint8_t yCoord){
  wireframe.offset.y = yCoord+mag*sin(millis()/600.0);
  wireframe.rotate(PI/8.0*sin(millis()/400),1);
}
vector<WireFrame> animateRotation(vector<WireFrame> w, float angle, uint8_t speed, uint8_t axis, float xA, float yA, float zA, WireFrame frame){
  float animAngle = angle/speed;
  uint8_t count = 0;
  vector<WireFrame> newFrames = w;

  while(count<speed){
    for(uint8_t i = 0; i<w.size(); i++){
      w[i].rotate(animAngle, axis);
      newFrames[i] = w[i];
    }
    display.clearDisplay();
    for(uint8_t i = 0; i<w.size(); i++){
      newFrames[i].rotate(xA,0);
      newFrames[i].rotate(yA,1);
      newFrames[i].rotate(zA,2);
      if(i == 1)
        newFrames[i].renderDie();
      else
        newFrames[i].render();
    }
    frame.render();
    display.display();
    count++;
  }
  return w;
}

//used to calculate which side of the die you're on
uint8_t getSide(WireFrame die){
  uint8_t closestV = die.getClosestVert();
  if(closestV == 0)
    return 1;
  else if(closestV <= 2)
    return 2;
  else if(closestV <= 5)
    return 3;
  else if(closestV <= 9)
    return 4;
  else if(closestV <= 14)
    return 5;
  else if(closestV <= 20)
    return 6;
  else
    return 0;
}

WireFrame makeCube(uint8_t size){
  Vertex v1 = Vertex(size/2,size/2,size/2);
  Vertex v2 = Vertex(size/2,size/2,-size/2);
  Vertex v3 = Vertex(size/2,-size/2,size/2);
  Vertex v4 = Vertex(-size/2,size/2,size/2);
  Vertex v5 = Vertex(size/2,-size/2,-size/2);
  Vertex v6 = Vertex(-size/2,size/2,-size/2);
  Vertex v7 = Vertex(-size/2,-size/2,size/2);
  Vertex v8 = Vertex(-size/2,-size/2,-size/2);
  vector<Vertex> vertices = {v1,v2,v3,v4,v5,v6,v7,v8};
  vector<vector<uint8_t>> edges = {{0,1},{0,2},{2,4},{4,1},{2,6},{4,7},{0,3},{1,5},{6,7},{5,3},{6,3},{5,7}};
  return WireFrame(vertices,edges);
}

WireFrame makeDieDots(uint8_t x1, uint8_t y1, uint8_t distance, float scale){

  //1
  Vertex v1 = Vertex(0,0,distance);
  //2
  Vertex v2 = Vertex(-5,distance,-5);
  Vertex v3 = Vertex(5,distance,5);
  //3
  Vertex v4 = Vertex(-distance,-5,-5);
  Vertex v5 = Vertex(-distance,5,5);
  Vertex v6 = Vertex(-distance,0,0);
  //4
  Vertex v7 = Vertex(distance,-5,-5);
  Vertex v8 = Vertex(distance,-5,5);
  Vertex v9 = Vertex(distance,5,-5);
  Vertex v10 = Vertex(distance,5,5);

  //5
  Vertex v11 = Vertex(-5,-distance,-5);
  Vertex v12 = Vertex(5,-distance,-5);
  Vertex v13 = Vertex(0,-distance,0);
  Vertex v14 = Vertex(-5,-distance,5);
  Vertex v15 = Vertex(5,-distance,5);

  //6
  Vertex v16 = Vertex(-5,-5,-distance);
  Vertex v17 = Vertex(5,-5,-distance);
  Vertex v18 = Vertex(-5,0,-distance);
  Vertex v19 = Vertex(5,0,-distance);
  Vertex v20 = Vertex(-5,5,-distance);
  Vertex v21 = Vertex(5,5,-distance);

  vector<Vertex> verts2 = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19,v20,v21};
  WireFrame allDots = WireFrame(verts2);
  allDots.offset.x = x1;
  allDots.offset.y = y1;
  allDots.drawEdges = false;
  allDots.drawDots = true;
  //so it draws all the dots
  for(uint8_t i = 0; i<allDots.verts.size(); i++){
    allDots.dots.push_back(i);
  }
  allDots.scale = scale;
  return allDots;
}

//makes a cube with die dots!
//dots ~are~ accurate
WireFrame genRandMenuObjects(uint8_t x1, uint8_t y1, uint8_t distance, float scale){
  WireFrame cube = makeCube(20);
  cube.offset.x = x1;
  cube.offset.y = y1;
  cube.scale = scale;
  WireFrame allDots = makeDieDots(x1,y1,distance,scale);
  cube.join(allDots);
  cube.drawDots = true;
  return cube;
}

WireFrame makeHalfHouse(float width){
  vector<Vertex> verts = {
    Vertex(-1,0.25,width),
    Vertex(-1,-1,width),
    Vertex(-1.25,-1,width),
    Vertex(0,-2,width),
    Vertex(1.25,-1,width),
    Vertex(1,-1,width),
    Vertex(1,0.25,width)
  };
  vector<vector<uint8_t>> edges;
  for(uint8_t i = 0; i < verts.size()-1; i++){
      edges.push_back({i,static_cast<unsigned char>(i+1)});
  }
  edges.push_back({static_cast<unsigned char>(verts.size()-1),0});
  WireFrame house = WireFrame(verts,edges);
  return house;
}

WireFrame makeHouse(){
  const float width = 3;
  WireFrame house = makeHalfHouse(width/2);
  uint8_t offset = house.verts.size();
  for(uint8_t i = 0; i<house.verts.size(); i++){
      house.edges.push_back({i,static_cast<unsigned short>(i+offset)});
  }
  house.join(makeHalfHouse(-width/2));
  house.offset.x = 64;
  house.offset.y = 40;
  house.scale = 12.0;
  viewWireFrame(house);
  return house;
}

WireFrame genFrame(){
  //frame
  Vertex v1 = Vertex(10,10,25);
  Vertex v2 = Vertex(10,-10,25);
  Vertex v3 = Vertex(-10,10,25);
  Vertex v4 = Vertex(-10,-10,25);

  vector<Vertex> verts3 = {v1,v2,v3,v4};
  vector<vector<uint8_t>> edges2 = {{0,1},{1,3},{2,3},{2,0}};
  WireFrame frame = WireFrame(verts3,edges2);
//  frame.offset.x = 32;
//  frame.offset.y = 32;
    frame.offset.x = 64;
    frame.offset.y = 36;
  frame.rotate(-20,0);
  frame.rotate(20,1);
  frame.rotate(-20,2);
  return frame;
}
WireFrame makeHammer(){
  vector<Vertex> verts = {Vertex(-1,-4,1),
  Vertex(-3,-4,1),
  Vertex(-3,-3.5,1),
  Vertex(-4,-3.5,1),
  Vertex(-4,-6.5,1),
  Vertex(-3,-6.5,1),
  Vertex(-3,-6,1),
  Vertex(2,-6,1),
  Vertex(5,-4,1),
  Vertex(3,-5,1),
  Vertex(1,-5,1),
  Vertex(1,6,1),
  Vertex(-1,6,1)};
  vector<vector<uint16_t>> edges;
  for(uint8_t i = 0; i < verts.size()-1; i++){
    edges.push_back({i,uint16_t(i+1)});
  }
  edges.push_back({uint16_t(verts.size()-1),0});
  WireFrame hammer = WireFrame(verts,edges);
  //side 2;
  verts = {Vertex(-1,-4,-1),
  Vertex(-3,-4,-1),
  Vertex(-3,-3.5,-1),
  Vertex(-4,-3.5,-1),
  Vertex(-4,-6.5,-1),
  Vertex(-3,-6.5,-1),
  Vertex(-3,-6,-1),
  Vertex(2,-6,-1),
  Vertex(5,-4,-1),
  Vertex(3,-5,-1),
  Vertex(1,-5,-1),
  Vertex(1,6,-1),
  Vertex(-1,6,-1)};
  edges = {};
  for(uint8_t i = 0; i < verts.size()-1; i++){
    edges.push_back({i,uint16_t(i+1)});
  }
  edges.push_back({uint16_t(verts.size()-1),0});
  //add the two together
  hammer.join(WireFrame(verts,edges));
  //stitching the two together
  for(uint8_t v = 0; v<13; v++){
    hammer.edges.push_back({v,uint16_t(v+13)});
  }
  hammer.scale = 2.2;
  hammer.offset.x = screenWidth/2;
  hammer.offset.y = screenHeight/2;
  hammer.rotate(15,2);
  return hammer;
}
WireFrame makeWrench(){
  //wrench
  //10 verts
  //frontside
  //lower jaw tip
  Vertex v1 = Vertex(-9,-6,2);
  //lower jaw elbow
  Vertex v2 = Vertex(-9,-1,2);
  //topL handle
  Vertex v3 = Vertex(-4,-1,2);
  //bottomL handle
  Vertex v4 = Vertex(6,9,2);
  //bottomR handle
  Vertex v5 = Vertex(9,6,2);
  //topR handle
  Vertex v6 = Vertex(-1,-4,2);
  //upper jaw elbow
  Vertex v7 = Vertex(-1,-9,2);
  //upper jaw tip
  Vertex v8 = Vertex(-6,-9,2);
  //upper inner jaw
  Vertex v9 = Vertex(-3,-6,2);
  //lower inner jaw
  Vertex v10 = Vertex(-6,-3,2);

  //backside
  //lower jaw tip
  Vertex v11 = Vertex(-9,-6,-2);
  //lower jaw elbow
  Vertex v12 = Vertex(-9,-1,-2);
  //topL handle
  Vertex v13 = Vertex(-4,-1,-2);
  //bottomL handle
  Vertex v14 = Vertex(6,9,-2);
  //bottomR handle
  Vertex v15 = Vertex(9,6,-2);
  //topR handle
  Vertex v16 = Vertex(-1,-4,-2);
  //upper jaw elbow
  Vertex v17 = Vertex(-1,-9,-2);
  //upper jaw tip
  Vertex v18 = Vertex(-6,-9,-2);
  //upper inner jaw
  Vertex v19 = Vertex(-3,-6,-2);
  //lower inner jaw
  Vertex v20 = Vertex(-6,-3,-2);
  vector<Vertex> verties = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19,v20};
  vector<vector<uint8_t>> edges = 
                                  {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8},{8,9},{9,0},
                                   {10,11},{11,12},{12,13},{13,14},{14,15},{15,16},{16,17},{17,18},{18,19},{19,10},
                                   {0,10},{1,11},{2,12},{3,13},{4,14},{5,15},{6,16},{7,17},{8,18},{9,19}};
  WireFrame wrench = WireFrame(verties,edges);
  wrench.offset.x = screenWidth/2;
  wrench.offset.y = screenHeight/2;
  wrench.scale = 1.5;
  return wrench;
}

//drawing a circular din jack
WireFrame makeMIDI(){
  vector<Vertex> finalVerts;
  vector<vector<uint16_t>> finalEdges;
  int8_t zCoef = -1;
  for(uint8_t j = 0; j<2 ; j++){
    if(j == 1)
      zCoef = 1;
    //going around the circle
    //basically, copy this quarter circle 4 times
    vector<Vertex> verties;
    int8_t xCoef;
    int8_t yCoef;
    for(uint8_t i = 0; i<4; i++){
      vector<Vertex> temp;
      switch(i){
        case 3:
          xCoef = -1;
          yCoef = -1;
          temp.push_back(Vertex(xCoef,yCoef*4,zCoef));//1
          temp.push_back(Vertex(float(xCoef)*2.75,yCoef*3,zCoef));//2
          temp.push_back(Vertex(xCoef*4,yCoef,zCoef));//3
          break;
        case 2:
          xCoef = -1;
          yCoef = 1;
          temp.push_back(Vertex(xCoef*4,yCoef,zCoef));//3
          temp.push_back(Vertex(float(xCoef)*2.75,yCoef*3,zCoef));//2
          temp.push_back(Vertex(xCoef,yCoef*4,zCoef));//1
          break;
        case 1:
          xCoef = 1;
          yCoef = 1;
          temp.push_back(Vertex(xCoef,yCoef*4,zCoef));//1
          temp.push_back(Vertex(float(xCoef)*2.75,yCoef*3,zCoef));//2
          temp.push_back(Vertex(xCoef*4,yCoef,zCoef));//3
          break;
        case 0:
          xCoef = 1;
          yCoef = -1;
          temp.push_back(Vertex(xCoef*4,yCoef,zCoef));//3
          temp.push_back(Vertex(float(xCoef)*2.75,yCoef*3,zCoef));//2
          temp.push_back(Vertex(xCoef,yCoef*4,zCoef));//1
          break;
      }
      verties.insert(verties.begin(), temp.begin(),temp.end());
    }
    
    //bottom of the top notch, from right to left /|__|
    Vertex v1 = Vertex(1,-3,zCoef);
    Vertex v2 = Vertex(-1,-3,zCoef);
    verties.push_back(v1);
    verties.push_back(v2);
    vector<vector<uint16_t>> edges;
    uint16_t bigOffset = finalVerts.size();
    //making all the circle edges (they're all sequential)
    for(uint8_t i = 0; i<verties.size()-1; i++){
      vector<uint16_t> temp;
      temp = {uint16_t(i+bigOffset),uint16_t(bigOffset+i+1)};
      edges.push_back(temp);
    }
    edges.push_back({uint16_t(verties.size()-1+bigOffset),bigOffset});
    //add
    finalVerts.insert(finalVerts.end(),verties.begin(),verties.end());
    finalEdges.insert(finalEdges.end(),edges.begin(),edges.end());
    //if it's the final iteration, connect all the edges
    uint16_t offset = verties.size();
    if(j == 1){
      for(uint8_t v = 0; v<verties.size(); v++){
        finalEdges.push_back({v,uint16_t(v+offset)});
      }
    }
  }
  //adding midi dots
  uint8_t dotOffset = finalVerts.size();
  finalVerts.push_back(Vertex(-2,1,0));
  finalVerts.push_back(Vertex(-2.5,-1,0));
  finalVerts.push_back(Vertex(0,2.5,0));
  finalVerts.push_back(Vertex(2.5,-1,0));
  finalVerts.push_back(Vertex(2,1,0));

  //building final WF object
  WireFrame jack = WireFrame(finalVerts,finalEdges);
  jack.dots.push_back(dotOffset);
  jack.dots.push_back(dotOffset+1);
  jack.dots.push_back(dotOffset+2);
  jack.dots.push_back(dotOffset+3);
  jack.dots.push_back(dotOffset+4);

  jack.drawDots = true;
  jack.offset.x = screenWidth/2;
  jack.offset.y = screenHeight/2;
  jack.scale = 3;
  jack.rotate(30,0);
  return jack;
}

WireFrame makeStretchedCircle(float x1, float x2, float y, float z, float r, float points, uint8_t vertOffset){
  float angleIncrement = float(2*PI)/float(points);
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  for(uint8_t i = 0; i<points; i++){
    Vertex v;
    if(i>=points/2)
      v = Vertex(y+r*cos(angleIncrement*i),x1+r*sin(angleIncrement*i),z);
    else
      v = Vertex(y+r*cos(angleIncrement*i),x2+r*sin(angleIncrement*i),z);
    verties.push_back(v);
    if(i<points-1)
      edges.push_back({uint16_t(vertOffset+i),uint16_t(vertOffset+i+1)});
  }
  edges.push_back({uint16_t(vertOffset+points-1),vertOffset});

  WireFrame disc = WireFrame(verties,edges);
  return disc;
}

WireFrame makeDisc(float x1, float y1, float z1, float x2, float y2, float z2, float r, uint8_t points, uint8_t vertOffset){
  float angleIncrement = float(2.0*PI)/float(points);
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  for(uint8_t i = 0; i<points; i++){
    Vertex v;
    if(i<=points/2)
      v = Vertex(x2+r*cos(angleIncrement*i),y2+r*sin(angleIncrement*i),z2);
    else
      v = Vertex(x1+r*cos(angleIncrement*i),y1+r*sin(angleIncrement*i),z1);
    verties.push_back(v);
    if(i<points-1)
      edges.push_back({uint16_t(vertOffset+i),uint16_t(vertOffset+i+1)});
  }
  edges.push_back({uint16_t(vertOffset+points-1),vertOffset});

  WireFrame disc = WireFrame(verties,edges);
  return disc;
}

//makes a disc, vertOffset is so that the disc edges stay in synch
WireFrame makeDisc_centered(float x1, float y1, float z1, float r, float xSpacing, uint8_t points, uint8_t vertOffset){
  float angleIncrement = float(2*PI)/float(points);
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  for(uint8_t i = 0; i<points; i++){
    Vertex v;
    if(i>=points/2)
      v = Vertex(x1+r*cos(angleIncrement*i),y1+r*sin(angleIncrement*i)-xSpacing/2,z1);
    else
      v = Vertex(x1+r*cos(angleIncrement*i),y1+r*sin(angleIncrement*i)+xSpacing/2,z1);
    verties.push_back(v);
    if(i<points-1)
      edges.push_back({uint16_t(vertOffset+i),uint16_t(vertOffset+i+1)});
  }
  edges.push_back({uint16_t(vertOffset+points-1),vertOffset});

  WireFrame disc = WireFrame(verties,edges);
  return disc;
}

WireFrame makeCircle(float r1, uint8_t points){
  float angleIncrement = float(2*PI)/float(points);
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  for(uint8_t i = 0; i<points; i++){
    Vertex v;
    v = Vertex(r1*cos(angleIncrement*i),r1*sin(angleIncrement*i),0);
    verties.push_back(v);
    if(i<points-1)
      edges.push_back({i,uint16_t(i+1)});
  }
  edges.push_back({uint16_t(points-1),0});

  WireFrame circle = WireFrame(verties,edges);
  return circle;
}

WireFrame makeGyro(float angleX, float angleY, float angleZ,float angle2X, float angle2Y, float angle2Z){
  WireFrame arc1 = makeCircle(16,16);
  WireFrame arc2 = arc1;

  arc1.rotate(angleX,0);
  arc1.rotate(angleY,1);
  arc1.rotate(angleZ,2);

  arc2.rotate(angle2X,0);
  arc2.rotate(angle2Y,1);
  arc2.rotate(angle2Z,2);

  arc1.join(arc2);
  arc1.offset.x = 111;
  arc1.offset.y = 16;
  return arc1;
}

WireFrame makeCycle(){
    vector<Vertex> verts;
    vector<vector<uint16_t>> edges;
    //forks
    verts.push_back(Vertex(-5,0,-0.5));//0 front wheel center
    verts.push_back(Vertex(-5,0,0.5));//1
    verts.push_back(Vertex(-2,-3,-2));//2 side of fork
    verts.push_back(Vertex(-2,-3,2));//3
    verts.push_back(Vertex(-1.5,-5,-1));//4 top of windshield
    verts.push_back(Vertex(-1.5,-5,1));//5
    
    //center
    verts.push_back(Vertex(-2,0.5,1));//6
    verts.push_back(Vertex(1,0.5,1));//7
    verts.push_back(Vertex(2,-2,1));//8
    verts.push_back(Vertex(-3,-2,1));//9
    verts.push_back(Vertex(-1.5,-1,1.5));//10
    verts.push_back(Vertex(0.5,-1,1.5));//11
    //difference in verts between these two is 6!
    verts.push_back(Vertex(-2,0.5,-1));//12
    verts.push_back(Vertex(1,0.5,-1));//13
    verts.push_back(Vertex(2,-2,-1));//14
    verts.push_back(Vertex(-3,-2,-1));//15
    verts.push_back(Vertex(-1.5,-1,-1.5));//16
    verts.push_back(Vertex(0.5,-1,-1.5));//17
    
    //rear axle (for the rear fork)
    verts.push_back(Vertex(5,0,-0.5));//18
    verts.push_back(Vertex(5,0,0.5));//19
    
    //center point of each wheel (for spokes)
    verts.push_back(Vertex(5,0,0));//20
    verts.push_back(Vertex(-5,0,0));//21

             //forks
    edges = {{0,2},{2,4},{4,0},{4,5},{5,1},{1,3},{3,5},
             //body
             {6,7},{7,8},{8,9},{9,6},{6,10},{9,10},{10,11},{11,7},{11,8},
             {12,13},{13,14},{14,15},{15,12},{12,16},{15,16},{16,17},{17,13},{17,14},
             {6,12},{7,13},{8,14},{9,15},
             //rear fork
             {8,19},{7,19},
             {14,18},{13,18},
             //axles
             {0,1},{18,19},
    };

    WireFrame cycle = WireFrame(verts, edges);
    WireFrame wheel = makeCircle(2.25,10);
    WireFrame wheel1 = wheel;
    //front wheel
    wheel1.move(-5,0,0.5);
    cycle.join(wheel1);
    wheel1 = wheel;
    wheel1.move(-5,0,-0.5);
    cycle.join(wheel1);
    //back wheel
    wheel1 = wheel;
    wheel1.move(5,0,0.5);
    cycle.join(wheel1);
    wheel1 = wheel;
    wheel1.move(5,0,-0.5);
    cycle.join(wheel1);
    
    //front spokes
    WireFrame spokes = makeCircle(1.5,12);
    WireFrame rim = makeCircle(1.75,12);
    spokes.edges = {};
    for(uint8_t i = 0; i< spokes.verts.size(); i+=2){
        spokes.edges.push_back({i,uint16_t(i+6)});
    }
    spokes.move(-5,0,0);
    rim.move(-5,0,0);
//    cycle.join(spokes);
    cycle.join(rim);
    rim.move(10,0,0);
    cycle.join(rim);

    
    //front disc brake
    WireFrame disc = makeCircle(0.75,10);
    WireFrame disc1 = disc;
    disc1.move(-5,0,0);
    cycle.join(disc1);
    //rear disc brake
    disc1 = disc;
    disc1.move(5,0,0);
    cycle.join(disc1);
    
    
    cycle.offset.x = 64;
    cycle.offset.y = 32;
    cycle.scale = 5;
    return cycle;
}

//makes a pram outline
WireFrame makeHalfPramBody(float zOff){
  vector<Vertex> verts;
  vector<vector<uint16_t>> edges;
  //body
  Vertex v1 = Vertex(1,0,zOff);//bottom of shade hinge
  Vertex v2 = Vertex(0,-7,zOff);//top of pram
  Vertex v3 = Vertex(3,-6,zOff);//top of pram
  Vertex v4 = Vertex(5,-3,zOff);//top of pram
  Vertex v5 = Vertex(6,0,zOff);//base of shade
  Vertex v6 = Vertex(4,5,zOff);//bottom R corner of body
  Vertex v7 = Vertex(-5,5,zOff);//bottom L corner of body
  Vertex v8 = Vertex(-6.5,1,zOff);//front of pram
  Vertex v9 = Vertex(-6,0,zOff);//top L corner
  verts = {v1,v2,v3,v4,v5,v6,v7,v8,v9};
  edges.push_back({uint16_t(verts.size()-1),0});
  for(uint8_t i = 0; i<verts.size()-1; i++){
    edges.push_back({i,uint16_t(i+1)});
  }

  WireFrame halfPram = WireFrame(verts,edges);
  return halfPram;
}
WireFrame makeHalfPramLegs(float zOff){
  vector<Vertex> verts;
  vector<vector<uint16_t>> edges;
  //back wheel legs
  Vertex v10 = Vertex(2,5,zOff);
  Vertex v11 = Vertex(4,9,zOff);
  verts.push_back(v10);
  verts.push_back(v11);
  edges.push_back({uint16_t(verts.size()-2),uint16_t(verts.size()-1)});
  //front wheel legs
  Vertex v12 = Vertex(-2,5,zOff);
  Vertex v13 = Vertex(-5,9,zOff);
  verts.push_back(v12);
  verts.push_back(v13);
  edges.push_back({uint16_t(verts.size()-2),uint16_t(verts.size()-1)});
  WireFrame legs = WireFrame(verts,edges);
  return legs;
}

WireFrame makePram(){
  const float zOff = 5;
  //body
  WireFrame pram = makeHalfPramBody(zOff);
  uint8_t vertCount = pram.verts.size();
  pram.join(makeHalfPramBody(-zOff));
  //connecting two halves
  for(uint8_t i = 0; i<vertCount; i ++){
    pram.edges.push_back({i,uint16_t(i+vertCount)});
  }
  //legs
  pram.join(makeHalfPramLegs(zOff));
  pram.join(makeHalfPramLegs(-zOff));
  //wheels
  for(uint8_t i = 0; i<4; i++){
    WireFrame wheel = makeCircle(2.5,18);
    switch(i){
      //front wheels
      case 0:
        wheel.move(-5,9,zOff);
        break;
      case 1:
        wheel.move(-5,9,-zOff);
        break;
      //back wheels
      case 2:
        wheel.move(4,9,zOff);
        break;
      case 3:
        wheel.move(4,9,-zOff);
        break;
    }
    pram.join(wheel);
  }
  pram.offset.x = screenWidth/2;
  pram.offset.x-=10;
  pram.offset.y = screenHeight/2;
  pram.scale = 1.5;
  pram.rotate(15,2);
  pram.rotate(15,0);
  return pram;
}

WireFrame makeGear(float r1, float r2, uint8_t teeth, uint8_t points, bool center, float zPos){
  float angleIncrement = float(2*PI)/(points);
  uint8_t ptsPerTooth = points/(teeth*2);
  bool inside = false;
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  for(int i = 0; i < points; i++){
    Vertex v1;
    //every time you make 'x' points, go from inside-->outside (or vice versa)
    if(!(i%(ptsPerTooth))){
      inside = !inside;
    }
    if(inside)
      v1 = Vertex(r1*cos(angleIncrement*i),r1*sin(angleIncrement*i),zPos);
    else
      v1 = Vertex(r2*cos(angleIncrement*i),r2*sin(angleIncrement*i),zPos);
    verties.push_back(v1);
  }
  for(uint8_t i = 0; i<verties.size()-1; i++){
    edges.push_back({i,uint16_t(i+1)});
  }
  edges.push_back({uint16_t(verties.size()-1),0});
  WireFrame gear = WireFrame(verties,edges);
  if(center){
    gear.join(makeCircle(2,8));
  }
  // gear.offset.x = 64;
  // gear.offset.y = 32;
  // gear.scale = 3;
  return gear;
}

WireFrame makeThickGear(float r1, float r2, uint8_t teeth, uint8_t points, bool center){
  WireFrame g1 = makeGear(r1,r2,teeth,points,false,-1);
  WireFrame g2 = makeGear(r1,r2,teeth,points,false,1);
  uint16_t offset = g2.verts.size();
  for(uint16_t i = 0; i<offset; i++){
    g1.edges.push_back({i,uint16_t(i+offset)});
  }
  g1.join(g2);
  if(center)
    g1.join(makeCircle(2,8));
  return g1;
}

//makes a generic box centered on 0,0,0
WireFrame makeBox(uint8_t w, uint8_t h, uint8_t d){
  vector<Vertex> verties;
  vector<vector<uint16_t>> edgeList;
  int8_t zSign = 1;
  uint8_t offset = 0;
  //for each 'side' of the box (basically a front and back square)
  for(uint8_t side = 0; side<2; side++){
    if(side == 1){
      zSign = -zSign;
      offset+=4;
    }
    //square corners
    Vertex v1 = Vertex(-w/2,-h/2,d/2*zSign);
    Vertex v2 = Vertex(w/2,-h/2,d/2*zSign);
    Vertex v3 = Vertex(w/2,h/2,d/2*zSign);
    Vertex v4 = Vertex(-w/2,h/2,d/2*zSign);
    verties.push_back(v1);
    verties.push_back(v2);
    verties.push_back(v3);
    verties.push_back(v4);
    for(uint8_t point = 0; point<3; point++){
      edgeList.push_back({uint16_t(offset+point),uint16_t(offset+point+1)});
    }
    edgeList.push_back({uint16_t(offset+3),offset});
  }
  vector<vector<uint16_t>> temp2 = {{0,4},{1,5},{2,6},{3,7}};
  edgeList.insert(edgeList.end(),temp2.begin(),temp2.end());
  WireFrame box = WireFrame(verties,edgeList);
  return box;
}
WireFrame makeCassette(){
  //case
  WireFrame b = makeBox(12,8,2);
  //tape
  WireFrame d = makeDisc_centered(1,0,1,2,6,20,0);
  d.rotate(90,2);
  b.join(d);
  //spokes
  WireFrame a = makeDisc_centered(-3,1,1,1,0,10,0);
  WireFrame c = makeDisc_centered(3,1,1,1,0,10,0);
  b.join(a);
  b.join(c);
  //rectangle
  Vertex v1 = Vertex(-5.5,3,1);
  Vertex v2 = Vertex(5.5,3,1);
  Vertex v3 = Vertex(5.5,-2.25,1);
  Vertex v4 = Vertex(-5.5,-2.25,1);
  vector<Vertex> verts = {v1,v2,v3,v4};
  vector<vector<uint16_t>> edges = {{0,1},{1,2},{2,3},{3,0}};
  WireFrame e = WireFrame(verts,edges);
  b.join(e);

  b.offset.x = screenWidth/2;
  b.offset.y = screenHeight/2;
  b.scale = 2.5;
  // b.rotate(30,0);
  // b.rotate(15,2);
  return b;
}
WireFrame makeGraphBox_old(float offset){
  // WireFrame box = makeBox(12,8,8);
  WireFrame box;
  //backHalf
  Vertex v1 = Vertex(-5,0,0);
  box.verts.push_back(v1);
  box.dots.push_back(box.verts.size()-1);
  //making points within box
  for(uint8_t point = 0; point<4; point++){
    box.verts.push_back(Vertex(-4+point*2,offset*sin(point*3*PI/2),0));
    //connecting points
    box.edges.push_back({uint16_t(box.verts.size()-1),uint16_t(box.verts.size()-2)});
  }
  Vertex v2 = Vertex(5,0,0);
  box.verts.push_back(v2);
  box.edges.push_back({uint16_t(box.verts.size()-1),uint16_t(box.verts.size()-2)});
  box.dots.push_back(uint16_t(box.verts.size()-1));

  box.offset.x = screenWidth/2;
  box.offset.y = screenHeight/2;
  box.drawDots = true;
  box.scale = 2;
  box.rotate(20,2);
  box.rotate(15,0);
  return box;
}

WireFrame makeGraphBox(float offset){

  WireFrame box;

  const uint16_t numberOfPoints = 20;
  const float width = 10;
  const float height = 6;

  // WireFrame box = makeBox(width,height,height);
  // box.verts[0] = Vertex(-width/2,sin(offset),0);
  for(uint16_t i = 0; i<numberOfPoints; i++){
    float x = float(i)/float(numberOfPoints) * width-width/2.0;
    float y = height*sin(x*PI/(width/4.0)+offset);
    box.verts.push_back(Vertex(x,y,0));
    if(i != 0)
      box.edges.push_back({uint16_t(i-1),i});
  }

  // box.offset.x = screenWidth/2;
  // box.offset.y = screenHeight/2;
  box.scale = 2;
  box.rotate(15,0);
  return box;
}

float limit(float target,float min,float max){
  if(target<min)
    return min;
  else if(target>max)
    return max;
  return target;
}

//makes a metronome (with an adjustable angle)
WireFrame makeMetronome(float offset){
  //8 verts for box
  Vertex v1 = Vertex(-2,-6,-3);//TopbackL
  Vertex v2 = Vertex(2,-6,-3);//TopbackR
  Vertex v3 = Vertex(2,-6,0);//TopfrontR
  Vertex v4 = Vertex(-2,-6,0);//TopfrontL

  Vertex v5 = Vertex(-4,6,-3);//BbackL
  Vertex v6 = Vertex(4,6,-3);//BbackR
  Vertex v7 = Vertex(4,6,3);//BfrontR
  Vertex v8 = Vertex(-4,6,3);//BfrontL

  //two for a line across
  Vertex v9 = Vertex(-3.5,1,1.75);//L
  Vertex v10 = Vertex(3.5,1,1.75);//R

  //two for stick
  Vertex v11 = Vertex(0,1,1.75);//lower
  Coordinate r = utils.getRadian(0,1,5,5,30.0*cos(offset)+90.0);
  Vertex v12 = Vertex(r.x,-r.y,1.75);//upper
  // Vertex v12 = Vertex(5.0*cos(offset),-5.5+limit(sin(offset/PI),0,1),1.75);//upper

  vector<Vertex> verties = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12};
  vector<vector<uint8_t>> edges = {{0,1},{1,2},{2,3},{3,0},//top
                                   {4,5},{5,6},{6,7},{7,4},//bottom
                                   {0,4},{1,5},{2,6},{3,7},//sides
                                   {8,9},{10,11}};//stick+divider
  WireFrame met = WireFrame(verties,edges);
  met.offset.x = screenWidth/2;
  met.offset.y = screenHeight/2;
  met.scale = 1.75;
  met.rotate(10,0);
  met.rotate(10,1);
  return met;
}
//this is busted
WireFrame makePencil(){
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  WireFrame pencil;
  pencil.offset.x = screenWidth/2;
  pencil.offset.y = screenHeight/2;
  pencil.join(makeDisc_centered(0,20,0,4,0,6,0));
  for(uint16_t edge = 0; edge<6; edge++){
    pencil.edges.push_back({edge,uint16_t(edge+5)});
    pencil.edges.push_back({uint16_t(edge+6),uint16_t(edge+11)});
  }
  return pencil;
}

WireFrame makeLoopArrows(float angle){
  const uint8_t numberOfPoints = 15;
  float angleOffset = float(2*PI)/float(numberOfPoints);
  uint8_t r = 5;
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  int8_t xSign = 1;
  int8_t ySign = 1;
  uint8_t offset = 0;
  for(uint8_t arrow = 0; arrow<2; arrow++){
    vector<Vertex> temp;
    if(arrow == 1){
      xSign = -xSign;
      ySign = -ySign;
      offset = verties.size();
    }
    //iterate around a circle
    //the first 2 points are the head elbows, the tip
    //outer points (save the first 2 for the head)
    //goes left to right
    for(uint8_t point = 2; point<=numberOfPoints/2; point++){
      float x1 = xSign*r*cos(point*angleOffset);
      float y1 = ySign*r*sin(point* angleOffset);
      temp.push_back(Vertex(x1,y1,0));
    }
    //inner points (save the last 3 for the head)
    //goes right to left
    for(uint8_t point = numberOfPoints/2; point>=2; point--){
      float x1 = xSign*(r-2)*cos(point*angleOffset);
      float y1 = ySign*(r-2)*sin(point*angleOffset);
      temp.push_back(Vertex(x1,y1,0));
    }
    //arrow heads
    //bottom R to top L
    Vertex v1 = Vertex(xSign*(r-3)*cos(angleOffset*2),ySign*(r-3)*sin(angleOffset*2),0);//bottom elbow
    Vertex v2 = Vertex(xSign*(r-1)*cos(angleOffset),ySign*(r-1)*sin(angleOffset),0);//tip
    Vertex v3 = Vertex(xSign*(r+1)*cos(angleOffset*2),ySign*(r+1)*sin(angleOffset*2),0);//top elbow
    temp.push_back(v1);
    temp.push_back(v2);
    temp.push_back(v3);
    for(uint8_t edge = 0; edge<temp.size()-1; edge++){
      edges.push_back({uint16_t(offset+edge),uint16_t(offset+edge+1)});
    }
    edges.push_back({uint16_t(offset+temp.size()-1),offset});
    verties.insert(verties.end(),temp.begin(),temp.end());
  }
  WireFrame arrows = WireFrame(verties,edges);
  arrows.offset.x = screenWidth/2;
  arrows.offset.y = screenHeight/2;
  arrows.scale = 3;
  arrows.rotate(-angle,2);
  return arrows;
}

WireFrame makeMobius(float offset){
  //mobius parametric representation
  /*
  x = (R+s*cos(1/2*t))*cos(t)
  y = (R+s*cos(1/2*t))*sin(t)
  z = s*sin(1/2*t);

  t varies from 0 --> 2PI
  and s varies from -w --> w (the width)
  R is the radius
  */
 //for this loop, we'll make two edges of the mobius band with different widths
  const uint8_t R = 5;
  const uint8_t num = 16;
  // const uint8_t num = 12;
  float w = -1.5;
  //make each edge
  vector<Vertex> verts1;
  vector<vector<uint16_t>> edges;
  for(uint8_t edge = 0; edge<2; edge++){
    for(float t = 0; t<(2*PI); t+=float(2*PI)/float(num)){
      float x1 = (R+w*cos(t/2+offset/2))*cos(t+offset);
      float y1 = (R+w*cos(t/2+offset/2))*sin(t+offset);
      float z1 = w*sin(t/2+offset/2);
      verts1.push_back(Vertex(x1,y1,z1));
    }
    w = -w;
  }
  for(uint8_t i = 0; i<verts1.size()-1; i++){
    //ring edges
    edges.push_back({i,uint16_t(i+1)});
    //edges connecting the two rings
    if(i<num && i>0)
      edges.push_back({i,uint16_t(i+num)});
  }
  edges.push_back({uint16_t(verts1.size()-1),0});
  WireFrame mobius = WireFrame(verts1,edges);

  mobius.offset.x = screenWidth/2;
  mobius.offset.y = screenHeight/2;
  mobius.scale = 2.5;
  mobius.rotate(60,0);
  mobius.rotate(30,2);
  return mobius;
}

WireFrame makeLoopArrows_Old(float angle){
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  int8_t xSign = 1;
  int8_t ySign = 1;
  uint8_t offset = 0;
  for(uint8_t arrow = 0; arrow<2; arrow++){
    if(arrow == 1){
      xSign = -xSign;
      ySign = -ySign;
      offset = 14;
    }
    //bottom of arrow
    Vertex v1 = Vertex(-3*xSign,-2*ySign,0);
    Vertex v2 = Vertex(-5*xSign,-2*ySign,0);
    Vertex v3 = Vertex(-5*xSign,-3*ySign,0);
    Vertex v4 = Vertex(-3*xSign,-5*ySign,0);
    Vertex v5 = Vertex(3*xSign,-5*ySign,0);
    Vertex v6 = Vertex(5*xSign,-3*ySign,0);
    Vertex v7 = Vertex(5*xSign,-1*ySign,0);//arrow head
    Vertex v8 = Vertex(6*xSign,-1*ySign,0);
    Vertex v9 = Vertex(4*xSign,1*ySign,0);//tip
    Vertex v10 = Vertex(2*xSign,-1*ySign,0);
    Vertex v11 = Vertex(3*xSign,-1*ySign,0);
    Vertex v12 = Vertex(3*xSign,-2*ySign,0);
    Vertex v13 = Vertex(2*xSign,-3*ySign,0);
    Vertex v14 = Vertex(-2*xSign,-3*ySign,0);
    vector<Vertex> temp = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14};
    for(uint8_t edge = 0; edge<temp.size()-1; edge++){
      edges.push_back({uint16_t(offset+edge),uint16_t(offset+edge+1)});
    }
    edges.push_back({uint16_t(offset+temp.size()-1),offset});
    verties.insert(verties.end(),temp.begin(),temp.end());
  }
  WireFrame arrows = WireFrame(verties,edges);
  arrows.offset.x = screenWidth/2;
  arrows.offset.y = screenHeight/2;
  arrows.scale = 2;
  arrows.rotate(angle,2);
  return arrows;
}
WireFrame makeKeys(){
  vector<Vertex> verties;
  vector<vector<uint16_t>> edges;
  //making big keys
  for(uint8_t i = 0; i<3; i++){
    //top
    Vertex v1 = Vertex(-5.5+4*i,-4.5,-0.5);//topL
    Vertex v2 = Vertex(-2.5+4*i,-4.5,-0.5);//topR
    Vertex v3 = Vertex(-2.5+4*i,4.5,-0.5);//bottomR
    Vertex v4 = Vertex(-5.5+4*i,4.5,-0.5);//bottomL
    //bottom
    // Vertex v5 = Vertex(-5.5+4*i,-4.5,-2.25);//topL
    // Vertex v6 = Vertex(-2.5+4*i,-4.5,-2.25);//topR
    // Vertex v7 = Vertex(-2.5+4*i,4.5,-2.25);//bottomR
    // Vertex v8 = Vertex(-5.5+4*i,4.5,-2.25);//bottomL
    vector<Vertex> temp = {v1,v2,v3,v4};
    //edges
    int offset = verties.size();
    vector<vector<uint16_t>> temp2;
    for(uint8_t j = 0; j<4; j++){
      temp2.push_back({uint16_t(offset+j),uint16_t(offset+(j+1)%4)});
    }
    //add new verts and edges
    verties.insert(verties.end(),temp.begin(),temp.end());
    edges.insert(edges.end(),temp2.begin(),temp2.end());
  }
  //making small keys
  for(uint8_t i = 0; i<2; i++){
    //top
    Vertex v1 = Vertex(-3.5+4*i,-6.5,0.5);//topL
    Vertex v2 = Vertex(-0.5+4*i,-6.5,0.5);//topR
    Vertex v3 = Vertex(-0.5+4*i,0.5,0.5);//bottomR
    Vertex v4 = Vertex(-3.5+4*i,0.5,0.5);//bottomL
    vector<Vertex> temp = {v1,v2,v3,v4};
    //edges
    int offset = verties.size();
    vector<vector<uint16_t>> temp2;
    for(uint8_t j = 0; j<4; j++){
      temp2.push_back({uint16_t(offset+j),uint16_t(offset+(j+1)%4)});
    }
    //add new verts and edges
    verties.insert(verties.end(),temp.begin(),temp.end());
    edges.insert(edges.end(),temp2.begin(),temp2.end());
  }

  WireFrame keys = WireFrame(verties,edges);
  keys.offset.y = screenHeight/2;
  keys.rotate(45,0);
  keys.scale = 2;
  return keys;
}

//5 piano keys (2 black, 3 white)
// WireFrame makeKeys_old(){
  // vector<Vertex> verties;
  // vector<vector<uint8_t>> edges;
  // //making big keys
  // for(uint8_t i = 0; i<3; i++){
  //   //top
  //   Vertex v1 = Vertex(-5.5+4*i,-4.5,-0.25);//topL
  //   Vertex v2 = Vertex(-2.5+4*i,-4.5,-0.25);//topR
  //   Vertex v3 = Vertex(-2.5+4*i,4.5,-0.25);//bottomR
  //   Vertex v4 = Vertex(-5.5+4*i,4.5,-0.25);//bottomL
  //   //bottom
  //   Vertex v5 = Vertex(-5.5+4*i,-4.5,-2.25);//topL
  //   Vertex v6 = Vertex(-2.5+4*i,-4.5,-2.25);//topR
  //   Vertex v7 = Vertex(-2.5+4*i,4.5,-2.25);//bottomR
  //   Vertex v8 = Vertex(-5.5+4*i,4.5,-2.25);//bottomL
  //   vector<Vertex> temp = {v1,v2,v3,v4,v5,v6,v7,v8};
  //   //edges
  //   int offset = verties.size();
  //   vector<vector<uint8_t>> temp2 = {{0+offset,1+offset},{1+offset,2+offset},{2+offset,3+offset},{3+offset,0+offset},//top
  //                                    {4+offset,5+offset},{5+offset,6+offset},{6+offset,7+offset},{7+offset,4+offset},//bottom
  //                                    {0+offset,4+offset},{1+offset,5+offset},{2+offset,6+offset},{3+offset,7+offset}};//edges
  //   //add new verts and edges
  //   verties.insert(verties.end(),temp.begin(),temp.end());
  //   edges.insert(edges.end(),temp2.begin(),temp2.end());
  // }
  // //making small keys
  // for(uint8_t i = 0; i<2; i++){
  //   //top
  //   Vertex v1 = Vertex(-3.5+4*i,-6.5,0.25);//topL
  //   Vertex v2 = Vertex(-0.5+4*i,-6.5,0.25);//topR
  //   Vertex v3 = Vertex(-0.5+4*i,0.5,0.25);//bottomR
  //   Vertex v4 = Vertex(-3.5+4*i,0.5,0.25);//bottomL
  //   //bottom
  //   Vertex v5 = Vertex(-3.5+4*i,-6.5,2.25);//topL
  //   Vertex v6 = Vertex(-0.5+4*i,-6.5,2.25);//topR
  //   Vertex v7 = Vertex(-0.5+4*i,0.5,2.25);//bottomR
  //   Vertex v8 = Vertex(-3.5+4*i,0.5,2.25);//bottomL
  //   vector<Vertex> temp = {v1,v2,v3,v4,v5,v6,v7,v8};
  //   //edges
  //   int offset = verties.size();
  //   vector<vector<uint8_t>> temp2 = {{0+offset,1+offset},{1+offset,2+offset},{2+offset,3+offset},{3+offset,0+offset},//top
  //                                    {4+offset,5+offset},{5+offset,6+offset},{6+offset,7+offset},{7+offset,4+offset},//bottom
  //                                    {0+offset,4+offset},{1+offset,5+offset},{2+offset,6+offset},{3+offset,7+offset}};//edges
  //   //add new verts and edges
  //   verties.insert(verties.end(),temp.begin(),temp.end());
  //   edges.insert(edges.end(),temp2.begin(),temp2.end());
  // }

  // WireFrame keys = WireFrame(verties,edges);
  // keys.offset.y = screenHeight/2;
  // keys.rotate(60,0);
  // keys.scale = 2;
  // return keys;
// }

//hand for arp menu
WireFrame makeHand_flat(float f1, float f2, float f3, float f4, float f5){
  //top
  Vertex v1 = Vertex(-2.5,0,0.5);
  Vertex v2 = Vertex(-4,-3,0.5+f1);//tip
  Vertex v3 = Vertex(-3,-3,0.5+f1);//tip
  Vertex v4 = Vertex(-1.5,0,0.5);//pinky done
  Vertex v5 = Vertex(-3,-5,0.5+f2);//tip
  Vertex v6 = Vertex(-2,-5,0.5+f2);//tip
  Vertex v7 = Vertex(-0.5,-1,0.5);//ring done
  Vertex v8 = Vertex(-0.5,-5.5,0.5+f3);
  Vertex v9 = Vertex(0.5,-5.5,0.5+f3);
  Vertex v10 = Vertex(0.5,-1,0.5);//middle done
  Vertex v11 = Vertex(2,-5,0.5+f4);
  Vertex v12 = Vertex(3,-5,0.5+f4);
  Vertex v13 = Vertex(1.5,0,0.5);//pointer done
  Vertex v14 = Vertex(4,-1,0.5+f5);
  Vertex v15 = Vertex(4,0,0.5+f5);
  Vertex v16 = Vertex(1.5,2,0.5);//thumb done

  Vertex v17 = Vertex(-1.5,2,0.5);//wrist

  vector<Vertex> verties = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17};
  vector<vector<uint16_t>> edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8},{8,9},{9,10},{10,11},{11,12},{12,13},{13,14},{14,15},{15,16},{16,0}};//top
  WireFrame hand = WireFrame(verties,edges);
  hand.offset.x = screenWidth/2;
  hand.offset.y = screenHeight/2;
  hand.scale = 3;
  return hand;
}

//hand for arp menu
WireFrame makeHand(float f1, float f2, float f3, float f4, float f5){
  //top
  Vertex v1 = Vertex(-2.5,0,0.5);
  Vertex v2 = Vertex(-4,-3,0.5+f1);//tip
  Vertex v3 = Vertex(-3,-3,0.5+f1);//tip
  Vertex v4 = Vertex(-1.5,0,0.5);//pinky done
  Vertex v5 = Vertex(-3,-5,0.5+f2);//tip
  Vertex v6 = Vertex(-2,-5,0.5+f2);//tip
  Vertex v7 = Vertex(-0.5,-1,0.5);//ring done
  Vertex v8 = Vertex(-0.5,-5.5,0.5+f3);
  Vertex v9 = Vertex(0.5,-5.5,0.5+f3);
  Vertex v10 = Vertex(0.5,-1,0.5);//middle done
  Vertex v11 = Vertex(2,-5,0.5+f4);
  Vertex v12 = Vertex(3,-5,0.5+f4);
  Vertex v13 = Vertex(1.5,0,0.5);//pointer done
  Vertex v14 = Vertex(4,-1,0.5+f5);
  Vertex v15 = Vertex(4,0,0.5+f5);
  Vertex v16 = Vertex(1.5,2,0.5);//thumb done

  Vertex v17 = Vertex(-1.5,2,0.5);//wrist

  //bottom
  Vertex v18 = Vertex(-2.5,0,-0.5);
  Vertex v19 = Vertex(-4,-3,-0.5+f1);
  Vertex v20 = Vertex(-3,-3,-0.5+f1);
  Vertex v21 = Vertex(-1.5,0,-0.5);
  Vertex v22 = Vertex(-3,-5,-0.5+f2);//tip
  Vertex v23 = Vertex(-2,-5,-0.5+f2);//tip
  Vertex v24 = Vertex(-0.5,-1,-0.5);//ring done
  Vertex v25 = Vertex(-0.5,-5.5,-0.5+f3);
  Vertex v26 = Vertex(0.5,-5.5,-0.5+f3);
  Vertex v27 = Vertex(0.5,-1,-0.5);//middle done
  Vertex v28 = Vertex(2,-5,-0.5+f4);
  Vertex v29 = Vertex(3,-5,-0.5+f4);
  Vertex v30 = Vertex(1.5,0,-0.5);//pointer done
  Vertex v31 = Vertex(4,-1,-0.5+f5);
  Vertex v32 = Vertex(4,0,-0.5+f5);
  Vertex v33 = Vertex(1.5,2,-0.5);//thumb done

  Vertex v34 = Vertex(-1.5,2,-0.5);//wrist

  vector<Vertex> verties = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19,v20,v21,v22,v23,v24,v25,v26,v27,v28,v29,v30,v31,v32,v33,v34};
  vector<vector<uint16_t>> edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8},{8,9},{9,10},{10,11},{11,12},{12,13},{13,14},{14,15},{15,16},{16,0},//top
                                   {17,18},{18,19},{19,20},{20,21},{21,22},{22,23},{23,24},{24,25},{25,26},{26,27},{27,28},{28,29},{29,30},{30,31},{31,32},{32,33},{33,17},//bottom
                                   {0,17},{1,18},{2,19},{3,20},{4,21},{5,22},{6,23},{7,24},{8,25},{9,26},{10,27},{11,28},{12,29},{13,30},{14,31},{15,32},{16,33}};//connects
  WireFrame hand = WireFrame(verties,edges);
  hand.offset.x = screenWidth/2;
  hand.offset.y = screenHeight/2;
  hand.scale = 3;
  hand.rotate(-45,0);
  return hand;
}

WireFrame makeCD(){
  WireFrame outerDiscTop = makeDisc_centered(0,0,0,10,0,20,0);
  // WireFrame outerDiscBottom = makeDisc_centered(0,0,1,10,20,20);
  WireFrame middleDisc = makeDisc_centered(0,0,0,3,0,10,20);
  WireFrame innerDisc = makeDisc_centered(0,0,0,1,0,10,30);
  //adding middle disc
  outerDiscTop.verts.insert(outerDiscTop.verts.end(),middleDisc.verts.begin(),middleDisc.verts.end());
  outerDiscTop.edges.insert(outerDiscTop.edges.end(),middleDisc.edges.begin(),middleDisc.edges.end());

  ///adding inner disc
  outerDiscTop.verts.insert(outerDiscTop.verts.end(),innerDisc.verts.begin(),innerDisc.verts.end());
  outerDiscTop.edges.insert(outerDiscTop.edges.end(),innerDisc.edges.begin(),innerDisc.edges.end());
  
  outerDiscTop.offset.x = screenWidth/2;
  outerDiscTop.offset.y = screenHeight/2;
  outerDiscTop.scale = 1.5;
  // for(uint8_t dot = 0; dot<outerDiscTop.verts.size(); dot++){
  //   outerDiscTop.dots.push_back(dot);
  // }
  outerDiscTop.rotate(15,0);
  return outerDiscTop;
}


WireFrame makeHelix(uint8_t r, uint8_t revs, float length, uint8_t points, bool chirality){
  //angle increment
  float increment = 2*PI*float(revs)/float(points);
  float zIncrement = length/float(points);
  //left or right handed
  if(chirality)
    increment = -increment;
  //storing wf data
  vector<Vertex> verts;
  vector<vector<uint16_t>> edges;

  //making each point
  for(uint8_t t = 0; t<points; t++){
    Vertex v = Vertex(r*cos(t*increment),r*sin(t*increment),zIncrement*t-length/2);
    verts.push_back(v);
    if(t!=points-1)
      edges.push_back({t,uint16_t(t+1)});
  }
  WireFrame helix = WireFrame(verts,edges);
  helix.offset.x = screenWidth/2;
  helix.offset.y = screenHeight/2;
  return helix;
}

//folder anim works correctly now :)
WireFrame makeFolder(float openAngle){
  //2 points for top front edge
  Vertex v1 = Vertex(-9,-16,2);
  Vertex v2 = Vertex(9,-16,2);
  //6 for for top back edge
  Vertex v3 = Vertex(-9,-16,-2);
  Vertex v4 = Vertex(2,-16,-2);
  Vertex v5 = Vertex(3,-18,-2);
  Vertex v6 = Vertex(7,-18,-2);
  Vertex v7 = Vertex(8,-16,-2);
  Vertex v8 = Vertex(9,-16,-2);
  //bottom 2 points
  Vertex v9 = Vertex(-9,0,0);
  Vertex v10 = Vertex(9,0,0);
  vector<Vertex> verties = {v1,v2,v3,v4,v5,v6,v7,v8,v9,v10};
  vector<vector<uint8_t>> edges = {
    //front
    {0,1},{0,8},{1,9},
    //back
    {2,8},{2,3},{3,4},{4,5},{5,6},{6,7},{7,9},
    //bottom
    {8,9}
  };
  WireFrame folder = WireFrame(verties,edges);
  folder.verts[0].rotate(-openAngle,0);
  folder.verts[1].rotate(-openAngle,0);
  folder.rotate(20,2);
  folder.offset.x = screenWidth/2;
  folder.offset.y = screenHeight/2;
  folder.move(0,9,0);
  return folder;
}

//builds a rectangle in the XY plane centered on c with height = h and width = w
WireFrame makeRect(float w, float h, Vertex c){
  vector<Vertex> verties = {Vertex(c.x-w/2,c.y-h/2,c.z),Vertex(c.x+w/2,c.y-h/2,c.z),Vertex(c.x+w/2,c.y+h/2,c.z),Vertex(c.x-w/2,c.y+h/2,c.z)};
  vector<vector<uint16_t>> edges = {{0,1},{1,2},{2,3},{3,0}};
  return WireFrame(verties,edges);
}

WireFrame makeMonitor(){
    //front frame
    // WireFrame screen = makeRect(10,8,Vertex(0,0,0));
    //  WireFrame screen = makeFolder(90);
    WireFrame screen = makeRect(10,8,Vertex(0,0,0));
    screen.join(makeRect(8,6.5,Vertex(0,0,-7)));
    screen.join(makeRect(8,6.5,Vertex(0,0,-0.5)));
    // screen.addEdges({{0,4},{1,5},{2,6},{3,7}});
    screen.edges.push_back({0,4});
    screen.edges.push_back({1,5});
    screen.edges.push_back({2,6});
    screen.edges.push_back({3,7});

    WireFrame keyboard = makeBox(10,2,7);
    keyboard.move(0,7,0);
    screen.join(keyboard);

    WireFrame base = makeBox(7,4,4);
    base.move(0,6,-5);
    screen.join(base);

    screen.scale = 2;
    screen.offset.x = 80;
    screen.move(0,0,2);
  return screen;
}

//keys are three rectangular prisms that each ebb and flow
WireFrame makeArpBoxes(float keyOffsetTimer){
    WireFrame keys[3];
    //bouncing each key
    for(uint8_t i = 0; i<3; i++){
        WireFrame key = makeBox(7, 5, 5);
        key.move(10*i-10,7.0*sin(keyOffsetTimer/200.0+((2.0*PI/3.0)*i)),0);
        // key.move(10*i-10,0,0);
        keys[i] = key;
    }
    //joingin keys into one wireframe
    keys[1].join(keys[2]);
    keys[0].join(keys[1]);
    //styling
    keys[0].rotate(15,2);
    keys[0].rotate(30.0,1);
    return keys[0];
}


void openFolderAnimation(WireFrame& w,float amount){
  //make a new folder wireframe with the right open amount
  WireFrame  n = makeFolder(amount);
  //rotate it so it's at the same position as the current folder
  n.rotate(w.currentAngle[1],1);
  //then swap their vertices
  w.verts = n.verts;
}

//opening and closing
void folderAnimation(WireFrame& w){
  //make a new folder wireframe with the right open amount
  WireFrame  n = makeFolder((45.0*sin(millis()/400.0)+45.0)/2.0);
  //rotate it so it's at the same position as the current folder
  n.rotate(w.currentAngle[1],1);
  //then swap their vertices
  w.verts = n.verts;
}

void handAnimation(WireFrame* w){
  //same as folderanimation
  float fingies[5];
  for(uint8_t i = 0; i<5; i++){
    fingies[i] = abs(2*sin(millis()/100+2*i));
  }
  WireFrame n = makeHand_flat(fingies[0],fingies[1],fingies[2],fingies[3],fingies[4]);
  n.rotate(w->currentAngle[1],1);
  w->verts = n.verts;
}
void keysAnimation(WireFrame* w){
    
}

void loopArrowAnimation(WireFrame* w){
  WireFrame  n = makeLoopArrows(millis()/10);
  //rotate it so it's at the same position as the current
  n.rotate(w->currentAngle[1],1);
  //then swap their vertices
  w->verts = n.verts;
}

void metAnimation(WireFrame* w){
  //make a new metronome  wireframe with the right open amount
  WireFrame  n = makeMetronome(float((millis())/100.0));
  //rotate it so it's at the same position as the current metroniome
  n.rotate(w->currentAngle[1],1);
  //then swap their vertices
  w->verts = n.verts;
}

void graphAnimation(WireFrame* w){
  //make a new folder wireframe with the right open amount
  // WireFrame  n = makeGraphBox(6*sin(millis()/float(400)));
    WireFrame  n = makeGraphBox(millis()/float(400));
  //rotate it so it's at the same position as the current folder
  n.rotate(w->currentAngle[1],1);
  //then swap their vertices
  w->verts = n.verts;
}

void renderTest(){
  vector<WireFrame> meshes = {makeWrench(),makeFolder(30.0),makeHand(0,0,0,0,0),makeKeys(),makeMIDI(),makeMetronome(0),makeLoopArrows(0),makeGraphBox(5)};
//  float angleX,angleY,angleZ;
  uint8_t activeOption = 0;
  bool justOne = false;
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  while(true){
    controls.readJoystick();
    controls.readButtons();
    // angleX = ((millis()/400)%2)-20;
    // angleY = ((millis()/400)%2)+20;
    // angleZ = 2*((millis()/400)%2)-20;
    // meshes[1].rotate(10,1);
    // meshes[1].render();

    if(!justOne){
      for(uint8_t i = 0; i<meshes.size(); i++){
        // WireFrame temp = meshes[i];
        // temp.rotate(angleX,0);
        // temp.rotate(angleY,1);
        // temp.rotate(angleZ,2);
        // temp.render();
        if(i == 1)
          folderAnimation(meshes[i]);
        else if(i == 2)
          handAnimation(&meshes[i]);
        meshes[i].rotate(5,1);
        display.clearDisplay();
        meshes[i].render();
      }
    }
    else{
      meshes[activeOption].offset.x = screenWidth/2;
      if(activeOption == 1)
        folderAnimation(meshes[activeOption]);
      else if(activeOption == 2){
        handAnimation(&meshes[activeOption]);
      }
      else if(activeOption == 5){
        metAnimation(&meshes[activeOption]);
      }
      else if(activeOption == 6){
        loopArrowAnimation(&meshes[activeOption]);
      }
      else if(activeOption == 7){
        graphAnimation(&meshes[activeOption]);
      }
      meshes[activeOption].rotate(5,1);
      display.clearDisplay();
      meshes[activeOption].render();
    }
    display.setCursor(0,0);
    display.print("mem:");
//    display.print(rp2040.getUsedHeap());
    display.display();
    if(controls.MENU()){
      return;
    }
    if(utils.itsbeen(200)){
      if(controls.LOOP()){
        lastTime = millis();
        activeOption++;
        if(activeOption>=meshes.size())
          activeOption = 0;
      }
      if(controls.DELETE()){
        lastTime = millis();
        justOne = !justOne;
      }
    }
  }
}
