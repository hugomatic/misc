import express from 'express'
import Web3 from 'web3'
import dotenv from 'dotenv'
import compile from './compile.js'
import path from 'path';
import {fileURLToPath} from 'url'
import YAML from 'yamljs'
import swaggerUi from 'swagger-ui-express'

import erc20token from './erc20token.js'
import deploy from './deploy.js'

import phone from './photo_phone.js'
import {getAllDevices, getTrips, getMd5} from './fetch_traccar.js'

// Define "require"
// import { createRequire } from "module";
// const require = createRequire(import.meta.url);

dotenv.config()

const app = express()
const port = 4000

app.use(express.static('public'))

// swagger docs route
const swaggerDocument = YAML.load('./swagger.yaml')
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocument))

// qr code library
app.get('/scripts/easy.qrcode.min.js', function(req, res) {
  const __filename = fileURLToPath(import.meta.url);
  const __dirname = path.dirname(__filename);
  //console.log(process.cwd())
  res.sendFile(__dirname + '/node_modules/easyqrcodejs/dist/easy.qrcode.min.js');
});

const web3 = new Web3 (
  new Web3.providers.HttpProvider(
    `https://${process.env.POLYGON_NETWORK}.infura.io/v3/${process.env.INFURA_API_KEY}`)
)

const signer = web3.eth.accounts.privateKeyToAccount(process.env['SIGNER_PRIVATE_KEY'])
app.get('/api/contract', (req, res) => {
  res.json({
    address: process.env['TOKEN_CONTRACT'],
    account: signer.address
  })
})

app.get('/api/name', (req, res) => {
  erc20token.name().then((name)=> {res.send(name)})
})

app.get('/api/symbol', (req, res) => {
  erc20token.symbol().then((symbol)=> {res.send(symbol)})
})

app.get('/api/totalSupply', (req, res) => {
  erc20token.totalSupply().then((sup)=> {res.send(sup)})
})

app.get('/api/balanceOf', (req, res) => {
  const account = req.query.account
  erc20token.balanceOf(account)
    .then((result)=> {res.send(result)})
    .catch((error) => {
      console.log('Error during balanceOf:', error)
      res.status(400).send({error: error.reason})
    })
})

app.get('/api/events', (req, res) => {
  const eventName = req.query.eventName
  const nbOfBlocks = req.query.nbOfBlocks
  erc20token.getEvents(eventName, nbOfBlocks).then((result)=>{res.send(result)})
})

app.post('/api/mint', (req, res) => {
  const to = req.query.to
  const amount = req.query.amount
  erc20token.mint(to, amount)
    .then((result) => {res.send(result)})
    .catch((error) => {
      let  stat = 500
      if (error.code == 'INVALID_ARGUMENT') {
        stat = 400
      }
      let er = error.reason
      if (!er) er = error
      console.log('Error during mint:', error)
      res.status(stat).send({error: '' + er})
    })
})

app.post('/api/transfer', (req, res) => {
  const to = req.query.to
  const amount = req.query.amount
  erc20token.transfer(to, amount)
    .then((result) => {res.send(result)})
    .catch((error) => {
      let  stat = 500
      if (error.code == 'INVALID_ARGUMENT') {
        stat = 400
      }
      console.log('Error during transfer:', error)
      res.status(stat).send({error: error.reason})
    })
})

app.post('/api/deploy', (req, res) => {
  const contractPath = process.env.CONTRACT_SRC
  console.log('deploy', contractPath)
  deploy.deploy_contract(contractPath)
    .then((address) => {
      const result = {contract:contractPath , address: address}
      console.log('deploy result', result)
      res.send(result)
    })
    .catch((error) => {
      let stat = 500
      console.log('Error during deploy:', error)
      res.status(stat).send({error: error.reason})
  })
})

app.post('/photo', phone.photoRequest, (req, res) => {
  console.log(res.ticketData)

  const to = process.env['DST_ACCOUNT']
  const amount = '' + (res.ticketData.price * 100) + '000000000000000'
  console.log('mint', amount,'tokens to ', to)

  erc20token.mint(to, amount)
    .then((result) => {
      console.log(result)
    })
    .catch((error) => {
      let  stat = 500
      if (error.code == 'INVALID_ARGUMENT') {
        stat = 400
      }
      console.log('Error during transfer:', error)
      res.status(stat).send({error: error.reason})
    })

  res.json({ticket: res.ticketData, mint: {amount: amount, to: to}})
})

let paid_trips = []

async function getTraccarTrips(){
  const trips = []
  const devId = 161
  const days = 20
  const tripData = await getTrips(devId, days)
  console.log(tripData.length, 'trips for dev', devId, 'in last', days, 'days')
  for (let i in tripData) {
    const trip = tripData[i]
    delete(trip.deviceId)
    delete(trip.startAddress)
    delete(trip.endAddress)
    delete(trip.driverUniqueId)
    delete(trip.driverName)
    delete(trip.spentFuel)
    const hash =  getMd5(trip)
    let paid = true
    if (!paid_trips.includes(hash)) {
      paid = false
    }
    trips.push({hash:hash, paid: paid, trip: trip})
  }
  const data = {
    server: process.env.traccar_url ,
    user: process.env.traccar_username,
    days: days,
    trips: trips
  }
  return data
}

app.get('/trips', async (req, res) => {
  const data = await getTraccarTrips()
  res.send(data)
})

app.get('/reset_trips', (req, res) => {
  const count = paid_trips.length
  paid_trips = []
  const r = {success:true, count:count}
  console.log('reset trips', r)
  res.send(r)
})

app.get('/process_trips', (req, res) => {
  consol.log()
})


app.listen(port, '0.0.0.0', () => console.log(`http://localhost:${port}`))
