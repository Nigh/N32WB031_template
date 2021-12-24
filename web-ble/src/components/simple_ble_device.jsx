import React, { useEffect } from "react"
import { observer } from "mobx-react-lite"
import { Button, ButtonGroup } from "@mui/material"

const timeSyncMsg = () => {
	let array = []
	let now = new Date()
	array.push(Math.round(255 * Math.random()))
	array.push(10)
	array.push(0x01)
	array.push(0x02)
	array.push(now.getUTCFullYear() - 2000)
	array.push(now.getUTCMonth() + 1)
	array.push(now.getUTCDate())
	array.push(now.getUTCHours())
	array.push(now.getUTCMinutes())
	array.push(now.getUTCSeconds())
	array.push(8)
	array.push(0)
	return array
}

const led_on_msg = () => {
	let array = []
	array.push(0xb1)
	array.push(0xbf)
	array.push(0x20)
	array.push(1)
	return array
}
const led_off_msg = () => {
	let array = []
	array.push(0xb1)
	array.push(0xbf)
	array.push(0x20)
	array.push(0)
	return array
}
const led_blink_msg = () => {
	let array = []
	array.push(0xb1)
	array.push(0xbf)
	array.push(0x11)
	array.push(1)
	return array
}
const led_stop_blink_msg = () => {
	let array = []
	array.push(0xb1)
	array.push(0xbf)
	array.push(0x11)
	array.push(0)
	return array
}
const DeviceInfo = observer(({ localStore }) => {
	return (
		<div className="flex flex-col mt-0 mb-6 items-center font-mono">
			<span className="px-3 text-xl font-semibold text-white bg-red-700 min-w-full">
				{(localStore.device && localStore.device.name) || "-------"}
			</span>
			<span className="px-3 text-xs min-w-full">
				{"ID: "}
				{(localStore.device && localStore.device.id) || "--------"}
			</span>
		</div>
	)
})
const Device = observer(({ localStore }) => {
	if (localStore.device) {
		return (
			<div className="mt-3 border-red-700 border-2 rounded-lg">
				<DeviceInfo localStore={localStore} />
				<div className="container flex flex-col px-4 pb-2">
					<LedCtrl localStore={localStore} />
					<Button
						variant="contained"
						color="secondary"
						onClick={
							(localStore.connected && localStore.disconnect) ||
							localStore.connect
						}
					>
						{localStore.connected ? "Disconnect" : "Connect"}
					</Button>
				</div>
			</div>
		)
	}
	return null
})
const LedCtrl = ({ localStore }) => {
	const led2_on = () => {
		localStore.write(led_on_msg())
	}
	const led2_off = () => {
		localStore.write(led_off_msg())
	}
	const led1_blink = () => {
		localStore.write(led_blink_msg())
	}
	const led1_off = () => {
		localStore.write(led_stop_blink_msg())
	}
	if (localStore.connected) {
		return (
			<>
				<ButtonGroup
					className="mb-0 pb-0 py-2"
					color="secondary"
					variant="outlined"
					aria-label="outlined button group"
				>
					<Button className="w-1/2" color="success" onClick={led2_on}>
						LED2
						<br />
						ON
					</Button>
					<Button className="w-1/2" color="error" onClick={led2_off}>
						LED2
						<br />
						OFF
					</Button>
				</ButtonGroup>
				<ButtonGroup
					className="mt-0 pt-0 pb-2"
					color="secondary"
					variant="outlined"
					aria-label="outlined button group"
				>
					<Button
						className="w-1/2"
						color="success"
						onClick={led1_blink}
					>
						LED1
						<br />
						BLINK
					</Button>
					<Button className="w-1/2" color="error" onClick={led1_off}>
						LED1
						<br />
						OFF
					</Button>
				</ButtonGroup>
			</>
		)
	}
	return null
}
const DeviceControl = ({ ble_storage, board_storage }) => {
	return (
		<div className="flex flex-col my-2 min-w-40">
			<Button
				className="py-2"
				variant="contained"
				color="primary"
				onClick={ble_storage.scan}
			>
				{(ble_storage.device == null && "Scan") || "Rescan"}
			</Button>
			<Device className="py-2" localStore={ble_storage} />
		</div>
	)
}

export default observer(DeviceControl)
