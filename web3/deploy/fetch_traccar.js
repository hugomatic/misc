import dotenv from 'dotenv'
import {createHash} from "crypto"

dotenv.config()

function getMd5(data) {
  const hash = createHash("md5")
  const jsonString = JSON.stringify(data)
  hash.update(jsonString)
  const md5 = hash.digest("hex")
  return md5
}

const username = process.env.traccar_username
const password = process.env.traccar_password
const url = process.env.traccar_url

const credentials = btoa(`${username}:${password}`);

const opts = {
  method: 'GET',
  headers: {
    'Content-Type': 'application/json',
    'Accept': 'application/json',
    Authorization: `Basic ${credentials}`
  }
}

async function getAllDevices() {
  const finalUrl = url + '/api/devices?all=true'
  const response = await fetch(finalUrl, opts)
  const data = await response.json()
  return data
}

function generateTripsUrl(devId, days) {
  const encodeQueryData = (data) => {
    const ret = [];
    for (let d in data) {
      const encoded = data[d] // encodeURIComponent(data[d]).replace("/%3A/g", ":")
      ret.push(encodeURIComponent(d) + '=' + encoded)
    }
    return ret.join('&');
  }
  const now = new Date()
  // format "yyyy-mm-dd"
  const to_date = "" + now.getFullYear()
		+ "-" + String((now.getMonth() +1)).padStart(2, '0')
		+ "-"  + String(now.getDate()).padStart(2,'0')
  const from = new Date()
  // substract a nb of days from now
  from.setDate(now.getDate() - days)
  // format "yyyy-mm-dd"
  const from_date = "" + from.getFullYear()
		+ "-" + String((from.getMonth() +1 )).padStart(2,'0')
		+ "-"  + String(from.getDate()).padStart(2,'0')
  const from_time = from_date + 'T00:00:00.000Z'
  const to_time = to_date + 'T23:59:59.000Z'
  const parameters = {
	          deviceId: devId, // Change device id here
	          from: from_time,
	          to: to_time
	    }
  // GET parameters as query string : "user_api_hash=xxxxxx..."
  const get_request_args = encodeQueryData(parameters)
  const finalUrl = url + '/api/reports/trips?' + get_request_args
  return finalUrl
}

async function getTrips(devId, days) {
  const finalUrl = generateTripsUrl(devId, days)
  const response = await fetch(finalUrl, opts)
  const data = await response.json()
  return data
}

// export default main
export {getAllDevices, getTrips, getMd5}
