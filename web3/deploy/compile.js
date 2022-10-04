import fs from 'fs'
import solc from 'solc'
/*
const saveAbi = (contractSourcePath, jsonPath) => {
  console.log('saveAbi')
}
const instantiateContract = (baseContractPath) => {
  console.log('saveAbi')
}
*/

const saveAbi = (contractSourcePath, jsonPath) => {
  const contractData = instantiateContract(contractSourcePath)
  const json = JSON.stringify(contractData, null, 2)
  fs.writeFileSync(jsonPath, json)
}
// returns a contract object compiled using solc
// baseContractPath: relative path of the base contract, i.e. "./BaseContract.sol"
const instantiateContract = (baseContractPath) => {
  const sources = {};
  compileImports(baseContractPath, sources);

  var input = {
    language: "Solidity",
    sources: sources,
    settings: {
      outputSelection: {
        "*": {
          "*": ["*"],
        },
      },
    },
  };

  const output = solc.compile(JSON.stringify(input));
  const contract = JSON.parse(output);

  console.log(`${baseContractPath} output compiled`)

  const contractName = Object.keys(contract.contracts[baseContractPath])[0]
  const bytecode =
    "0x" + contract.contracts[baseContractPath][contractName].evm.bytecode.object;

  const abi = contract.contracts[baseContractPath][contractName].abi;
  return {
    bytecode: bytecode,
    abi: abi,
  };
};

// returns sources: { "Contract.sol": { content: fs.readFileSync("pathName.sol",utf8)...}}
// using recursion
const compileImports = (root, sources) => {
  sources[root] = { content: fs.readFileSync(root, "utf8") };
  const imports = getNeededImports(root);
  for (let i = 0; i < imports.length; i++) {
    compileImports(imports[i], sources);
  }
};

// returns all the import paths in absolute path
const getNeededImports = (path) => {
  const file = fs.readFileSync(path, "utf8");
  const files = new Array();
  file
    .toString()
    .split("\n")
    .forEach(function (line, index, arr) {
      if (
        (index === arr.length - 1 && line === "") ||
        !line.trim().startsWith("import")
      ) {
        return;
      }
      // the import is legit
      const relativePath = line.substring(8, line.length - 2);
      const fullPath = buildFullPath(path, relativePath);
      files.push(fullPath);
    });
  return files;
};

// parent: node_modules/.../ERC721/ERC721.sol
// returns absolute path of a relative one using the parent path
const buildFullPath = (parent, path) => {
  let curDir = parent.substr(0, parent.lastIndexOf("/")); //i.e. ./node/.../ERC721
  if (path.startsWith("./")) {
    return curDir + "/" + path.substr(2);
  }

  while (path.startsWith("../")) {
    curDir = curDir.substr(0, curDir.lastIndexOf("/"));
    path = path.substr(3);
  }

  return curDir + "/" + path;
};

async function main(){
  console.log(process.argv)
  if (process.argv.length != 4) {
    console.log('Usage:')
    console.log('  node compile_oz.js input.sol output.json')
    return -1
  }
  const contract = saveAbi(process.argv[2], process.argv[3])
}

if (typeof require !== 'undefined' && require.main === module) {
  main().then( () => process.exit(0))
}

export default {
  instantiateContract,
  saveAbi
}


