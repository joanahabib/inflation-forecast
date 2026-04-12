const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("api", {
  runModel: (filePath, iso) => ipcRenderer.invoke("run-model", filePath, iso),
  openFile: () => ipcRenderer.invoke("open-file")
});
