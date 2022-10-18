import express from 'express'
import fs from 'fs'
import formidable from 'formidable'
import dotenv from 'dotenv'
import vision from '@google-cloud/vision'
import { fileURLToPath } from 'url'
import { dirname } from 'path'

const __filename = fileURLToPath(import.meta.url)
const __dirname = dirname(__filename)

// Creates a client
const client = new vision.ImageAnnotatorClient();

dotenv.config()

const app = express()
const port = 4444

const imagePath = __dirname + '/data/image.jpg'
console.log('SAVING to ', imagePath)

app.use(express.static('public'))

app.post('/photo', (req, res) => {

  const form = formidable({ multiples: true });

  form.parse(req, (err, fields, files) => {
    if (err) {
      res.writeHead(err.httpCode || 400, { 'Content-Type': 'text/plain' });
      res.end(String(err));
      return;
    }
    res.writeHead(200, { 'Content-Type': 'application/json' })
    const tmpPath = files.image[0].filepath
    console.log(tmpPath)
    fs.rename(tmpPath, imagePath, async ()=>{
      console.log('photo saved to', imagePath)
      // res.end(JSON.stringify({ fields, files }, null, 2));
      // Performs text detection on the local file
      const [result] = await client.textDetection(imagePath)
      const detections = result.textAnnotations;
      console.log('Text:');
      detections.forEach(text => console.log(text));
      res.end(JSON.stringify(detections[0].description, null, 2))
    })

  });

  return;

 })

app.listen(port, '0.0.0.0', () => console.log(`http://localhost:${port}`))
