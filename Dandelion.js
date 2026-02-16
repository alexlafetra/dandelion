/*

okay so i'm thinking that only the pappus should get 'moved' when the
seed is attached. it should be pushed by the wind, and pulled by the achene to remain close to length
but, when the force hits breaking point, it should unattach

pappi should also repel eachother, to stay a min distance

*/

class Seed{
  constructor(motherFlower,radius){

    this.renderStyle = motherFlower.renderStyle;
    this.stemThickness = motherFlower.acheneTubeThickness;
    this.centerPosition = motherFlower.centerPosition;
    
    //stem length
    this.flowerRadius = motherFlower.flowerRadius;
    this.seedLength = motherFlower.seedLength;
    
    //pappus radius
    this.radius = radius;

    //pappus (tuft)
    const theta = random(0,TWO_PI);
    const phi = random(0,TWO_PI);
    this.pappus = createVector(this.flowerRadius*sin(phi)*cos(theta),this.flowerRadius*sin(phi)*sin(theta),this.flowerRadius*cos(phi));
    this.pVelocity = createVector(0,0,0);
    this.pAcceleration = createVector(0,0,0);

    //achene (seed)
    this.achene = this.pappus.copy().setMag(this.flowerRadius - this.seedLength);
    this.aVelocity = createVector(0,0,0);
    this.aAcceleration = createVector(0,0,0);

    this.attached = true;
    this.breakLimit = 0.03;
    this.tension = 0;
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
    // stroke(map(this.pVelocity.mag(),0,10,0,100),100,100);
    stroke(map(this.tension,0,this.breakLimit,100,0),100,100);
    noFill();
    rect(topL.x - gap + this.centerPosition.x,topL.y - gap + this.centerPosition.y,bottomR.x - topL.x + 2*gap,bottomR.y - topL.y + 2*gap);

    pointCanvas.end();
  }
  //draw a point at each pappus
  renderPoints(){
    pointCanvas.begin();
    colorMode(HSB,100);
    push();
    stroke(map(this.pVelocity.mag(),0,10,0,100),100,100);
    translate(this.pappus.x + this.centerPosition.x,this.pappus.y + this.centerPosition.y,this.pappus.z + this.centerPosition.z);
    strokeWeight(0.1);
    point(0,0);
    pop();
    pointCanvas.end();
  }
  update(wind,otherSeeds){
    if(this.attached){
      //if u stay attached, then ur acceleration is just the tangent force along the sphere surface
      const surfaceNormal = p5.Vector.sub(this.pappus, createVector(0,0,0)).normalize();
      // component of force along the normal
      const normalComponent = surfaceNormal.copy().mult(wind.dot(surfaceNormal));
      const tangentForce = p5.Vector.sub(wind, normalComponent);
      
      this.pAcceleration = p5.Vector.add(this.pAcceleration,tangentForce.mult(0.1));

      //let pappi repel one another
      for(let s of otherSeeds){
        if(s != this){
          //get angular distance (geodesic)
          const pA = this.pappus.copy().normalize();
          const pB = s.pappus.copy().normalize();
          //vector direction along the arc connecting the two points
          let dir = pB.copy().sub(pA.copy().mult(pA.dot(pB)));
          dir.normalize();

          const angle = acos(constrain(pA.dot(pB),-1,1));
          const surfaceDist = angle * this.flowerRadius;

          //if they're too close
          if(surfaceDist < this.radius){
            let collisionAmount = this.radius - surfaceDist;
            let mag = collisionAmount/this.radius;
            let repulsion = dir.mult(-mag*0.1);
            this.pAcceleration.add(repulsion);
          }
        }
      }

      this.pAcceleration.mult(0.2);

      // const random = createVector(2*noise(this.pappus.x*100)-1,2*noise(this.pappus.y*200)-1,2*noise(this.pappus.z*50)-1);
      // random.mult(0.01);
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
      const random = createVector(2*noise(this.pappus.x*100)-1,2*noise(this.pappus.y*200)-1,2*noise(this.pappus.z*50)-1);
      random.mult(0.05);
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
            let repulsion = p5.Vector.sub(s.pappus,this.pappus).mult(-mag*0.1);
            this.pAcceleration.add(repulsion);
          }
        }
      }
      this.pAcceleration.mult(0.1);

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
        const force = dir.mult(stretch * k);
        
        this.aAcceleration.add(force);
      }
      //gravity to give the achene some heft
      const gravity = createVector(0,0.9,0);
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

    const avgSeedRadius = 4*PI*this.flowerRadius/this.seedCount;

    this.seedGeometry = this.buildPappusGeometry(avgSeedRadius*4,12);
    
    for(let i = 0; i<this.seedCount; i++){
      this.seeds.push(new Seed(this,avgSeedRadius*0.9));
    }
    this.stem = [
      createVector(0,0,0),
      createVector(width/16,height/4,0),
      createVector(0,height/2,0)
    ];
  }
  buildPappusGeometry(radius,segments){
    const tiltAngle = TWO_PI/12;

    let geometry = new p5.Geometry();

    for(let i = 0; i<segments; i++){
      //angle/position of each filament
      const a = i/segments*TWO_PI;
      const y = cos(a) * radius;
      const z = sin(a) * radius;

      //perpendicular offset for thickness
      const nY = -sin(a) * this.filamentThickness/2;
      const nZ = cos(a) * this.filamentThickness/2;

      const v = geometry.vertices.length;
      const lengthVariation = random(-radius/2,radius/2);
      const angleVariation = random(-tiltAngle/2,tiltAngle/2);
      //skinny quad, instead of a line to work w p5js
      geometry.vertices.push(
        createVector(0,  nY,  nZ),
        createVector(0, -nY, -nZ),
        createVector(-sin(tiltAngle+angleVariation)*(radius+lengthVariation), y - nY, z - nZ),
        createVector(-sin(tiltAngle+angleVariation)*(radius+lengthVariation), y + nY, z + nZ)
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
    pointCanvas.begin();
    clear();
    pointCanvas.end();

    for(let s of this.seeds){
      s.renderPoints();
    }

  }
  update(){
    let wind = createVector(0,0,0);
      if(mouseIsPressed){
        wind = createVector(mouseX-width/2-this.centerPosition.x,mouseY-height/2-this.centerPosition.y,0);
        this.currentWind.add(wind).mult(0.0001);
      }
      else{
        this.currentWind.mult(0.6);
      }
    for(let s of this.seeds){
      s.update(this.currentWind,this.seeds);
    }
  }
}