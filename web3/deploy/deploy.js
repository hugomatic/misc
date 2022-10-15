import Web3 from 'web3'
import fs from 'fs'
import dotenv from 'dotenv'
import compile from './compile.js'

export default {
  deploy_contract
}

async function deploy_contract(contractPath) {
  dotenv.config()

  // compile solidity source and get abi, bytecode
  const contractInstance = compile.instantiateContract(contractPath)
  const abi = contractInstance.abi
  const bytecode = contractInstance.bytecode

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
    gas: await deployTx.estimateGas({from: signer.address})
  }).once("transactionHash", (txhash) => {
    console.log(`Mining deployment transaction ...`)
    const chain = network.replace('polygon-', '')
    console.log(
      `"${contractPath}" deployed https://${chain}.polygonscan.com/tx/${txhash}`)
  })
  // the contract is now deployed on chain
  return deployedContract.options.address
}


async function main() {
  console.log(process.argv);
  if (process.argv.length != 3) {
    console.log('wrong nb of arguments')
    console.log('usage: node deploy.js ./contract.sol')
    return -1
  }
  contractPath = process.argv[2]
  console.log(`Deploying ${contractPath}`)

  const address = await deploy_contract(contractPath)
  console.log(`Contract deployed at address "${address}"`)
}

if (process.argv[1].indexOf("deploy.js") > -1){
  main().then(()=> process.exit(0))
}

