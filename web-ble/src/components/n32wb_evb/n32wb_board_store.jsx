import { makeObservable, observable, autorun, computed, action, runInAction } from "mobx"

class BoardStore {
	static io_statu

	io_statu_change = () => {
		// console.log("btn1", this.btn1)
		// console.log("btn2", this.btn2)
		// console.log("led1", this.led1)
		// console.log("led2", this.led2)
	}
	get btn1() {
		return (this.io_statu & 0x01) > 0
	}
	get btn2() {
		return (this.io_statu & 0x02) > 0
	}
	get led1() {
		return (this.io_statu & 0x04) > 0
	}
	get led2() {
		return (this.io_statu & 0x08) > 0
	}
	get btn_rst() {
		return false
	}
	get btn_wup() {
		return false
	}
	constructor() {
		this.io_statu = 0x00
		this.data_handler = (data) => {
			// console.log("data_handler:", data)
			// console.log(data.byteLength)
			let view = new Uint8Array(data.buffer)
			let logs = "Recv[" + data.byteLength + "]:"
			view.forEach(element => {
				logs += " " + element.toString(16).toUpperCase().padStart(2, 0)
			});
			// console.log(logs)
			if(view.length>=4) {
				if(view[0]==0xB1 &&
					view[1]==0xBF &&
					view[2]==0x00) {
						runInAction(()=>{this.io_statu = view[3]})
					}
			}
		}
		makeObservable(this, {
			io_statu: observable,
			btn1: computed,
			btn2: computed,
			btn_rst: computed,
			btn_wup: computed,
			led1: computed,
			led2: computed,
		})
		autorun(this.io_statu_change)
	}
}

export default BoardStore
