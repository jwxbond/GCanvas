const { createCanvas, Image } = require('../export')
const canvas = createCanvas(400, 400);
const ctx = canvas.getContext('2d')

ctx.fillRect(0, 0, 150, 150) // Draw a rectangle with default settings

ctx.fillStyle = '#09F' // Make changes to the settings
ctx.fillRect(15, 15, 120, 120) // Draw a rectangle with new settings

var basePNG = canvas.toDataURL();
console.log("base64 png output:" + basePNG);



var base64 = canvas.toDataURL("image/jpeg");
console.log("base64 jpeg output:" + base64);

const fs = require('fs')
const path = require('path');
const out = fs.createWriteStream(path.join(__dirname, "..")+ '/todataurl.png');
const stream = canvas.createPNGStream();
stream.pipe(out);
