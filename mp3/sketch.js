let video;
let poseNet;
let poses = [];

let m = []; //The Particles themselves

let count = 100; //How many particles to spawn

let spdLimit = 200; //The limit of each particle's speed in pixels per second

let disperse = false;

let gravityX = 0;
let gravityY = 0;

let accX = 0;
let accY = 0;
let accZ = 0;

let changeFactor = 3;

let boundTheta = 50;

let speedMultiplier = 2;

let colorLowerBound = 320;
let colorUpperBound = 420;
let lastColorChanged = Date.now();

function setup() {
  createCanvas(1000, 500);
  video = createCapture(VIDEO);
  video.size(width, height);

  // Create a new poseNet method with a single detection
  poseNet = ml5.poseNet(video, modelReady);
  // This sets up an event that fills the global variable "poses"
  // with an array every time new poses are detected
  poseNet.on('pose', function(results) {
    for (let i = 0; i < results.length; i++) {
      // For each pose detected, loop through all the keypoints
      let pose = results[i].pose;
      let rightWrist = pose.rightWrist;
      if (rightWrist.confidence > 0.2) {
        if (abs(rightWrist.x - gravityX) / width < 0.2
           && abs(rightWrist.y - gravityY) / height < 0.2
           && rightWrist.x <= width || rightWrist.x > 0
           && rightWrist.y <= height || rightWrist.y > 0){
          gravityX = rightWrist.x;
          gravityY = rightWrist.y;
        }
      }
      let oledX = limit((gravityX / width) * 128, 0, 128);
      let oledY = limit((gravityY / height) * 64, 0, 64);
      serialWriteTextData(Math.round(oledX) + "," + Math.round(oledY));
    }
  });
  // Hide the video element, and just show the canvas
  video.hide();

  colorMode(HSB, 360, 100, 100);
  m = [];
  for (i = 0; i < count; i++) {
    m.push(new Particle());
  }
  gravityX = width / 2;
  gravityY = height / 2;
}

function limit(number, min, max) {
  if (number > max) {
    return max;
  } else if (number < min) {
    return min;
  } else {
    return number;
  }
}

function modelReady() {
  select('#status').html('Model Loaded');
}

function draw() {
  translate(video.width, 0);
  scale(-1, 1);
  // image(video, 0, 0, width, height);

  let colorChanged = false;
  let now = Date.now();
  if (pow(accX, 2) + pow(accY, 2), + pow(accZ, 2) > 111 && now - lastColorChanged > 1000) {
    let middleColor = (colorLowerBound + colorUpperBound) / 2;
    let h = random(middleColor - 75, middleColor + 75);
    colorLowerBound = h - 50;
    colorUpperBound = h + 50;
    if (colorLowerBound > colorUpperBound) {
      let temp = colorLowerBound;
      colorLowerBound = colorUpperBound;
      colorUpperBound = temp;
    }
    lastColorChanged = Date.now();
    colorChanged = true;
  } 
  noStroke();
  fill(0, 0, 0, 0.1);
  rect(-1, -1, width + 1, height + 1);
  for (i = 0; i < m.length; i++) {
    let a = m[i];
    if (colorChanged) {
      a.updateColor();
    }
    a.update();
    a.draw();
  }
  fill(255, 0, 0);
  noStroke();
  ellipse(gravityX, gravityY, 10, 10);
}

class Particle {
  constructor() {
    this.pos = new p5.Vector(random(width), random(height));
    this.prev = new p5.Vector(this.pos.x, this.pos.y);
    this.spd = new p5.Vector(random(2), random(2));
    this.acc = new p5.Vector();
    this.turnFactor = random(3, 10);
    this.h = random(colorLowerBound, colorUpperBound) % 360;
    this.s = 90;
    this.b = 100;

  }

  update() {
    this.prev = new p5.Vector(this.pos.x, this.pos.y);
    if (this.pos.x > width + boundTheta || this.pos.x < 0 - boundTheta) {
      if (!disperse) {
        this.spd.x *= -0.9;
      } else {
        return;
      }
    }
    if (this.pos.y < 0 - boundTheta || this.pos.y > height + boundTheta) {
      if (!disperse) {
        this.spd.y *= -0.9;
      } else {
        return;
      }
    }
    let attraction = new p5.Vector(gravityX, gravityY);
    attraction.sub(this.pos);
    this.acc.set(attraction);
    this.acc.normalize();
    this.acc.div(this.turnFactor);
    this.spd.add(this.acc);
    this.spd.limit(spdLimit * (1 / 40));
    if (disperse) {
      this.pos.sub(this.spd);
    } else {
      this.pos.add(this.spd);
    }
  }

  draw() {
    noStroke();
    fill(color(this.h, this.s, this.b));
    ellipse(this.pos.x, this.pos.y, 10, 10);
    stroke(color(this.h, this.s, this.b));
  }

  updateColor() {
    this.h = random(colorLowerBound, colorUpperBound) % 360;
  }

  generateNewSubColor(og, input) {
    input = abs(input);
    return random(og - changeFactor * input / 2, og + changeFactor * input / 2);
  }
}
