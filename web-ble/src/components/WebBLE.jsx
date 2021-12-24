import { makeObservable, observable, autorun, runInAction } from "mobx"

const makeMsg = (array) => {
	return new Uint8Array(array).buffer
}

function getSupportedProperties(characteristic) {
	let supportedProperties = []
	for (const p in characteristic.properties) {
		if (characteristic.properties[p] === true) {
			supportedProperties.push(p.toUpperCase())
		}
	}
	return "[" + supportedProperties.join(", ") + "]"
}

class WebBLE {
	static device
	static connected
	static connect_statu

	static scan
	static connect
	static disconnect
	static write
	static notify_regist
	static notifyHandler
	static notifyDispatcher
	static write_characteristic

	static logStoreDetails

	constructor() {
		this.device = null
		this.connected = false
		this.connect_statu = "Disconnected"
		this.notifyDispatcher = []
		this.scan = () => {
			navigator.bluetooth
				.requestDevice({
					filters: [{ namePrefix: "N32WB" }],
					optionalServices: [0xcc00],
				})
				.then((getdevice) => {
					console.log("> Name:", getdevice.name)
					console.log("> Id:", getdevice.id)
					console.log("> Connected:", getdevice.gatt.connected)
					console.log(getdevice)
					runInAction(()=>{this.device = getdevice})
				})
				.catch((error) => {
					console.log("Argh! " + error)
				})
		}
		this.connect = () => {
			console.log("Connecting to GATT Server...")
			runInAction(()=>{this.connect_statu = "Connecting"})
			this.device.gatt
				.connect()
				.then((server) => {
					console.log("Getting Services...")
					runInAction(()=>{this.connect_statu = "Getting Services"})
					return server.getPrimaryServices()
				})
				.then((services) => {
					console.log("Getting Characteristics...")
					runInAction(()=>{this.connect_statu = "Getting Characteristics"})
					let queue = Promise.resolve()
					services.forEach((service) => {
						queue = queue.then((_) =>
							service
								.getCharacteristics()
								.then((characteristics) => {
									console.log("> Service: " + service.uuid)
									characteristics.forEach(
										(characteristic) => {
											console.log(
												">> Characteristic: ",
												characteristic.uuid,
												getSupportedProperties(
													characteristic
												)
											)
											console.log(characteristic)
											if (
												characteristic.uuid.includes(
													"0000cc02"
												)
											) {
												characteristic.startNotifications()
												// characteristic.oncharacteristicvaluechanged
												characteristic.addEventListener(
													"characteristicvaluechanged",
													this.notifyHandler
												)
											}
											if (
												characteristic.uuid.includes(
													"0000cc01"
												)
											) {
												// android端在第一次write操作前需要delay一段时间。否则报错。
												// setTimeout(() => {
												// 	characteristic.writeValueWithoutResponse(
												// 		makeMsg(timeSyncMsg())
												// 	)
												// }, 1000)
												this.write_characteristic =
													characteristic
											}
										}
									)
								})
						)
					})
					return queue
				})
				.then(() => {
					runInAction(() => {
						this.connected = true
						this.connect_statu = "Connected"
					})
				})
				.catch((error) => {
					console.log("Argh! " + error)
				})
		}
		this.disconnect = () => {
			console.log("Disconnecting...")
			this.device.gatt.disconnect()
			runInAction(() => {
				this.connected = false
				this.connect_statu = "Disconnected"
			})
			// runInAction(()=>{
			// this.device = null
			// this.write_characteristic = null
			// })
		}
		this.write = (arr) => {
			this.write_characteristic.writeValueWithoutResponse(makeMsg(arr))
		}
		this.notify_regist = (handler) => {
			this.notifyDispatcher.push(handler)
		}
		this.write_characteristic = null
		this.notifyHandler = (event) => {
			const value = event.target.value
			// console.log("Received", value)
			this.notifyDispatcher.forEach((handler) => {
				handler(value)
			})
		}

		this.logStoreDetails = () => {
			console.log("logStoreDetails", this.connect_statu)
			// console.log("logStoreDetails", this.device)
		}

		makeObservable(this, {
			device: observable,
			connected: observable,
			connect_statu: observable,
			write_characteristic: observable,
		})
		autorun(this.logStoreDetails)
	}
}

export default WebBLE
