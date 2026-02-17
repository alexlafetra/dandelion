
let canvas;
let dandelion;
let screenBuffer;

function setup(){

    //grabbing some visual settings from url
    const urlParams = new URLSearchParams(window.location.search);//uses the ? string following the url
    const settings = {
        drawPoints: urlParams.has('points'),
        drawBoundingBoxes : urlParams.has('boxes')||urlParams.has('boundingboxes')||urlParams.has('boundingBoxes'),
        highlightColor : urlParams.get('color')
    };
    console.log(settings);

    //creating canvas to fit window
    canvas = createCanvas(min(windowWidth,1000),min(windowHeight,1000),WEBGL);

    //code for sending screen data via USB to pi pico/other usb device
    //to use screenbuffer, make sure w/h match the device ur sending it to
    // screenBuffer = new ScreenBuffer(width,height);

    dandelion = new Dandelion(settings);
    pixelDensity(0.5);
    ortho();
    // noSmooth();
    // pixelDensity(1);
}

function draw(){
    // orbitControl();
    clear();
    dandelion.update();
    dandelion.render();
}