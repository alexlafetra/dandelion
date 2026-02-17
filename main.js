
let canvas;
let dandelion;
let screenBuffer;
let microphone;
let loudest = 0;
let backgroundNoiseThreshold = 0.1;
let micLevel = 0;
let gyroPermission = undefined;
let settings;

function setup(){

    //grabbing some visual settings from url
    const urlParams = new URLSearchParams(window.location.search);//uses the ? string following the url
    settings = {
        drawPoints: urlParams.has('points'),
        drawBoundingBoxes : urlParams.has('boxes')||urlParams.has('boundingboxes')||urlParams.has('boundingBoxes'),
        highlightColor : urlParams.get('color'),
        interaction : urlParams.has('wish')?'blow':'touch'
    };

    //creating canvas to fit window
    canvas = createCanvas(windowWidth,windowHeight,WEBGL);
    canvas.mousePressed(requestAudioAndGyroAccess);

    if(settings.interaction == 'blow'){
        microphone = new p5.AudioIn();
        microphone.start();
    }

    //code for sending screen data via USB to pi pico/other usb device
    //to use screenbuffer, make sure w/h match the device ur sending it to
    // screenBuffer = new ScreenBuffer(width,height);

    dandelion = new Dandelion(settings);
    pixelDensity(0.5);
    ortho();
}

function draw(){
    // orbitControl();
    clear();
    dandelion.update();
    dandelion.render();
}


async function requestAudioAndGyroAccess(){
    userStartAudio();
    if(settings.interaction == 'blow' && !microphone.enabled){
        microphone.start();
        console.log('starting mic input!');
    }
    if(gyroPermission == undefined){
        //checking if it's an iphone
        //if there's a device orientation event object, and if requestPermission is a valid function
        if(typeof(DeviceOrientationEvent) !== 'undefined' && typeof(DeviceOrientationEvent.requestPermission) === 'function'){
            const permission = await DeviceOrientationEvent.requestPermission();
            gyroPermission = (permission === "granted");
        }
    }
}