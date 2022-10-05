import express from 'express'
import Web3 from 'web3'
import dotenv from 'dotenv'
import compile from './compile.js'
import erc20token from './erc20token.js'

// Define "require"
// import { createRequire } from "module";
// const require = createRequire(import.meta.url);

dotenv.config()

const app = express()
const port = 4000

app.use(express.static('public'))

const web3 = new Web3 (
  new Web3.providers.HttpProvider(
    `https://${process.env.POLYGON_NETWORK}.infura.io/v3/${process.env.INFURA_API_KEY}`)
)


const signer = web3.eth.accounts.privateKeyToAccount(process.env['SIGNER_PRIVATE_KEY'])
app.get('/contract', (req, res) => {
  res.json({
    address: process.env['TOKEN_CONTRACT'],
    account: signer.address
  })
})

app.get('/name', (req, res) => {
  erc20token.name().then((name)=> {res.send(name)})
})

app.get('/symbol', (req, res) => {
  erc20token.symbol().then((symbol)=> {res.send(symbol)})
})

app.get('/totalSupply', (req, res) => {
  erc20token.totalSupply().then((sup)=> {res.send(sup)})
})

app.get('/balanceOf', (req, res) => {
  const account = req.query.account
  erc20token.balanceOf(account).then((result)=> {res.send(result)})
})

app.get('/events', (req, res) => {
  const eventName = req.query.eventName
  const nbOfBlocks = req.query.nbOfBlocks
  erc20token.getEvents(eventName, nbOfBlocks).then((result)=>{res.send(result)})
})

app.post('/mint', (req, res) => {
  const to = req.query.to
  const amount = req.query.amount
  erc20token.mint(to, amount).then((result) => {res.send(result)})
})

app.post('/transfer', (req, res) => {
  const to = req.query.to
  const amount = req.query.amount
  erc20token.transfer(to, amount).then((result) => {res.send(result)})
})

app.listen(port, () => console.log(`http://localhost:${port}`))
