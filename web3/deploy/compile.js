const fs = require('fs').promises
const solc = require('solc')

async function compile_contract(contract_path, output_json) {
  const sourceCode = await fs.readFile(contract_path, 'utf8')
  const { abi, bytecode } = compile(sourceCode, 'Demo')
  // store in json
  const artifact = JSON.stringify({ abi, bytecode }, null, 2)
  await fs.writeFile(output_json, artifact)
}

function compile( sourceCode, contractName) {
  const input = {
    language: 'Solidity',
    sources: { main: { content: sourceCode } },
    settings: { outputSelection: { '*': { '*': ['abi', 'evm.bytecode'] } } },
  }
  // parse output
  const output = solc.compile(JSON.stringify(input))
  const artifact = JSON.parse(output).contracts.main[contractName]

  console.log('contract compiled')
  return {
    abi: artifact.abi,
    bytecode: artifact.evm.bytecode.object
  }
}

compile_contract('Demo.sol', 'Demo.json').then(() => process.exit(0))

