const fs = require('fs');
const util = require('util');
const config = require("config");

const readdir = util.promisify(fs.readdir);
const readfile = util.promisify(fs.readFile);

let cred;

async function getPathCred () {
	if (process.platform == 'darwin') {usbRoot = '/Volumes';}
	else if (process.platform == 'linux') {usbRoot = '/media/pi';}
	
	let usbMedias = await readdir(usbRoot);
	
	for (let i=0; i < usbMedias.length; i++) {
		try {
			usbMediaPath = `${usbRoot}/${usbMedias[i]}`;
			let usbFiles = await readdir(usbMediaPath);
			for (let j=0; j<usbFiles[i].length; j++) {
				if (usbFiles[j] === 'credentialOverview.cred'){
					let credPath = `${usbMediaPath}/credentialOverview.cred`;
					console.log("credentials acquired");
					return credPath;
				}
			}
		} catch (error) {
			// console.log("error while reading the directory: ", error);	
		}
	}
}

async function readCredentials () {
	let credPath = await getPathCred();
	cred = undefined;
	try {
		cred = await readfile(credPath, 'utf8');
	} catch (error) {
		// console.log("error read cred: ", error);	
	}
}

function getCredentials () {
	// return cred;
	return config.get("cred");
}


exports.readCredentials = readCredentials;
exports.getCredentials = getCredentials;

//  c9723a8f84c6

