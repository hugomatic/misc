const fs = require('fs')
const Web3 = require('web3')
const dotenv = require('dotenv')

// load contract abi


async function call_method(contract_json, contract_address, method, methodParam) {

  const { abi } = JSON.parse(fs.readFileSync(contract_json));

  const network = process.env.POLYGON_NETWORK
  const web3 = new Web3 (
    new Web3.providers.HttpProvider(
      `https://${network}.infura.io/v3/${process.env.INFURA_API_KEY}`))

  const signer = web3.eth.accounts.privateKeyToAccount(process.env.SIGNER_PRIVATE_KEY)
  web3.eth.accounts.wallet.add(signer)

  let txUrl = ""
  const contract = new web3.eth.Contract( abi, contract_address)
  const tx = contract.methods.echo(methodParam)
  const receipt = await tx.send({
      from: signer.address,
      gas: await tx.estimateGas(),
    })
    .once('transactionHash', (txhash) => {
      console.log(`Mining transaction ...`)
      const chain = network.replace('polygon-', '')
      txUrl = `https://${chain}.polygonscan.com/tx/${txhash}`
      console.log(txUrl)
      console.log
    })
  // done
  console.log(`Mined in block ${receipt.blockNumber}`)

  console.log('\nEvents:')
  const opts ={fromBlock: receipt.blockNumber-3000, toBlock: 'latest', filter: {}}
  const past_events = await contract.getPastEvents('Echo', opts)
  console.log(past_events)

  return {
    block: receipt.blockNumber,
    url: txUrl,
    events: past_events
  }

  //  .then(results => console.log(results))
  //  .catch(err => {throw err})
}


dotenv.config()
// main()

async function main() {
  console.log(process.argv);
  contract_json = process.argv[2]
  contract_address = process.argv[3]
  method = process.argv[4]
  param = process.argv[5]

  await call_method(contract_json, contract_address, method, param)
}

if (typeof require !== 'undefined' && require.main === module) {
  // deploy_contract(contract_json).then(() => process.exit(0))
  main().then(()=> process.exit(0))
}

