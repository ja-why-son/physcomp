
// Setup Web Serial using serial.js
const serial = new Serial();
serial.on(SerialEvents.CONNECTION_OPENED, onSerialConnectionOpened);
serial.on(SerialEvents.CONNECTION_CLOSED, onSerialConnectionClosed);
serial.on(SerialEvents.DATA_RECEIVED, onSerialDataReceived);
serial.on(SerialEvents.ERROR_OCCURRED, onSerialErrorOccurred);

async function onButtonConnectToSerialDevice() {
    console.log("onButtonConnectToSerialDevice");
    if (!serial.isOpen()) {
        await serial.connectAndOpen();
    }
}

async function onButtonDisconnectToSerialDevice() {
    if (serial.isOpen()) {
        await serial.close();
    }
}

function onSerialErrorOccurred(eventSender, error) {
    console.log("onSerialErrorOccurred", error);
}

function onSerialConnectionOpened(eventSender) {
    console.log("onSerialConnectionOpened", eventSender);
}

function onSerialConnectionClosed(eventSender) {
    console.log("onSerialConnectionClosed", eventSender);
}

function onSerialDataReceived(eventSender, newData) {
    console.log("onSerialDataReceived", newData);
    if (newData === "buttonPressed") {
        disperse = !disperse;
    } else {
        let raw = newData;
        let processed = raw.split(",");
        accX = processed[0];
        accY = processed[1];
        accZ = processed[2];
    }
}

async function onConnectButtonClick() {
    console.log("Connect button clicked!");
}

// Send text data over serial
async function serialWriteTextData(textData) {
    if (serial.isOpen()) {
        console.log("Writing to serial: ", textData);
        serial.writeLine(textData);
    }
}
