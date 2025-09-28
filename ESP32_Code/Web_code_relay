String getHTML() {
  String html = "<!DOCTYPE HTML><html><head>";
  html += "<style>";
  html += "body {background-color:#F0F0F0; font-family:Arial; text-align:center;}";
  html += "h1 {color:#333333;}";
  html += "button {width:150px; height:50px; font-size:20px; margin:10px;}";
  html += "</style></head><body>";
  html += "<h1>ESP32 Relay Control</h1>";
  html += "<p>Relay state: <span style='color:red;'>";
  if (relayState == LOW) html += "ON";
  else html += "OFF";
  html += "</span></p>";
  html += "<button onclick=\"window.location.href='/relay/on'\">Turn ON</button>";
  html += "<button onclick=\"window.location.href='/relay/off'\">Turn OFF</button>";
  html += "</body></html>";
  return html;
}
