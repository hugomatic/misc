const Web3 = require('web3')
const fs = require('fs')
const dotenv = require('dotenv')

async function deploy_contract(contract_json) {
  dotenv.config()

  const jsonData = fs.readFileSync(contract_json)
  const { abi, bytecode } = JSON.parse(jsonData)

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

  return deployedContract.options.address
}

async function main() {
  console.log(process.argv);
  contract_json = process.argv[2]
  console.log(`Deploying ${contract_json}`)

  const address = await deploy_contract(contract_json)
  console.log(`Contract deployed at address "${address}"`)
}

if (typeof require !== 'undefined' && require.main === module) {
  // deploy_contract(contract_json).then(() => process.exit(0))
  main().then(()=> process.exit(0))
}

