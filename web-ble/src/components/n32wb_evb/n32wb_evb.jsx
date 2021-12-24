import React, { useState } from "react"
import { observer } from "mobx-react-lite"
import board from "./board.webp"
import btn_up from "./btn_up.png"
import led_on from "./led_on.png"

import "./evb.css"

var bgStyle = {
	position: "relative",
	backgroundImage: "url(" + board + ")",
	backgroundPosition: "top",
	backgroundSize: "contain",
	backgroundRepeat: "no-repeat",
	// width: "60%",
	// height: "80%",
}

const Mask = observer(({ ble_storage }) => {
	if(ble_storage.connected==false){
	return (
		<div className="relative z-20 flex justify-center items-center h-full w-full rounded-2xl bg-gray-100/80">
			<div className="h-auto w-1/2 rounded-2xl bg-red-500 flex items-center justify-center">
				<p className="text-center text-white font-black h-8 flex items-center">
					{ble_storage.connect_statu}
				</p>
			</div>
		</div>
	)
	}
	return null
})

const EVB = ({ ble_storage, board_storage }) => {
	return (
		<>
			<div
				className="absolute min-w-300px min-h-447px md:min-w-400px md:min-h-596px blur-md"
				style={bgStyle}
			>
				<Mask ble_storage={ble_storage} />
				{!board_storage.btn1 && (
					<img className="btn1 z-10" src={btn_up} alt="" />
				)}
				{!board_storage.btn2 && (
					<img className="btn2 z-10" src={btn_up} alt="" />
				)}
				{!board_storage.btn_rst && (
					<img className="btn-rst z-10" src={btn_up} alt="" />
				)}
				{!board_storage.btn_wup && (
					<img className="btn-wup z-10" src={btn_up} alt="" />
				)}
				{board_storage.led1 && (
					<img className="led1 z-10" src={led_on} alt="" />
				)}
				{board_storage.led2 && (
					<img className="led2 z-10" src={led_on} alt="" />
				)}
			</div>
		</>
	)
}
export default observer(EVB)
