const fs = require('fs')
const Web3 = require('web3')
const dotenv = require('dotenv')

// load contract abi


async function call_method(contract_abi, contract_address, method, methodParam) {
  const network = process.env.POLYGON_NETWORK
  const web3 = new Web3 (
    new Web3.providers.HttpProvider(
      `https://${network}.infura.io/v3/${process.env.INFURA_API_KEY}`)
  )

  let txUrl = ""
  const contract = new web3.eth.Contract( contract_abi, contract_address)

  const signer = web3.eth.accounts.privateKeyToAccount(process.env.SIGNER_PRIVATE_KEY)
  web3.eth.accounts.wallet.add(signer)

  const tx = contract.methods.echo(methodParam)
  const receipt = await tx.send({
      from: signer.address,
      gas: await tx.estimateGas(),
    })
    .once('transactionHash', (txhash) => {
      console.log(`Mining transaction ...`)
      const chain = network.replace('polygon-', '')
      txUrl = `https://${chain}.polygonscan.com/tx/${txhash}`
      // console.log(txUrl)
    })
  // done
  console.log(`Mined in block ${receipt.blockNumber}`)

  // console.log(past_events)
  return {
    block: receipt.blockNumber,
    url: txUrl,
    receipt: receipt
  }

  //  .then(results => console.log(results))
  //  .catch(err => {throw err})
}

async function get_events(contract_abi, contract_address, event_name) {

  console.log('\nEvents:')
  const network = process.env.POLYGON_NETWORK
  const web3 = new Web3 (
    new Web3.providers.HttpProvider(
      `https://${network}.infura.io/v3/${process.env.INFURA_API_KEY}`)
  )

  const contract = new web3.eth.Contract( contract_abi, contract_address)
  const blockFrom = await web3.eth.getBlockNumber() - 3000
  const opts ={fromBlock: blockFrom, toBlock: 'latest', filter: {}}
  const past_events = await contract.getPastEvents(event_name, opts)

  return past_events
}

dotenv.config()
// main()

async function main() {
  console.log(process.argv);
  contract_json = process.argv[2]
  contract_address = process.argv[3]
  method = process.argv[4]
  param = process.argv[5]

  const { abi } = JSON.parse(fs.readFileSync(contract_json));
  results = await call_method(abi, contract_address, method, param)

  console.log('Call results:')
  console.log(` - mined in block ${results.block}`)
  console.log(` - url: ${results.url}`)

  const past_events = await get_events(abi, contract_address, 'Echo')
  console.log(past_events)
  for (i in past_events) {
    const ev = past_events[i]
    const res = JSON.stringify(ev.returnValues)
    console.log(`    - ${i} Event: ${res}`)
  }
}


if (typeof require !== 'undefined' && require.main === module) {
  // deploy_contract(contract_json).then(() => process.exit(0))
  main().then(()=> process.exit(0))
}

