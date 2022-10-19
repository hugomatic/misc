import express from 'express'
import fs from 'fs'
import formidable from 'formidable'
import dotenv from 'dotenv'
import vision from '@google-cloud/vision'
import { fileURLToPath } from 'url'
import { dirname } from 'path'
import phone from './photo_phone.js'

const __filename = fileURLToPath(import.meta.url)
const __dirname = dirname(__filename)

// Creates a client
const client = new vision.ImageAnnotatorClient();

dotenv.config()

const app = express()
const port = 4444

app.use(express.static('public'))
app.post('/photo', phone.photoRequest)

app.listen(port, '0.0.0.0', () => console.log(`http://localhost:${port}`))
