
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

function onSerialErrorOccurred(eventSender, error) {
    console.log("onSerialErrorOccurred", error);
}

function onSerialConnectionOpened(eventSender) {
    console.log("onSerialConnectionOpened", eventSender);
    serialWriteTextData("start");
}

function onSerialConnectionClosed(eventSender) {
    console.log("onSerialConnectionClosed", eventSender);
    serialWriteTextData("end");
}

function onSerialDataReceived(eventSender, newData) {
    console.log("onSerialDataReceived", newData);
    rcvdText.textContent = newData;
    if (newData === "buttonPressed") {
        disperse = !disperse;
    }
}

async function onConnectButtonClick() {
    console.log("Connect button clicked!");
}
const inputText = document.querySelector('input');
const outputText = document.getElementById('output-text');
const rcvdText = document.getElementById('received-text')

inputText.addEventListener('input', updateOutputText);

// Called automatically when the input textbox is updated
function updateOutputText(e) {
    outputText.textContent = e.target.value;
    serialWriteTextData(e.target.value);
}

// Send text data over serial
async function serialWriteTextData(textData) {
    if (serial.isOpen()) {
        console.log("Writing to serial: ", textData);
        serial.writeLine(textData);
    }
}
