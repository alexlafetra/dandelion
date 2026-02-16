
let canvas;
let dandelion;
let pointCanvas;
let screenBuffer;

function setup(){
    canvas = createCanvas(min(windowWidth,500),min(windowHeight,1000),WEBGL);
    //to use screenbuffer, make sure w/h match the device ur sending it to
    // screenBuffer = new ScreenBuffer(width,height);

    // pointCanvas = createFramebuffer({depth:false,antialias:true,textureFiltering:NEAREST});

    dandelion = new Dandelion();
    pixelDensity(0.5);
    // ortho();
    // noSmooth();
    // pixelDensity(1);
}

function draw(){
//   orbitControl();
    background(0,0,255);
//   clear();
    dandelion.update();
    dandelion.render();
    // dandelion.renderToPointCanvas();
    // image(pointCanvas,-width/2,-height/2,width,height);
}