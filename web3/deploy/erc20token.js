const fs = require('fs')
const Web3 = require('web3')
const dotenv = require('dotenv')
const compile = require('./compile_oz')
// load contract abi

dotenv.config()

const contractInstance = compile.instantiateContract('./O2club1.sol')
const web3 = new Web3 (
  new Web3.providers.HttpProvider(
    `https://${process.env.POLYGON_NETWORK}.infura.io/v3/${process.env.INFURA_API_KEY}`)
)

const contract = new web3.eth.Contract(contractInstance.abi,
                                       process.env.TOKEN_CONTRACT)
const signer = web3.eth.accounts.privateKeyToAccount(
                                       process.env.SIGNER_PRIVATE_KEY)

web3.eth.accounts.wallet.add(signer)


// internal function that returns the initialized contract.
// can be used to debug with the REPL, access contract functionality that is not
// wrapped
function getContract() {
  return contract;
}

// wrapper for the token mint function (gas tx fees apply)
async function mint(to, amount) {

  const tx = contract.methods.mint( to, amount)
  const gas = await tx.estimateGas({from: signer.address})

  let txUrl = ""
  const receipt = await tx.send({
      from: signer.address,
      gas: gas,
    })
    .once('transactionHash', (txhash) => {
      console.log(`Mining transaction ...`)
      const chain = process.env.POLYGON_NETWORK.replace('polygon-', '')
      txUrl = `https://${chain}.polygonscan.com/tx/${txhash}`
    })
  // done
  console.log(
    `tx: "mint" [to: "${to}", ] mined in block ${receipt.blockNumber} ${txUrl}`)
  return {
    block: receipt.blockNumber,
    url: txUrl,
    receipt: receipt
  }
}

// wrapper for balanceOf (free call)
async function balanceOf(account) {
  const msg = contract.methods.balanceOf(account)
  const result = await msg.call()
  return result
}

// wrapper for totalSupply (free call)
async function totalSupply() {
  const msg = contract.methods.totalSupply()
  const result = await msg.call()
  return result
}

// wrapper to get token symbol (free call)
async function symbol() {
  const msg = contract.methods.symbol()
  const result = await msg.call()
  return result
}

// wrapper to get name of the contract (free call)
async function name() {
  const msg = contract.methods.name()
  const result = await msg.call()
  return result
}

// wrapper for token transfer (gas tx fees apply)
async function transfer(to, amount) {

  const tx = contract.methods.transfer( to, amount)
  const gas = await tx.estimateGas({from: signer.address})
  let txUrl = ""
  const receipt = await tx.send({
      from: signer.address,
      gas: gas,
    })
    .once('transactionHash', (txhash) => {
      console.log(`Mining transaction ...`)
      const chain = process.env.POLYGON_NETWORK.replace('polygon-', '')
      txUrl = `https://${chain}.polygonscan.com/tx/${txhash}`
    })
  // done
  console.log(
    `tx "transfer" [to: ${to}, amount: ${amount}] Mined in block ${receipt.blockNumber}`
    ` ${txUrl}`
  )

  return {
    block: receipt.blockNumber,
    url: txUrl,
    receipt: receipt
  }

  //  .then(results => console.log(results))
  //  .catch(err => {throw err})
}

// look in the blockchain for latest events (free call).
// Infura has a limit of 3500 past blocks (Polygon) to look into
// It can take time for the envent (ex Transfer event) to show up after
// the tx has been mined (ex transfer tx)
async function getEvents(event_name, nbOfBlocks) {
  if (!nbOfBlocks )
    nbOfBlocks = 3000
  if (nbOfBlocks > 3500)
    nbOfBlocks = 3000
  const blockFrom = await web3.eth.getBlockNumber() - 3000
  const opts ={fromBlock: blockFrom, toBlock: 'latest', filter: {}}
  const past_events = await contract.getPastEvents(event_name, opts)
  return past_events
}

module.exports = {
  transfer,
  mint,
  balanceOf,
  totalSupply,
  symbol,
  name,
  getContract,
  getEvents
}

// main()

function usage() {
 console.log(`

Transfer O2Club token to account

Usage:

 cmd (transfer or mint)
 to (address)
 amount (decimal number)

ex:

node erc20token.js 0x964Db2077df70FcE54F91F03F277b46088999B8a 1000000000000000000
`)
}

async function main() {
  console.log(process.argv);
  if (process.argv.length <= 4) {
    usage()
    return -1
  }
  const cmd = process.argv[2]
  const to = process.argv[3]


  // const { abi } = JSON.parse(fs.readFileSync(contract_json));
  let results = null
  if (cmd == "transfer") {
    const amount = process.argv[4]
    results = await transfer(to, amount)
  }
  else if (cmd == "mint") {
    const amount = process.argv[4]
    results = await mint(to, amount)
  }
  else if (cmd == "balance") {
    results = await balance(to)
  }
  else {
    console.log(`Error: command ${cmd} unknown`)
    return -2
  }
  console.log('Call results:')
  console.log(` - mined in block ${results.block}`)
  console.log(` - url: ${results.url}`)

  const past_events = await get_events(contractInstance, process.env.TOKEN_CONTRACT, 'Transfer')
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

// contract methods:
//  'DEFAULT_ADMIN_ROLE()',
//  'MINTER_ROLE()',
//  'allowance(address,address)',
//  'approve(address,uint256)',
//  'balanceOf(address)',
//  'decimals()',
//  'decreaseAllowance(address,uint256)',
//  'getRoleAdmin(bytes32)',
//  'grantRole(bytes32,address)',
//  'hasRole(bytes32,address)',
//  'increaseAllowance(address,uint256)',
//  'mint(address,uint256)',
//  'name()',
//  'renounceRole(bytes32,address)',
//  'revokeRole(bytes32,address)',
//  'supportsInterface(bytes4)',
//  'symbol()',
//  'totalSupply()',
//  'transfer(address,uint256)',
//  'transferFrom(address,address,uint256)'

// contract events:
//  'Approval',
//  'RoleAdminChanged(bytes32,bytes32,bytes32)',
//  'RoleGranted(bytes32,address,address)',
//  'RoleRevoked(bytes32,address,address)',
//  'Transfer(address,address,uint256)',
