const fs = require('fs')
const Web3 = require('web3')
const dotenv = require('dotenv')

// load contract abi
const { abi } = JSON.parse(fs.readFileSync("Demo.json"));


async function main() {
  const network = process.env.POLYGON_NETWORK
  const web3 = new Web3 (
    new Web3.providers.HttpProvider(
      `https://${network}.infura.io/v3/${process.env.INFURA_API_KEY}`))

  const signer = web3.eth.accounts.privateKeyToAccount(process.env.SIGNER_PRIVATE_KEY)
  web3.eth.accounts.wallet.add(signer)

  const contract = new web3.eth.Contract( abi, process.env.DEMO_CONTRACT)
  const tx = contract.methods.echo('Toto, world!')
  const receipt = await tx.send({
      from: signer.address,
      gas: await tx.estimateGas(),
    })
    .once('transactionHash', (txhash) => {
      console.log(`Mining transaction ...`)
      const chain = network.replace('polygon-', '')
      console.log(`https://${chain}.polygonscan.com/tx/${txhash}`)
    })
  // done
  console.log(`Mined in block ${receipt.blockNumber}`)

  console.log('\nEvents:')
  const opts ={fromBlock: receipt.blockNumber-3000, toBlock: 'latest', filter: {}}
  contract.getPastEvents('Echo', opts)
    .then(results => console.log(results))
    .catch(err => {throw err})
}

dotenv.config()
main()


