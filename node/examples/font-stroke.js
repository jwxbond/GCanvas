const { createCanvas, Image } = require('../export');
const canvas = createCanvas(320, 320);
const ctx = canvas.getContext('2d')
ctx.fillStyle="#ffffff"

ctx.fillRect(0,0,canvas.width,canvas.height)
ctx.font = "48px serif";
ctx.lineWidth=3
ctx.strokeText("Hello world", 50, 100);

const fs = require('fs')
const path = require('path')
const out = fs.createWriteStream(path.join(__dirname,"..") + '/font-stroke.png');
var stream = canvas.createPNGStream();
stream.pipe(out);