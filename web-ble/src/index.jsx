import React from "react";
import ReactDOM from "react-dom";
import "./index.css";
import "virtual:windi.css";
import { registerSW } from "virtual:pwa-register";
import App from "./App";

const updateSW = registerSW({
  onNeedRefresh() {},
  onOfflineReady() {},
  onRegiterError(error) {}
});

ReactDOM.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
  document.getElementById("root")
);
