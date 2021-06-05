let video;
let poseNet;
let poses = [];

let m = []; //The Particles themselves

let count = 100; //How many particles to spawn

let spdLimit = 200; //The limit of each particle's speed in pixels per second

let disperse = false;

let gravityX = 0;
let gravityY = 0;

function setup() {
  createCanvas(640, 320);
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
        gravityX = rightWrist.x;
        gravityY = rightWrist.y;
      }
      serialWriteTextData(gravityX + "," + gravityY);
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

function modelReady() {
  select('#status').html('Model Loaded');
}

function draw() {
  translate(video.width, 0);
  scale(-1, 1);
  // image(video, 0, 0, width, height);

  // We can call both functions to draw all keypoints and the skeletons
  noStroke();
  fill(0, 0, 0, 0.1);
  rect(-1, -1, width + 1, height + 1);
  for (i = 0; i < m.length; i++) {
    let a = m[i];
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

    this.col = color(random(320, 420) % 360, 90, 100);
  }

  update() {
    this.prev = new p5.Vector(this.pos.x, this.pos.y);
    if (this.pos.x > width || this.pos.x < 0) {
      this.spd.x *= -0.9;
    }
    if (this.pos.y < 0 || this.pos.y > height) {
      this.spd.y *= -0.9;
    }
    let attraction = new p5.Vector(gravityX, gravityY);
    attraction.sub(this.pos);
    this.acc.set(attraction);
    this.acc.normalize();
    this.acc.div(this.turnFactor);
    this.spd.add(this.acc);
    this.spd.limit(spdLimit * (1 / 50));
    if (disperse) {
      this.pos.sub(this.spd);
    } else {
      this.pos.add(this.spd);
    }
  }

  draw() {
    noStroke();
    fill(this.col);
    ellipse(this.pos.x, this.pos.y, 10, 10);
    stroke(this.col);
  }
}
