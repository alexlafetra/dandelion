
let canvas;
let dandelion;
let pointCanvas;
let screenBuffer;

function setup(){
    canvas = createCanvas(min(windowWidth,1000),min(windowHeight,1000),WEBGL);
    //to use screenbuffer, make sure w/h match the device ur sending it to
    // screenBuffer = new ScreenBuffer(width,height);

    // pointCanvas = createFramebuffer({depth:false,antialias:true,textureFiltering:NEAREST});

    dandelion = new Dandelion();
    pixelDensity(0.5);
    ortho();
    // noSmooth();
    // pixelDensity(1);
}

function draw(){
    if(keyIsDown(SHIFT) || touches.length > 1)
        orbitControl();
    clear();
    dandelion.update();
    dandelion.render();
    // dandelion.renderToPointCanvas();
    // image(pointCanvas,-width/2,-height/2,width,height);
}