const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("api", {
  runModel: (iso) => ipcRenderer.invoke("run-model", iso)
});
