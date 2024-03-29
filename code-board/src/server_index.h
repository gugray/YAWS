#ifndef SERVER_INDEX_H
#define SERVER_INDEX_H

const char text[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>YAWS setup</title>
  <link rel="icon" href="data:;base64,iVBORw0KGgo=">
  <style>
    * { box-sizing: border-box; }
    html { height: 100%; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, avenir next, avenir, segoe ui, helvetica neue, helvetica, Cantarell, Ubuntu, roboto, noto, arial, sans-serif;
      font-size: 17px; padding: 0; margin: 0; min-height: 100%;
      display: flex; justify-content: center; background-color: white; color: #555;
    }
    main { max-width: 600px; flex-grow: 1; padding: 30px 10px; }
    h1 { font-size: 28px; margin: 0 0 20px 0; border-bottom: 1px solid #555; }
    h2 { font-size: 24px; margin: 24px 0 16px 0; }
    section.readings { display: flex; justify-content: space-between; }
    section.readings p { display: inline-block; margin: 0; width: 33%; }
    p.error { display: none; color: red; margin: 0 0 8px 0; }
    p.success { display: none; color: green; margin: 0 0 8px 0; }
    p.progress { display: none; color: #666; margin: 0 0 8px 0; }
    table, tr, td { padding: 0 0 8px 0; border: 0; }
    td.left { padding-right: 20px; }
    table p { margin: 0; }
    input[type=text] {
      font-family: inherit; font-size: 18px; color: inherit; background-color: inherit;
      box-shadow: none; -webkit-box-shadow: none; -webkit-appearance: none;
      border-radius: 3px; border: 1px solid #888; padding: 2px 5px;
      width: 80px; text-align: center;
    }
    button {
      font-family: inherit; font-size: inherit;
    }
  </style>
</head>
<body>
<script>

  document.addEventListener("DOMContentLoaded", () => {
    updateReadings();
    getConfig();
    document.getElementById("btnSave").addEventListener("click", onSaveConfig);
    document.getElementById("btnRestart").addEventListener("click", onRestart);
    document.getElementById("btnUpload").addEventListener("click", onUpload);
  });

  async function getConfig() {
    try {
      const response = await fetch("/config", {
        method: 'GET',
      });
      if (response.status != 200) throw "up";
      let data = await response.json();
      document.getElementById("txtPressureAdjust").setAttribute("value", data.ofsPres);
      document.getElementById("txtTempAdjust").setAttribute("value", data.ofsTemp);
      document.getElementById("errGetConfig").style.display = "none";
    }
    catch {
      document.getElementById("errGetConfig").style.display = "block";
    }
  }

  async function onRestart() {
    await fetch("/restart", {
      method: 'POST'
    });
  }

  async function onSaveConfig() {
    document.getElementById("doneSaveConfig").style.display = "none";
    let valuesOk = true;
    let vOfsPres = Number.parseFloat(document.getElementById("txtPressureAdjust").value);
    if (Number.isNaN(vOfsPres)) {
      document.getElementById("errPressureAdjust").style.display = "block";
      valuesOk = false;
    }
    else document.getElementById("errPressureAdjust").style.display = "none";
    let vOfsTemp = Number.parseFloat(document.getElementById("txtTempAdjust").value);
    if (Number.isNaN(vOfsTemp)) {
      document.getElementById("errTempAdjust").style.display = "block";
      valuesOk = false;
    }
    else document.getElementById("errTempAdjust").style.display = "none";
    if (!valuesOk)
      return;
    let config = {
      ofsPres: vOfsPres,
      ofsTemp: vOfsTemp,
    };
    try {
      const response = await fetch("/config", {
        method: 'POST',
        body: JSON.stringify(config)
      });
      if (response.status != 200) throw "up";
      document.getElementById("errSaveConfig").style.display = "none";
      document.getElementById("doneSaveConfig").style.display = "block";
    }
    catch {
      document.getElementById("errSaveConfig").style.display = "block";
    }
  }

  async function updateReadings() {
    const response = await fetch("/readings", {
      method: 'GET',
    });
    let gotReadings = false;
    if (response.status == 200) {
      try {
        let data = await response.json();
        document.getElementById("pres").innerText = Math.round(data.pres * 10) / 10 + " hPa";
        document.getElementById("humi").innerText = data.humi + "%";
        document.getElementById("temp").innerText = Math.round(data.temp * 10) / 10 + "°C";
        gotReadings = true;
      }
      catch {}
    }
    if (!gotReadings) {
      document.getElementById("pres").innerText = "n/a";
      document.getElementById("humi").innerText = "n/a";
      document.getElementById("temp").innerText = "n/a";
    }
    setTimeout(updateReadings, 500);
  }

  async function onUpload() {
    document.getElementById("errUpload").style.display = "none";
    document.getElementById("doneUpload").style.display = "none";
    document.getElementById("progUpload").style.display = "none";
    let elmFileInput = document.getElementById("sourceFileUpload");
    let formData = new FormData();
    formData.append("userfile", elmFileInput.files[0]);
    const request = new XMLHttpRequest();
    request.open("POST", "/upload", true);
    request.onload = () => {
      document.getElementById("progUpload").style.display = "none";
      if (request.status == 200)
        document.getElementById("doneUpload").style.display = "block";
      else
        document.getElementById("errUpload").style.display = "block";
    };
    document.getElementById("progUpload").style.display = "block";
    request.send(formData);
  }

</script>
<main>
  <h1>YAWS: Yet Another Weather Station</h1>
  <section class="readings">
    <p><b>Pressure: </b> <span id="pres">n/a</span></p>
    <p><b>Humidity: </b> <span id="humi">n/a</span></p>
    <p><b>Temperature: </b> <span id="temp">n/a</span></p>
  </section>
  <h2>Configure</h2>
  <p class="error" id="errGetConfig">Failed to load configuration from YAWS. Please try reloading the page.</p>
  <table>
    <tr valign="top">
      <td class="left">Pressure adjust:</td>
      <td>
        <p><input type="text" id="txtPressureAdjust"> hPa</p>
        <p class="error" id="errPressureAdjust">Must be a number.</p>
      </td>
    </tr>
    <tr valign="top">
      <td class="left">Temperature adjust:</td>
      <td>
        <p><input type="text" id="txtTempAdjust"> °C</p>
        <p class="error" id="errTempAdjust">Must be a number.</p>
      </td>
    </tr>
    <tr>
      <td class="left"></td>
      <td>
        <p><button id="btnSave">Save changes</button></p>
        <p class="error" id="errSaveConfig">Failed to save changes.</p>
        <p class="success" id="doneSaveConfig">Config changes saved.</p>
      </td>
    </tr>
  </table>
  <h2>Update firmware</h2>
  <table>
    <tr valign="top">
      <td class="left">Firmware.bin file:</td>
      <td>
        <input type="file" id="sourceFileUpload">
      </td>
    </tr>
    <tr>
      <td class="left"></td>
      <td>
        <p><button id="btnUpload">Upload and install</button></p>
        <p class="error" id="errUpload">Failed to upload file.</p>
        <p class="progress" id="progUpload">Uploading...</p>
        <p class="success" id="doneUpload">New firmware uploaded.</p>
      </td>
    </tr>
  </table>
  <h2>Restart YAWS</h2>
  <p>
    You can restart YAWS from this page. That will exit the web server mode and return to
    the weather station display.
    <br/><br/>
    <button id="btnRestart">Restart</button>
  </p>
</main>
</body>
</html>
)";

#endif
