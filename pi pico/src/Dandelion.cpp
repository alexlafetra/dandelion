#include <cstdint>
#include <vector>
#include "graphics/WireFrame.h"
#include <Adafruit_SSD1306.h>
#include "Dandelion.h"

extern Adafruit_SSD1306 display;
using namespace std;


float globalMultiplier = 2.0;
float globalScale = 0.3;


struct AngularDistance{
    public:
        float distance;
        Vertex direction;
        AngularDistance(float dist, Vertex dir){
            distance = dist;
            direction = Vertex(dir.x,dir.y,dir.z);
        }
};

float dot(Vertex A, Vertex B){
    return A.x*B.x + A.y * B.y + A.z * B.z;
}

// float constrain(float val, float min, float max){
//     if(val < min)
//         return min;
//     if(val > max)
//         return max;
//     return val; 
// }

AngularDistance calcAngularDistance(Vertex v1, Vertex v2, float radius){
    // Serial.println("hello!");
    v1.normalize();
    v2.normalize();
    float d = dot(v1,v2);
    v1.mult(d);
    v2.sub(v1);
    v2.normalize();
    Vertex dir = Vertex(v2.x,v2.y,v2.z);

    float angle = acos(constrain(dot(v1,v2),-1,1));
    float surfaceDistance = angle * radius;
    return AngularDistance(surfaceDistance,dir);
}

Seed::Seed(Dandelion* mom, uint16_t i, Vertex position){
    mother = mom;

    id = i;

    pappus = Vertex(position.x,position.y,position.z);
    achene = Vertex(position.x,position.y,position.z);
    achene.setMag((mother->flowerRadius) - (mother->seedLength));

    aVelocity = Vertex(0,0,0);
    aAcceleration = Vertex(0,0,0);
    pVelocity = Vertex(0,0,0);
    pAcceleration = Vertex(0,0,0);

}

void Seed::update(Vertex wind){
    if(attached){
        Vertex surfaceNormal = Vertex(pappus.x,pappus.y,pappus.z);
        surfaceNormal.sub(Vertex(0,0,0));
        surfaceNormal.normalize();

        Vertex normalComponent = Vertex(surfaceNormal.x,surfaceNormal.y,surfaceNormal.z);
        normalComponent.mult(dot(wind,surfaceNormal));
        Vertex tangentForce = Vertex(wind.x,wind.y,wind.z);
        tangentForce.sub(normalComponent);
        tangentForce.mult(0.1*globalMultiplier);
        
        pAcceleration.add(tangentForce);
   

        for(uint16_t i = 0; i<(mother->seedCount); i++){
            if((mother->seeds)[i].id != id){
                AngularDistance d = calcAngularDistance(pappus,(mother->seeds)[i].pappus,mother->flowerRadius);
                if(d.distance < (mother->pappusRadius)){
                    float collisionAmount = (mother->pappusRadius) - d.distance;
                    float mag = collisionAmount/(mother->pappusRadius);
                    d.direction.mult(-mag*0.1*globalMultiplier);
                    pAcceleration.add(d.direction);
                }
            }
        }

        pVelocity.add(pAcceleration);
        Vertex vRadial = Vertex(surfaceNormal.x,surfaceNormal.y,surfaceNormal.z);
        vRadial.mult(dot(pVelocity,surfaceNormal));
        pVelocity.sub(vRadial);

        Vertex stemDirection = Vertex(pappus.x,pappus.y,pappus.z);
        stemDirection.sub(achene);
        float mag = stemDirection.getMag();
        stemDirection.normalize();
        tension = dot(wind,stemDirection);
        if((tension*mag) > (mother->breakLimit)){
            attached = false;
        }

        pappus.add(pVelocity);

        pappus.normalize();
        pappus.mult(mother->flowerRadius);

        achene = Vertex(pappus.x,pappus.y,pappus.z);
        achene.setMag((mother->flowerRadius) - (mother->seedLength));
    }
    else{
        pAcceleration.add(wind);
        pAcceleration.add(Vertex(0.01,0,0));
    
        for(uint16_t i = 0; i<(mother->seedCount); i++){
            if((mother->seeds)[i].id != id){
                AngularDistance d = calcAngularDistance(pappus,(mother->seeds)[i].pappus,mother->flowerRadius);
                if(d.distance < (mother->pappusRadius)){
                    float collisionAmount = (mother->pappusRadius) - d.distance;
                    float mag = collisionAmount/(mother->pappusRadius);
                    d.direction.mult(-mag*0.1*globalMultiplier);
                    pAcceleration.add(d.direction);
                }
            }
        }

        pAcceleration.mult(0.1);
        pVelocity.add(pAcceleration);
        pappus.add(pVelocity);

        const float k = 0.85;
        const float damping = 0.1;
        Vertex delta = Vertex(pappus.x,pappus.y,pappus.z);
        delta.sub(achene);
        float dist = delta.getMag();
        if(dist > 0){
            float stretch = dist - mother->seedLength;
            delta.normalize();
            delta.mult(stretch*k*globalMultiplier);
            aAcceleration.add(delta);
        }

        Vertex gravity = Vertex(0.9*globalMultiplier,0.0,0);
        aAcceleration.add(gravity);
        aVelocity.add(aAcceleration);
        aVelocity.mult(damping);
        achene.add(aVelocity);
    }
}

// #define PI 3.14159265359

Dandelion::Dandelion(){
    seedCount = 100;
    flowerRadius = 50;
    seedLength = 15;
    centerPosition = Vertex(0,0,0);
    pappusRadius = 4.0*PI*float(flowerRadius)/float(seedCount);
    breakLimit = 0.1;
    windVal = 0.0;
    for(uint16_t i = 0; i<seedCount; i++){
        float theta = 2.0*PI*random();
        float phi = 2.0*PI*random();
        Vertex coordinate = Vertex(flowerRadius*sin(phi)*cos(theta),flowerRadius*sin(phi)*sin(theta),flowerRadius*cos(phi));
        seeds.push_back(Seed(this,i,coordinate));
    }
}
void Dandelion::render(){
    for(uint16_t i = 0; i<seedCount; i++){
        display.fillCircle(seeds[i].pappus.x*globalScale+16,seeds[i].pappus.y*globalScale+32,1,1);
        display.drawLine(seeds[i].pappus.x*globalScale+16,seeds[i].pappus.y*globalScale+32,seeds[i].achene.x*globalScale+16,seeds[i].achene.y*globalScale+32,1);
    }
}
void Dandelion::update(){
    windVal+=0.001;
    for(uint16_t i = 0; i<seedCount; i++){
        seeds[i].update(Vertex(windVal,0.0,0.0));
    }
}