const { app, BrowserWindow, ipcMain, dialog } = require("electron");
const { execFile } = require("child_process");
const path = require("path");

function createWindow() {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      preload: path.join(__dirname, "preload.js")
    }
  });

  win.loadFile("index.html");
}

app.whenReady().then(createWindow);

ipcMain.handle("open-file", async () => {
  const result = await dialog.showOpenDialog({
    properies: ["openFile"],
    filters: [{ name: "CSV Files", extensions: ["csv"] }]
  });
  if (result.canceled) return null;

  return { filePath: result.filePaths[0] };
});
  
ipcMain.handle("run-model", async (event, filePath, iso) => {
  return new Promise((resolve) => {

    const exePath = path.join(__dirname, "..", "build", "influx_app");
    const dataPath = filePath || path.join(__dirname, "..", "data", "sample_inflation.csv");

    execFile(exePath, [dataPath, iso], (err, stdout, stderr) => {

      if (err) {
        console.error("EXEC ERROR:", err);
        console.error("STDERR:", stderr);
        return resolve({ forecast: [] });
      }

      try {
        resolve(JSON.parse(stdout));
      } catch {
        console.error("BAD OUTPUT:", stdout);
        resolve({ forecast: [] });
      }
    });
  });
});
