#pragma once

#include <cstdint>
#include <vector>
#include <Arduino.h>

// const uint8_t orthoMatrix[2][3] = {{1,0,0},{0,1,0}};

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
  void coordTransform(std::vector<std::vector<float>>);
  void setMag(float m);
  float getMag();
  void normalize();
  void sub(Vertex v);
  void add(Vertex v);
  void mult(float f);
};

class WireFrame{
  public:
  float currentAngle[3];
  std::vector<Vertex> verts;
  //this should also be a vector of arrays instead of a vec of vecs
  //bc each edge will only be between 2 points
  std::vector<std::vector<uint16_t>>edges;//should be 16-bit so it can handle more than 256 verts (just in case)
  std::vector<uint16_t> dots;
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
  WireFrame();
  WireFrame(std::vector<Vertex>);
  WireFrame(std::vector<Vertex>,std::vector<std::vector<uint8_t>>);
  WireFrame(std::vector<Vertex>,std::vector<std::vector<uint16_t>>);

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
  void addVerts(std::vector<Vertex>);
  void addEdges(std::vector<std::vector<uint16_t>>);
  void move(float,float,float);
};
