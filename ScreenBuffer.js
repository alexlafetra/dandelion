
class ScreenBuffer{
    constructor(w,h){
        this.connectButton = createButton("connect");
        this.connectButton.mousePressed(connect);
        this.width = w;
        this.height = h;
        this.grayScaleLevels = 1;
        this.bufferedFrames = [];
        for(let i = 0; i<this.grayScaleLevels; i++){
            this.bufferedFrames.push(new Uint8Array(w*h/8));
        }
        this.sendingFrame = 0;

        this.buffer = new Uint8Array(w*h/8);
        this.connected = false;
        this.port = createSerial();
        this.options = {
            baudRate:9600,
            bufferSize:w*h/8
        };
        //automatically reconnecting
        let usedPorts = usedSerialPorts();
        if (usedPorts.length > 0) {
            this.port.open(usedPorts[0], 9600);
        }
    }
    logInfo(){
        if(this.port.opened()){
            console.log("----- Connected! ---------")
            console.log("baud: "+this.baudRate);
            let info = this.port.port.getInfo();
            console.log("Vendor ID:  "+info.usbVendorID);
            console.log("Product ID: "+info.usbProductID);
            console.log("--------------------------")
        }
        else{
            console.log("-- no ports connected --");
        }
    }
    connect(){
        if(this.port.opened())
            return;
        this.port.open(this.options.baudRate);
        this.connected = true;
        this.logInfo();
    }
    disconnect(){
        if(!this.port.opened())
            return;
        this.port.close();
        this.connected = false;
    }
    updateBuffer(){
        loadPixels();
        //brightness steps
        const step = 255/this.grayScaleLevels;
        let px = 0;
        const w = 128;
        const h = 64;
        for(let y = 0; y<h; y+= 8){
            for(let x = 0; x < w; x++){
                for(let frame = 0; frame<this.bufferedFrames.length; frame++){
                    let byte = 0;
                    for(let bit = 0; bit<8; bit++){
                        const brightness = pixels[x*4 + w*(bit+y)*4 + 3] / step;
                        byte |= (brightness > frame)<<bit;
                    }
                    this.bufferedFrames[frame][px] = byte;
                }
                px++;
            }
        }
    }
    run(){
        let data = this.port.readBytes();
        this.updateBuffer();
        // this.port.write(this.buffer);
        this.port.write(this.bufferedFrames[this.sendingFrame]);
        this.sendingFrame++;
        this.sendingFrame %= this.bufferedFrames.length;
    }
}

function connect(){
    screenBuffer.connect();
    screenBuffer.connectButton.mousePressed(disconnect);
    screenBuffer.connectButton.elt.innerText = "disconnect";
}

function disconnect(){
    screenBuffer.disconnect();
    screenBuffer.connectButton.mousePressed(connect);
    screenBuffer.connectButton.elt.innerText = "connect";
}