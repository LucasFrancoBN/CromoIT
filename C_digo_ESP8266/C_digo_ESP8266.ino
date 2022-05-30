#include <ESP8266WiFi.h>

// Enter your wifi network name and Wifi Password
const char* ssid = "SILUCRIS";
const char* password = "sil300218";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// These variables store current output state of LED
String vermelho_estado = "off";
String verde_estado = "off";
String azul_estado = "off";
String laranja_estado = "off";
String magenta_estado = "off";
String amarelo_estado = "off";
String branco_estado = "off";
String turquesa_estado = "off";
//String colorPicker_estado = "off";

//Estado para o color Picker
String vermelhoCP = "0";
String verdeCP = "0";
String azulCP = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;


// Assign output variables to GPIO pins
const int vermelhoLED = 4;
const int verdeLED = 14;
const int azulLED = 12;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
Serial.begin(115200);
// Initialize the output variables as outputs
pinMode(vermelhoLED, OUTPUT);
pinMode(verdeLED, OUTPUT);
pinMode(azulLED,OUTPUT);
// Set outputs to LOW
digitalWrite(vermelhoLED, 0);
digitalWrite(verdeLED, 0);
digitalWrite(azulLED, 0);

// Connect to Wi-Fi network with SSID and password
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){
WiFiClient client = server.available(); // Listen for incoming clients

if (client) { // If a new client connects,
Serial.println("New Client."); // print a message out in the serial port
String currentLine = ""; // make a String to hold incoming data from the client
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
currentTime = millis(); 
if (client.available()) { // if there's bytes to read from the client,
char c = client.read(); // read a byte, then
Serial.write(c); // print it out the serial monitor
header += c;
if (c == '\n') { // if the byte is a newline character
// if the current line is blank, you got two newline characters in a row.
// that's the end of the client HTTP request, so send a response:
if (currentLine.length() == 0) {
// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
// and a content-type so the client knows what's coming, then a blank line:
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

// turns the GPIOs on and off
if (header.indexOf("GET /2/on") >= 0) {
estadoOff ();
vermelho_estado = "on";
RGB_cor(255, 0, 0);
} else if (header.indexOf("GET /2/off") >= 0) {
Serial.println("RED LED is off");
vermelho_estado = "off";
RGB_cor(0, 0, 0);
} 

else if (header.indexOf("GET /4/on") >= 0) {
Serial.println("Green LED is on");
estadoOff ();
verde_estado = "on";
RGB_cor(0, 255, 0);
} else if (header.indexOf("GET /4/off") >= 0) {
Serial.println("Green LED is off");
verde_estado = "off";
RGB_cor(0, 0, 0);
} 

else if (header.indexOf("GET /5/on") >= 0) {
Serial.println("Yellow LED is on");
estadoOff ();
azul_estado = "on";
RGB_cor(0, 0, 255);
} else if (header.indexOf("GET /5/off") >= 0) {
Serial.println("Yellow LED is off");
azul_estado = "off";
RGB_cor(0, 0, 0);
} 

else if (header.indexOf("GET /6/on") >= 0) {
Serial.println("laranja LED is on");
estadoOff ();
laranja_estado = "on";
RGB_cor(255, 30, 0);
} else if (header.indexOf("GET /6/off") >= 0) {
Serial.println("laranja LED is off");
laranja_estado = "off";
RGB_cor(0, 0, 0);
}

else if (header.indexOf("GET /7/on") >= 0) {
Serial.println("Magenta LED is on");
estadoOff ();
magenta_estado = "on";
RGB_cor(139, 0, 139);
} else if (header.indexOf("GET /7/off") >= 0) {
Serial.println("Magenta LED is off");
magenta_estado = "off";
RGB_cor(0, 0, 0);
} 

else if (header.indexOf("GET /8/on") >= 0) {
Serial.println("Amarelo LED is on");
estadoOff ();
amarelo_estado = "on";
RGB_cor(255, 100, 0);
} else if (header.indexOf("GET /8/off") >= 0) {
Serial.println("Amarelo LED is off");
amarelo_estado = "off";
RGB_cor(0, 0, 0);
} 

else if (header.indexOf("GET /9/on") >= 0) {
Serial.println("Branco LED is on");
estadoOff ();
branco_estado = "on";
RGB_cor(255, 255, 255);
} else if (header.indexOf("GET /9/off") >= 0) {
Serial.println("Branco LED is off");
branco_estado = "off";
RGB_cor(0, 0, 0);
} 

else if (header.indexOf("GET /1/on") >= 0) {
Serial.println("Turquesa LED is on");
estadoOff ();
turquesa_estado = "on";
RGB_cor(0, 206, 209);
} else if (header.indexOf("GET /1/off") >= 0) {
Serial.println("Turquesa LED is off");
turquesa_estado = "off";
RGB_cor(0, 0, 0);
}




// Display the HTML web page
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<meta charset=\"UTF-8\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
client.println("<title> CromoIT </title>");
// CSS to style the on/off buttons 
client.println("<style>@import url('https://fonts.googleapis.com/css2?family=Lato&family=Nunito:wght@400;700&display=swap');");
client.println(" *{margin: 0; padding: 0;}html{ scroll-behavior: smooth;}");
client.println("body { font-family: 'Lato', sans-serif; max-width: 1220px; margin: auto; background-color: #F8F8FF; color: #101010; font-size: 16px; line-height: 24px;}");
client.println("header {display: flex; justify-content: space-between; align-items: center; padding: 25px 0;}");
client.println("h1{ font-family: 'Nunito', sans-serif;}header nav {display: flex; align-items: center;}");
client.println("header nav ul{ display: flex; gap: 15px; flex-wrap: wrap; list-style: none;}nav ul li a { display: flex; flex-shrink: 1; flex-grow: 1; box-shadow: 0px 2px 2px #dcdcdc;}");
client.println("nav a{text-decoration: none; padding: 4px 8px; border: 2px #BFBBBB solid; border-radius: 3px; font-family: 'Nunito', sans-serif; font-weight: 700; color: #101010;}");
client.println("main{ margin: 25px 0;}section{ margin-bottom: 15px;}section h2 {margin-bottom: 1px;font-family: 'Nunito', sans-serif;}");
client.println("#testar{margin-bottom: 15px} #botoes{display: grid; grid-template: 1fr 1fr 1fr 1fr 1fr / 1fr 1fr;} #botoes div{margin-bottom: 5px;}");

client.println(".buttonRed { background-color: #ff0000; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonGreen { background-color: #008000; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonBlue { background-color: #0055ff; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonOrange { background-color: #ff8c00; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonMagenta { background-color: #a020f0; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonYellow { background-color: #feeb36; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonWhite { background-color: #ffffff; border: 2px #BFBBBB solid; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonTurquesa { background-color: #00cece; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println(".buttonOff { background-color: #77878A; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println("@media screen and (max-width: 1220px){body{margin:0 50px; max-width: 720px; font-size: 0.9em;}}");
client.println("@media screen and (max-width: 770px) {header{flex-direction: column; align-items: center; gap: 10px;} #botoes {display: flex; flex-direction: column; align-items: center; justify-content: center;} #botoes div {display: flex; flex-direction: column; align-items: center; justify-content: center;}}");
client.println("@media screen and (max-width: 570px){header nav ul{ flex-direction: column; align-items: center;}}</style></head>");

// Web Page Heading
client.println("<body><header><h1> CromoIT</h1><nav><ul><li><a href=\"#proposito\">""PROPÓSITO</a></li><li><a href=\"#testar\">""VAMOS TESTAR? :D</a></li><li><a href=\"#sobre\">""SOBRE NÓS</a></li><li><a href=\"#integrantes\">""INTEGRANTES</a></li></ul></nav></header>");

//MAIN
client.println("<main><section><h2 id=\"proposito\">"" Nosso Propósito</h2><p>Esse projeto foi pensado em como nós podemos melhorar a qualidade do nosso sono no geral, já que, nos dias atuais, nós estamos tendo cada vez mais dificuldade de ter uma boa noite de sono, o que pode alterar todo o nosso humor durante o dia, além de alterar a nossa produtividade.</p></section>");

// Display current state, and ON/OFF buttons for GPIO 2 Red LED
client.println("<h2 id=\"testar\">""Vamos testar? :D</h2><section id=\"botoes\">"); 
client.println("<div><p>O Led vermelho está " + vermelho_estado + "</p>");
// If the vermelho_estado is off, it displays the OFF button 
if (vermelho_estado=="off") {
client.println("<p><a href=\"/2/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/2/off\"><button class=\"button buttonRed\">ON</button></a></p></div>");
} 

// Display current state, and ON/OFF buttons for GPIO 4 Green LED 
client.println("<div><p>O Led verde está " + verde_estado + "</p>");
// If the verde_estado is off, it displays the OFF button 
if (verde_estado =="off") {
client.println("<p><a href=\"/4/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/4/off\"><button class=\"button buttonGreen\">ON</button></a></p></div>");
}
// Display current state, and ON/OFF buttons for GPIO 5 Yellow LED 
client.println("<div><p>O Led azul está " + azul_estado + "</p>");
// If the azul_estado is off, it displays the OFF button 
if (azul_estado =="off") {
client.println("<p><a href=\"/5/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/5/off\"><button class=\"button buttonBlue\">ON</button></a></p></div>");
}
//COR LARANJA DO LED
client.println("<div><p>O Led laranja está " + laranja_estado + "</p>");
// If the azul_estado is off, it displays the OFF button 
if (laranja_estado =="off") {
client.println("<p><a href=\"/6/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/6/off\"><button class=\"button buttonOrange\">ON</button></a></p></div>");
}
//COR MAGENTA
client.println("<div><p>O Led Magenta está " + magenta_estado + "</p>");
// If the azul_estado is off, it displays the OFF button 
if (magenta_estado =="off") {
client.println("<p><a href=\"/7/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/7/off\"><button class=\"button buttonMagenta\">ON</button></a></p></div>");
}
//COR AMARELO
client.println("<div><p>O Led Amarelo está " + amarelo_estado + "</p>");
// If the azul_estado is off, it displays the OFF button 
if (amarelo_estado =="off") {
client.println("<p><a href=\"/8/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/8/off\"><button class=\"button buttonYellow\">ON</button></a></p></div>");
}
//COR Branca
client.println("<div><p>O Led Branco está " + branco_estado + "</p>");
if (branco_estado =="off") {
client.println("<p><a href=\"/9/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/9/off\"><button class=\"button buttonWhite\">ON</button></a></p></div>");
}
//TURQUESA
client.println("<div><p>O Led Turquesa está " + turquesa_estado + "</p>");
if (turquesa_estado =="off") {
client.println("<p><a href=\"/1/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/1/off\"><button class=\"button buttonTurquesa\">ON</button></a></p></div>");
}

//COLOR PICKER
client.println("<div><p>Esolha a cor que você deseja :D</p>");
client.println("<a href=\"ColorPick\" id=\"change_color\"><button class=\"button buttonOff\">Change Color</button></a> ");
client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\">");
client.println("<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
client.println("document.getElementById(\"change_color\").href=\"ColorPick?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}</script>");
client.println("</div>");
if(header.indexOf("GET /1/ColorPick?r") >= 0 ||
   header.indexOf("GET /2/ColorPick?r") >= 0 || 
   header.indexOf("GET /3/ColorPick?r") >= 0 ||
   header.indexOf("GET /4/ColorPick?r") >= 0 || 
   header.indexOf("GET /5/ColorPick?r") >= 0 || 
   header.indexOf("GET /6/ColorPick?r") >= 0 ||
   header.indexOf("GET /7/ColorPick?r") >= 0 ||
   header.indexOf("GET /8/ColorPick?r") >= 0 ||
   header.indexOf("GET /9/ColorPick?r") >= 0 ||
   header.indexOf("GET /ColorPick?r") >= 0){
  estadoOff();
  pos1 = header.indexOf('r');
  pos2 = header.indexOf('g');
  pos3 = header.indexOf('b');
  pos4 = header.indexOf('&');
  vermelhoCP = header.substring(pos1+7, pos2);
  verdeCP = header.substring(pos2+1, pos3);
  azulCP = header.substring(pos3+1, pos4);
  analogWrite(vermelhoLED, vermelhoCP.toInt());
  analogWrite(verdeLED, verdeCP.toInt());
  analogWrite(azulLED, azulCP.toInt());
  Serial.println(vermelhoCP.toInt());
  Serial.println(vermelhoCP);
  Serial.println(verdeCP.toInt());
  Serial.println(azulCP.toInt());
}

//APAGA TODAS AS CORES
client.println("<div><p> Apague o Led aqui </p>");
client.println("<p><a href=\"/3/off\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
if(header.indexOf("GET /3/off") >=0){
  RGB_cor(0, 0, 0);
  estadoOff ();
}



client.println("</section>");

//SOBRE NÓS
client.println("<section><h2 id=\"sobre\">""Sobre nós</h2><p>Somos um grupo da turma A de Análise e Desenvolvimento da FECAP e estamos fazendo esse projeto interdisciplinar. Temos um total de 5 integrantes no grupo, cuja cada um deu diversas ideias sobre a realização desse mesmo e esse é o resultado final que vocês estão vendo :D</p></section>");

//INTEGRANTES
client.println("<section><h2 id=\"integrantes\">""Integrantes</h2><p>Lucas Franco - 22023179</p><p>Glauber Vinicius - 22023179</p><p> Esdras Mendes - 22023179</p><p>Beatriz Candido - 22023095</p><p>Julya Joplin - 22023461</p></section>");

//FIM DO HTML
client.println("</main></body></html>");

// The HTTP response ends with another blank line
client.println();
// Break out of the while loop
break;
} else { // if you got a newline, then clear currentLine
currentLine = "";
}
} else if (c != '\r') { // if you got anything else but a carriage return character,
currentLine += c; // add it to the end of the currentLine
}
}
}
// Clear the header variable
header = "";
// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}

void RGB_cor (int vermelho_valor, int verde_valor, int azul_valor) {
  analogWrite (vermelhoLED, vermelho_valor);
  analogWrite (verdeLED, verde_valor);
  analogWrite (azulLED, azul_valor);
}

void estadoOff () {
  vermelho_estado = "off";
  verde_estado = "off";
  azul_estado = "off";
  laranja_estado = "off";
  magenta_estado = "off";
  amarelo_estado = "off";
  branco_estado = "off";
  turquesa_estado = "off";
}
