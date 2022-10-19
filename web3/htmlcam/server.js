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

function parse_ticket_ocr_string(ocrStr) {
  const lines = ocrStr.split('\\n')

  console.log('\n','parse')
  for (let i in lines) {
    console.log(i, lines[i])
  }
  let id = ""
  let price = 0

  const valid = lines[1].replace("VALID FOR", "")
  let startZone = 0
  let endZone = 0
  for (let i in lines) {
    const l = lines[i]

    // id and price
    if (l.length > 25 && l.indexOf('$') > 20) {
      const x = l.split('$')
      const idStr = x[0].trim()
      let priceStr = x[1]
      if (!priceStr) {
        priceStr = ''
      }
      priceStr = priceStr.replace('*','')
      priceStr = priceStr.replace('"','')
      priceStr = priceStr.replace('\\','')
      price = Number(priceStr.trim())
      id = idStr
    }

    // start / end zone
    if (l.length == 1) {
      if (startZone == 0 ) {
        startZone = Number(l)
      }
      else {
        if (endZone == 0) {
          endZone = Number(l)
        }
      }
    }
  }
  return { ocr: ocrStr,
           valid: valid.trim(),
           startZone: startZone,
           endZone: endZone,
           ticketId: id,
           price: price,
           // pos: lines[8]
         }
}

app.post('/photo', (req, res) => {

  const form = formidable({ multiples: true });

  form.parse(req, (err, fields, files) => {
    if (err) {
      res.writeHead(err.httpCode || 400, { 'Content-Type': 'text/plain' });
      res.end(String(err));
      return;
    }
    const tmpPath = files.image[0].filepath
    console.log(tmpPath)
    fs.rename(tmpPath, imagePath, async ()=>{
      console.log('photo saved to', imagePath)
      // res.end(JSON.stringify({ fields, files }, null, 2));
      // Performs text detection on the local file
      const [result] = await client.textDetection(imagePath)
      const detections = result.textAnnotations;
      const text = JSON.stringify(detections[0].description, null,2)
      console.log('ocr output',text)
      const ticketData = parse_ticket_ocr_string(text)
      // console.log(JSON.stringify(detections, null, 2));
      console.log('ticket data',JSON.stringify(ticketData))
      // res.writeHead(200, { 'Content-Type': 'application/json' })
      res.json(ticketData)
    })

  });

  return;

 })

app.listen(port, '0.0.0.0', () => console.log(`http://localhost:${port}`))
