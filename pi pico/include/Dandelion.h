#pragma once

#include <cstdint>
#include <vector>
#include "graphics/WireFrame.h"

class Dandelion;

class Seed{
    public:
        Dandelion* mother;
        Vertex pappus;
        Vertex pVelocity;
        Vertex pAcceleration;

        Vertex achene;
        Vertex aVelocity;
        Vertex aAcceleration;

        bool attached = true;
        float tension = 0;
        uint16_t id;
        Seed() = delete;
        Seed(Dandelion* mom, uint16_t i, Vertex position);
        void update(Vertex wind);
        // Seed(const Seed&) = delete;
        // Seed& operator=(const Seed&) = delete;

        // Seed(Seed&&) = default;
        // Seed& operator=(Seed&&) = default;
};

class Dandelion{
    public:
        float flowerRadius = 100;
        float seedLength = 25;
        float pappusRadius;
        float breakLimit;
        float windVal = 0.0;
        uint16_t seedCount = 10;
        Vertex centerPosition;
        std::vector<Seed> seeds;
        
        Dandelion();
        void render();
        void update();
        Dandelion(const Dandelion&) = delete;
        Dandelion& operator=(const Dandelion&) = delete;
};