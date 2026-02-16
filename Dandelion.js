/*

okay so i'm thinking that only the pappus should get 'moved' when the
seed is attached. it should be pushed by the wind, and pulled by the achene to remain close to length
but, when the force hits breaking point, it should unattach

pappi should also repel eachother, to stay a min distance

*/

const globalMultiplier = 1.0;

class Seed{
  constructor(motherFlower,coordinate,radius){

    this.renderStyle = motherFlower.renderStyle;
    this.stemThickness = motherFlower.acheneTubeThickness;
    this.centerPosition = motherFlower.centerPosition;
    
    //stem length
    this.flowerRadius = motherFlower.flowerRadius;
    this.seedLength = motherFlower.seedLength;
    
    //pappus radius
    this.radius = radius;

    //pappus (tuft)
    this.pappusAnchorPosition = coordinate.copy();
    this.pappus = coordinate.copy();
    this.pVelocity = createVector(0,0,0);
    this.pAcceleration = createVector(0,0,0);

    //achene (seed)
    this.achene = this.pappus.copy().setMag(this.flowerRadius - this.seedLength);
    this.aVelocity = createVector(0,0,0);
    this.aAcceleration = createVector(0,0,0);

    this.attached = true;
    this.breakLimit = 0.03;
    this.tension = 0;
    this.drag = 0.2;
  }
  render(geometry){
    colorMode(RGB);
    // const fillColor = [255,map(this.tension,0,this.breakLimit,0,255),map(this.tension,0,this.breakLimit,0,255)];
    const fillColor = [255,map(this.tension,0,this.breakLimit,255,0),map(this.tension,0,this.breakLimit,255,0)];
    // const fillColor = 255;
    noFill();
    strokeWeight(this.stemThickness);
    stroke(fillColor);
    line(this.achene.x,this.achene.y,this.achene.z,this.pappus.x,this.pappus.y,this.pappus.z);
        
    // push();
    const dir = p5.Vector.sub(this.achene,this.pappus).normalize();
    // reference axis
    const from = createVector(1, 0, 0);
    // rotation axis and angle
    let axis = from.cross(dir);
    let angle = acos(constrain(from.dot(dir), -1, 1));
    if(this.renderStyle == 'points'){
      push();
      translate(this.pappus.x,this.pappus.y,this.pappus.z);
      stroke(fillColor);
      strokeWeight(this.radius*2);
      point(0,0);
      pop();
    }
    else if(this.renderStyle == 'model'){
      push();
      translate(this.pappus.x,this.pappus.y,this.pappus.z);
      if (axis.magSq() > 1e-6) {
        axis.normalize();
        rotate(angle, axis);
      }
      fill(fillColor);
      noStroke();
      model(geometry);
      pop();
    }
  }
  //draw a bounding box roughly around the pappus & achene
  renderBoundingBoxes(){
    pointCanvas.begin();
    strokeWeight(1);
    const gap = 10;
    const topL = {x:min(this.pappus.x,this.achene.x),y:min(this.pappus.y,this.achene.y)};
    const bottomR = {x:max(this.pappus.x,this.achene.x),y:max(this.pappus.y,this.achene.y)};
    colorMode(HSB,100);
    stroke(map(this.pVelocity.mag(),0,5,0,100),100,100);
    // stroke(map(this.tension,0,this.breakLimit,100,0),100,100);
    noFill();
    rect(topL.x - gap + this.centerPosition.x,topL.y - gap + this.centerPosition.y,bottomR.x - topL.x + 2*gap,bottomR.y - topL.y + 2*gap);

    pointCanvas.end();
  }
  //draw a point at each pappus
  renderPoints(){
    pointCanvas.begin();
    colorMode(HSB,100);
    push();
    stroke(map(this.pVelocity.mag(),0,5,0,100),100,100);
    translate(this.pappus.x + this.centerPosition.x,this.pappus.y + this.centerPosition.y,this.pappus.z + this.centerPosition.z);
    strokeWeight(0.1);
    point(0,0);
    pop();
    pointCanvas.end();
  }
  calcAngularDistance(v1,v2){
    //get angular distance (geodesic)
    const pA = v1.copy().normalize();
    const pB = v2.copy().normalize();
    //vector direction along the arc connecting the two points
    let dir = pB.copy().sub(pA.copy().mult(pA.dot(pB)));
    dir.normalize();

    const angle = acos(constrain(pA.dot(pB),-1,1));
    const surfaceDist = angle * this.flowerRadius;

    return {distance:surfaceDist,direction:dir};
  }
  update(wind,otherSeeds){
    if(this.attached){
      //if u stay attached, then ur acceleration is just the tangent force along the sphere surface
      const surfaceNormal = p5.Vector.sub(this.pappus, createVector(0,0,0)).normalize();
      // component of force along the normal
      const normalComponent = surfaceNormal.copy().mult(wind.dot(surfaceNormal));
      const tangentForce = p5.Vector.sub(wind, normalComponent);
      
      this.pAcceleration = p5.Vector.add(this.pAcceleration,tangentForce.mult(0.1*globalMultiplier));

      //let pappi repel one another
      for(let s of otherSeeds){
        if(s != this){
          //get angular distance (geodesic)
          const angular = this.calcAngularDistance(this.pappus,s.pappus);
          //if they're too close
          if(angular.distance < this.radius){
            let collisionAmount = this.radius - angular.distance;
            let mag = collisionAmount/this.radius;
            let repulsion = angular.direction.mult(-mag*0.1*globalMultiplier);
            this.pAcceleration.add(repulsion);
          }
        }
      }
      
      //spring coupling from the pappus to its original 'anchor point' to simulate
      //the stiffness of the seed stalk
      //eg: trying to get the seeds not to slide all around the sphere of the flower
      // const k_1 = 0.85;
      // const damping_1 = 1;
      // //direction isn't in a straight line, has to be along the surface of the sphere
      // const angular = this.calcAngularDistance(this.pappus,this.pappusAnchorPosition);
      // if(angular.distance != 0){
      //   const force = angular.direction.mult(angular.distance * k_1 * damping_1);
      //   this.aAcceleration.add(force);
      // }

      this.pAcceleration.mult(this.drag);

      // const random = createVector(noise(this.pappus.x)-0.5,noise(this.pappus.y)-0.5,noise(this.pappus.z)-0.5);
      // random.mult(0.0001);
      // this.pAcceleration.add(random);

      this.pVelocity.x += this.pAcceleration.x;
      this.pVelocity.y += this.pAcceleration.y;
      this.pVelocity.z += this.pAcceleration.z;

      //remove radial velocity
      let vRadial = surfaceNormal.copy().mult(this.pVelocity.dot(surfaceNormal));
      this.pVelocity.sub(vRadial);

      //check to see how much force is on the stem
      //should this be perpendicular force, instead of inline force? or both?
      const stemDirection = p5.Vector.sub(this.pappus, this.achene).normalize();
      this.tension = wind.dot(stemDirection);
      if(this.tension > this.breakLimit)
        this.attached = false;

      this.pappus.x += this.pVelocity.x;
      this.pappus.y += this.pVelocity.y;
      this.pappus.z += this.pVelocity.z;

      //normalize position to stay along sphere
      this.pappus
        .normalize()
        .mult(this.flowerRadius);

      //move achene to stay axial with the center of the flower
      this.achene = this.pappus.copy().setMag(this.flowerRadius - this.seedLength);
    }
    //detached, free-floating
    //achenes are 'dragged' by pappi, and achenes are also affected by gravity
    else{
      const random = createVector(noise(this.pappus.x)-0.5,noise(this.pappus.y)-0.5,noise(this.pappus.z)-0.5);
      random.mult(0.05*globalMultiplier);
      this.pAcceleration.add(random);

      //wind
      this.pAcceleration.add(wind);
      //gravity
      this.pAcceleration.add(createVector(0,0.01,0));
      //repulsion
      for(let s of otherSeeds){
        if(s != this){
          const dist = p5.Vector.dist(s.pappus,this.pappus);
          //if they're too close
          if(dist < this.radius){
            let collisionAmount = this.radius - dist;
            let mag = collisionAmount/this.radius;
            let repulsion = p5.Vector.sub(s.pappus,this.pappus).mult(-mag*0.1*globalMultiplier);
            this.pAcceleration.add(repulsion);
          }
        }
      }
      this.pAcceleration.mult(this.drag);

      this.pVelocity.x += this.pAcceleration.x;
      this.pVelocity.y += this.pAcceleration.y;
      this.pVelocity.z += this.pAcceleration.z;

      this.pappus.x += this.pVelocity.x;
      this.pappus.y += this.pVelocity.y;
      this.pappus.z += this.pVelocity.z;

      //simple hooke's law spring coupling to pull achene along with the pappus
      //spring stiffness
      const k = 0.8;
      const damping = 0.01;
      const delta = p5.Vector.sub(this.pappus,this.achene);
      const dist = delta.mag();
      if(dist != 0){
        const stretch = dist - this.seedLength;
        const dir = delta.copy().normalize();
        const force = dir.mult(stretch * k * globalMultiplier);
        
        this.aAcceleration.add(force);
      }
      //gravity to give the achene some heft
      const gravity = createVector(0,0.9*globalMultiplier,0);
      this.aAcceleration.add(gravity);
      this.aVelocity.add(this.aAcceleration);
      this.aVelocity.mult(damping);
      
      this.achene.add(this.aVelocity);
    }
  }
}


class Dandelion{
  constructor(){
    this.currentWind = createVector(0,0,0);

    this.seeds = [];
    this.seedCount = 300;
    this.flowerRadius = 100;
    this.centerPosition = createVector(0,0,0);
    // this.centerPosition = createVector(-width/4, height/4 - this.flowerRadius - 50,0);

    this.filamentThickness = 1;
    this.acheneTubeThickness = 1;
    this.seedLength = 25;

    this.renderStyle = 'model';

    this.avgSeedRadius = 4*PI*this.flowerRadius/this.seedCount;
    this.numberofPappusFilaments = 12;
    this.pappusCamberAngle = TWO_PI/12;
    this.seedGeometry = this.buildPappusGeometry();
    
    this.placeSeeds('random');

    this.stem = [
      createVector(0,0,0),
      createVector(width/16,height/4,0),
      createVector(0,height/2,0)
    ];
  }
  placeSeeds(placement){
    //evenly-spaced using the golden ratio
    //probably more flower-accurate, but less visually interesting
    if(placement == 'fibonacci'){
      // https://extremelearning.com.au/evenly-distributing-points-on-a-sphere/?utm_source=chatgpt.com
      /*
      their python implementation: 
        from numpy import arange, pi, sin, cos, arccos
        n = 50
        i = arange(0, n, dtype=float) + 0.5
        phi = arccos(1 - 2*i/n)
        goldenRatio = (1 + 5**0.5)/2
        theta = 2 pi * i / goldenRatio
        x, y, z = cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi);
      */
      const goldenRatio = (1 + sqrt(5))/2;
      const randAngle = TWO_PI/128;
      for(let i = 0; i<this.seedCount; i++){
        const phi = acos(1 - 2*i/this.seedCount) + random(-randAngle,randAngle);
        const theta = TWO_PI * i/goldenRatio + random(-randAngle,randAngle);
        const x = cos(theta) * sin(phi);
        const y = sin(theta) * sin(phi);
        const z = cos(phi);
        const coordinate = createVector(x*this.flowerRadius,y*this.flowerRadius,z*this.flowerRadius);
        this.seeds.push(new Seed(this,coordinate,this.avgSeedRadius*0.9));
      }
    }
    else if(placement == 'random'){
      for(let i = 0; i<this.seedCount; i++){
        const theta = random(0,TWO_PI);
        const phi = random(0,TWO_PI);
        const coordinate = createVector(this.flowerRadius*sin(phi)*cos(theta),this.flowerRadius*sin(phi)*sin(theta),this.flowerRadius*cos(phi));
        this.seeds.push(new Seed(this,coordinate,this.avgSeedRadius*0.9));
      }
    }
  }
  buildPappusGeometry(){
    const radius = this.avgSeedRadius*4;
    let geometry = new p5.Geometry();

    for(let i = 0; i<this.numberofPappusFilaments; i++){
      //angle/position of each filament
      const a = i/this.numberofPappusFilaments*TWO_PI;
      const y = cos(a) * radius;
      const z = sin(a) * radius;

      //perpendicular offset for thickness
      const nY = -sin(a) * this.filamentThickness/2;
      const nZ = cos(a) * this.filamentThickness/2;

      const v = geometry.vertices.length;
      const lengthVariation = random(-radius/2,radius/2);
      const angleVariation = random(-this.pappusCamberAngle/2,this.pappusCamberAngle/2);
      //skinny quad, instead of a line to work w p5js
      geometry.vertices.push(
        createVector(0,  nY,  nZ),
        createVector(0, -nY, -nZ),
        createVector(-sin(this.pappusCamberAngle+angleVariation)*(radius+lengthVariation), y - nY, z - nZ),
        createVector(-sin(this.pappusCamberAngle+angleVariation)*(radius+lengthVariation), y + nY, z + nZ)
      );

      // two triangles making up each quad
      geometry.faces.push([v, v + 1, v + 2]);
      geometry.faces.push([v, v + 2, v + 3]);
    }

    geometry.computeNormals();
    return geometry;
  }

  render(){
    push();
    translate(this.centerPosition.x,this.centerPosition.y,this.centerPosition.z);
    for(let s of this.seeds){
      s.render(this.seedGeometry);
    }
    pop();
  }
  renderToPointCanvas(){
    // pointCanvas.begin();
    // clear();
    // pointCanvas.end();

    for(let s of this.seeds){
      s.renderPoints();
      // s.renderBoundingBoxes();
    }

  }
  update(){
    let wind = createVector(0,0,0);
      if(mouseIsPressed && !(keyIsDown(SHIFT) || touches.length > 1)){
        wind = createVector(mouseX-width/2-this.centerPosition.x,mouseY-height/2-this.centerPosition.y,0);
        this.currentWind.add(wind).mult(0.0001);
      }
      else{
        this.currentWind.mult(0.2);
      }
    for(let s of this.seeds){
      s.update(this.currentWind,this.seeds);
    }
  }
}