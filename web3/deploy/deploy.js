const Web3 = require('web3')
const fs = require('fs')
const dotenv = require('dotenv')

const jsonData = fs.readFileSync('Demo.json')
const { abi, bytecode } = JSON.parse(jsonData)

async function main() {
  const network = process.env.POLYGON_NETWORK
  const web3 = new Web3(
    new Web3.providers.HttpProvider(
      `https://${network}.infura.io/v3/${process.env.INFURA_API_KEY}`
    )
  )

  // create signing account
  const signer = web3.eth.accounts.privateKeyToAccount(
    process.env.SIGNER_PRIVATE_KEY
  )
  web3.eth.accounts.wallet.add(signer)

  // using the signing account to deploy the contract
  const contract = new web3.eth.Contract(abi)
  contract.options.data = bytecode

  console.log('calling deploy to create network tx')
  const deployTx = contract.deploy()

  const deployedContract = await deployTx.send({
    from: signer.address,
    gas: await deployTx.estimateGas()
  }).once("transactionHash", (txhash) => {
    console.log(`Mining deployment transaction ...`)
    console.log(`https://${network}.etherscan.io/tx/${txhash}`)
  })
  // the contract is now deployed on chain
  console.log(`Contract deployed!`)
  console.log(`Add to the .env file to store contract address:`)
  console.log(`DEMO_CONTRACT = "${deployedContract.options.address}"`)
}

dotenv.config()
if (typeof require !== 'undefined' && require.main === module) {
  main().then(() => process.exit(0))
}

