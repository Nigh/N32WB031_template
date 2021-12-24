import React, { useEffect } from "react"
import DeviceControl from "./components/simple_ble_device"
import EVB from "./components/n32wb_evb/n32wb_evb"
import WebBLE from "./components/WebBLE"
import BoardStore from "./components/n32wb_evb/n32wb_board_store"

function App() {
	const ble_storage = new WebBLE()
	const board_storage = new BoardStore()
	useEffect(() => {
		ble_storage.notify_regist(board_storage.data_handler)
	}, [])
	return (
		<div className="App">
			<div className="flex justify-center">
				<div className="flex flex-col justify-center items-center text-center text-white font-black h-auto w-300px md:w-400px rounded-2xl bg-red-500 ">
					<p className="text-2xl mt-2">
						N32WB EVB 评估工具
					</p>
					<p className="mb-1">v0.1.3</p>
				</div>
			</div>
			<div className="container flex mx-auto justify-center">
				<EVB ble_storage={ble_storage} board_storage={board_storage} />
			</div>
			<div className="container flex mx-auto justify-center">
				<DeviceControl
					ble_storage={ble_storage}
					board_storage={board_storage}
				/>
			</div>
		</div>
	)
}

export default App
