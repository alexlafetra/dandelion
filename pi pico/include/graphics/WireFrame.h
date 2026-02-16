const uint8_t orthoMatrix[2][3] = {{1,0,0},{0,1,0}};

class Vertex{
  public:
  float x;
  float y;
  float z;
  Vertex();
  Vertex(float,float,float);
  void render(uint8_t, uint8_t, float);
  void render(uint8_t, uint8_t, float, uint8_t);
  void render(uint8_t, uint8_t, float, uint8_t, bool);
  void render(uint8_t, uint8_t,float, uint8_t, String);
  void rotate(float, uint8_t);
  void coordTransform(vector<vector<float>>);
};

Vertex::Vertex(){
  x = 0;
  y = 0;
  z = 0;
}
Vertex::Vertex(float x1, float y1, float z1){
  x = x1;
  y = y1;
  z = z1;
}
void Vertex::render(uint8_t xOffset,uint8_t yOffset, float scale){
  display.fillCircle(x*scale+xOffset,y*scale+yOffset,2,SSD1306_WHITE);
}
void Vertex::render(uint8_t xOffset,uint8_t yOffset, float scale, uint8_t size){
  display.fillCircle(x*scale+xOffset,y*scale+yOffset,size,SSD1306_WHITE);
}
void Vertex::render(uint8_t xOffset,uint8_t yOffset, float scale, uint8_t size, bool full){
  if(full)
    display.fillCircle(x*scale+xOffset,y*scale+yOffset,size,SSD1306_WHITE);
  else
    display.drawCircle(x*scale+xOffset,y*scale+yOffset,size,SSD1306_WHITE);
}
void Vertex::render(uint8_t xOffset,uint8_t yOffset, float scale, uint8_t size, String text){
  display.fillCircle(x*scale+xOffset,y*scale+yOffset,size,SSD1306_WHITE);
  printSmall(x*scale+xOffset+3,y*scale+yOffset-5,text,SSD1306_WHITE);
}

//multiplies a vertex by a transformation matrix
void Vertex::coordTransform(vector<vector<float>> transformer){
  float x1 = 0, y1 = 0, z1 = 0;
  if(transformer.size() == 3){
    //multiplying columns
    for(uint8_t a = 0; a<3; a++){
      float temp = transformer[a][0] * x + transformer[a][1] * y + transformer[a][2] * z;
      switch(a){
        case 0:
          x1 = temp;
          break;
        case 1:
          y1 = temp;
          break;
        case 2:
          z1 = temp;
      }
    }
      x = x1;
      y = y1;
      z = z1;
  }
}

//rotates a vertex around x (0) y (1) or z (2) axes
void Vertex::rotate(float angle, uint8_t axis){
  if(axis>2)
    return;
  vector<vector<float>> rotationMatrix;
  //convert to radians
  angle = angle*(PI/180.0);
  switch(axis){
    //x
    case 0:
      rotationMatrix = {{1,0,0},{0,cos(angle),-sin(angle)},{0,sin(angle),cos(angle)}};
      break;
    //y
    case 1:
      rotationMatrix = {{cos(angle),0,sin(angle)},{0,1,0},{-sin(angle),0,cos(angle)}};
      break;
    //z
    case 2:
      rotationMatrix = {{cos(angle),-sin(angle),0},{sin(angle),cos(angle),0},{0,0,1}};
      break;
  }
  coordTransform(rotationMatrix);
}

class WireFrame{
  public:
  float currentAngle[3];
  vector<Vertex> verts;
  //this should also be a vector of arrays instead of a vec of vecs
  //bc each edge will only be between 2 points
  vector<vector<uint16_t>>edges;//should be 16-bit so it can handle more than 256 verts (just in case)
  vector<uint16_t> dots;
  // uint8_t offset.y;

  //struct for holding the screen-space offset of the wireframe
  struct Offset{
    int8_t x = 0;
    int8_t y = 0;
    Offset(int8_t x1, int8_t y1){
      x = x1;
      y = y1;
    }
  };
  Offset offset = Offset(0,0);
  uint8_t dotSize = 1;
  float scale;
  bool drawEdges;
  bool drawDots;
  WireFrame(){};
  WireFrame(vector<Vertex>);
  WireFrame(vector<Vertex>,vector<vector<uint8_t>>);
  WireFrame(vector<Vertex>,vector<vector<uint16_t>>);

  void render();
  void renderDie();
  void renderDotsIfInFrontOf(float zCutoff);
  void rotate(float,uint8_t);
  void setRotation(float,uint8_t);
  bool isFarthestVert(uint8_t);
  bool isClosestVert(uint8_t);
  uint8_t getFarthestVert();
  uint8_t getClosestVert();
  void reset(uint8_t);
  void resetExceptFor(uint8_t);
  void rotateVertRelative(uint8_t,float,uint8_t);
  void join(WireFrame);
  void addVerts(vector<Vertex>);
  void addEdges(vector<vector<uint16_t>>);
  void move(float,float,float);
};
WireFrame::WireFrame(vector<Vertex> vertices){
  verts = vertices;
  scale = 1;
  drawEdges = true;
  drawDots = false;
}
WireFrame::WireFrame(vector<Vertex> vertices,vector<vector<uint8_t>> edgeList){
  verts = vertices;
  //casting to 16-bit
  vector<vector<uint16_t>> edgeVec16Bit;
  for(int i = 0; i<edgeList.size(); i++){
    vector<uint16_t> tempVec = {uint16_t(edgeList[i][0]),uint16_t(edgeList[i][1])};
    edgeVec16Bit.push_back(tempVec);
  }
  edges = edgeVec16Bit;
  scale = 1;
  drawEdges = true;
  drawDots = false;
}
WireFrame::WireFrame(vector<Vertex> vertices,vector<vector<uint16_t>> edgeList){
  verts = vertices;
  edges = edgeList;
  scale = 1;
  drawEdges = true;
  drawDots = false;
}
void WireFrame::join(WireFrame w){

  int16_t offset = verts.size();

  //increment dot and edge indices, then add them to the mesh
  for(uint16_t e = 0; e<w.edges.size(); e++){
    edges.push_back({uint16_t(w.edges[e][0]+offset),uint16_t(w.edges[e][1]+offset)});
  }
  //adding dots
  for(uint16_t d = 0; d<w.dots.size(); d++){
    dots.push_back({uint16_t(w.dots[d]+offset)});
  }
  //finally, add verts
  verts.insert(verts.end(),w.verts.begin(),w.verts.end());
}

void WireFrame::move(float x1, float y1, float z1){
  for(int i = 0; i<verts.size(); i++){
    verts[i].x += x1;
    verts[i].y += y1;
    verts[i].z += z1;
  }
}
void WireFrame::addVerts(vector<Vertex> v){
  for(Vertex vert:v){
      this->verts.push_back(vert);
  }
}
void WireFrame::addEdges(vector<vector<uint16_t>> e){
  for(vector<uint16_t> edge:e){
      this->edges.push_back(edge);
  }
}

bool WireFrame::isFarthestVert(uint8_t which){
  for(uint8_t v =0; v<verts.size(); v++){
    if(verts[v].z<verts[which].z)
      return false;
  }
  return true;
}

bool WireFrame::isClosestVert(uint8_t which){
  for(uint8_t v = 0; v<verts.size(); v++){
    //if any coordinate is closer, u know it's not the closest
    if(verts[v].z>verts[which].z)
      return false;
  }
  return true;
}
uint8_t WireFrame::getFarthestVert(){
  uint8_t farthest = 0;
  float lowestZ = verts[0].z;
  for(uint8_t v = 1; v<verts.size(); v++){
    if(verts[v].z<lowestZ){
      farthest = v;
      lowestZ = verts[v].z;
    }
  }
  return farthest;
}
uint8_t WireFrame::getClosestVert(){
  uint8_t closest = 0;
  float highestZ = verts[0].z;
  for(uint8_t v = 1; v<verts.size(); v++){
    if(verts[v].z>highestZ){
      closest = v;
      highestZ = verts[v].z;
    }
  }
  return closest;
}
void WireFrame::renderDie(){
  //stores index of the farthest vertex
  if(drawDots){
    renderDotsIfInFrontOf(0.0);
  }
  if(drawEdges){
    uint8_t farthestVert;
    //loop thru each vertex and drW it if it's not the farthest one
    for(uint8_t vertex = 0; vertex<verts.size(); vertex++){
      if(isFarthestVert(vertex)){
        farthestVert = vertex;
        continue;
      }
    }
    //draw edges
    for(uint8_t edge = 0; edge<edges.size(); edge++){
      //checking to see if this very is the farthest
      //if it is, don't draw any lines
      if(isFarthestVert(edges[edge][0]) || isFarthestVert(edges[edge][1])){
        continue;
      }
      else
        display.drawLine(verts[edges[edge][0]].x*scale+offset.x,verts[edges[edge][0]].y*scale+offset.y,verts[edges[edge][1]].x*scale+offset.x,verts[edges[edge][1]].y*scale+offset.y,SSD1306_WHITE);
    }
  }
}

void WireFrame::render(){
  if(drawDots){
    for(uint8_t i = 0; i<dots.size(); i++){
      verts[dots[i]].render(offset.x,offset.y,scale,dotSize);
      //for labelling verts
      // verts[dots[i]].render(offset.x,offset.y,scale,1,String(i));
    }
  }
  //if it's just edges
  if(drawEdges){
    //draw edges
    for(uint8_t edge = 0; edge<edges.size(); edge++){
      //checking to see if this very is the farthest
      //if it is, don't draw any lines
      display.drawLine(verts[edges[edge][0]].x*scale+offset.x,verts[edges[edge][0]].y*scale+offset.y,verts[edges[edge][1]].x*scale+offset.x,verts[edges[edge][1]].y*scale+offset.y,SSD1306_WHITE);
    }
  }
}

void WireFrame::renderDotsIfInFrontOf(float zCutoff){
  for(uint8_t i = 0; i<dots.size(); i++){
    if(verts[dots[i]].z>zCutoff)
      verts[dots[i]].render(offset.x,offset.y,scale,1);
  }
}

//roates each vert
void WireFrame::rotate(float angle, uint8_t axis){
  for(uint8_t vertex = 0; vertex<verts.size(); vertex++){
    verts[vertex].rotate(angle,axis);
  }
  currentAngle[axis] += angle;
  if(currentAngle[axis] > 360)
    currentAngle[axis]-=360;
}
//resets verts rotation before and after applying the transformation
void WireFrame::rotateVertRelative(uint8_t which, float angle, uint8_t axis){
  verts[which].x -= offset.x;
  verts[which].y -= offset.y;
  //reset rotation
  verts[which].rotate(-currentAngle[0],0);
  verts[which].rotate(-currentAngle[1],1);
  verts[which].rotate(-currentAngle[2],2);
  
  //actual rotation
  verts[which].rotate(angle,axis);

  //reset rotation
  verts[which].rotate(currentAngle[0],0);
  verts[which].rotate(currentAngle[1],1);
  verts[which].rotate(currentAngle[2],2);
  verts[which].x += offset.x;
  verts[which].y += offset.y;
}
//this one resets the rotation of the target axis, THEN applies the rotation
void WireFrame::setRotation(float angle, uint8_t axis){
  if(axis>2)
    return;
  // float oldAngles[3] = {currentAngle[0],currentAngle[1],currentAngle[2]};
  //reset current rotation, then rotate to the new rotation (in one line)
  rotate(angle-currentAngle[axis],axis);
}

//resets the rotation of the WF based on the stored angle
void WireFrame::reset(uint8_t axis){
  for(uint8_t vertex = 0; vertex<verts.size(); vertex++){
    verts[vertex].rotate(-currentAngle[axis],axis);
  }
}
void WireFrame::resetExceptFor(uint8_t exceptFor){
  for(uint8_t vertex = 0; vertex<verts.size(); vertex++){
    for(uint8_t axis = 0; axis<3; axis++){
      if(axis != exceptFor)
        verts[vertex].rotate(-currentAngle[axis],axis);
    }
  }
}
