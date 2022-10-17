import express from 'express'
import fs from 'fs'
import formidable from 'formidable'

import { fileURLToPath } from 'url'
import { dirname } from 'path'

const __filename = fileURLToPath(import.meta.url)
const __dirname = dirname(__filename)


const app = express()
const port = 4000

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
    fs.rename(tmpPath, imagePath, ()=>{
      console.log('photo saved to', imagePath)
      res.end(JSON.stringify({ fields, files }, null, 2));

    })

  });

  return;

 })

app.listen(port, '0.0.0.0', () => console.log(`http://localhost:${port}`))
