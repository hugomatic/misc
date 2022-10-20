
# Setup

npm install

You need a .env file with the following keys:

POLYGON_NETWORK = "polygon-mumbai"
INFURA_API_KEY = "xxxxxx"
// this is secret!!!
SIGNER_PRIVATE_KEY = "xxxxx"
CONTRACT_SRC = "./erc20token.sol"
TOKEN_CONTRACT = "xxxx"
// where to send tokens from caltrains demo
DST_ACCOUNT = "xxx"
// this is for the google vision api (OCR to read ticket data from phone pict)
GOOGLE_APPLICATION_CREDENTIALS="../goog.json"

# Running

## problem with contracts:

The compile script doesn't like the contract files inside the node_module
directory.

copy the @openzepelin directory from inside the node_modules directory to the
root directory.

cp -r node_modules/@openzeppelin/ .

## run server

npm start

navigate to http://localhost:4000

## run as scripts

You can run deploy.js compile.js and erc20token.js as scripts from the cmd line.


