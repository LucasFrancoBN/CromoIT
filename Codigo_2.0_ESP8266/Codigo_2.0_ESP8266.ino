#include <ESP8266WiFi.h>

// Entre com a sua rede wifi e a senha
const char* ssid = "SILUCRIS";
const char* password = "sil300218";

// Define a porta 80 para o servidor web
WiFiServer server(80);

//Variavel para armazenar a requsição HTTP
String header;

//Essas variaveis armazena o estado atual da saida dos LEDS
String vermelho_estado = "off";
String verde_estado = "off";
String azul_estado = "off";
String laranja_estado = "off";
String magenta_estado = "off";
String amarelo_estado = "off";
String branco_estado = "off";
String turquesa_estado = "off";

//Estado para o color Picker
String vermelhoCP = "0";
String verdeCP = "0";
String azulCP = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;


//Assinatura das variaveis de saidas para os pinos GPIO
const int vermelhoLED = 4;
const int verdeLED = 14;
const int azulLED = 12;

//Tempo atual
unsigned long currentTime = millis();

//Tempo decorrido
unsigned long previousTime = 0; 

//Define o tempo limite em milisegundos 
const long timeoutTime = 2000;

void setup() {
Serial.begin(115200);
//Inicializa as variaveis da saida dos leds
pinMode(vermelhoLED, OUTPUT);
pinMode(verdeLED, OUTPUT);
pinMode(azulLED,OUTPUT);
// Set outputs to LOW
digitalWrite(vermelhoLED, 0);
digitalWrite(verdeLED, 0);
digitalWrite(azulLED, 0);

// Conecta na rede wifi com usuario e senha
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
//Imprime o endereco de IP e inicia o servidor web
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){
WiFiClient client = server.available(); // Ouve o que esta vindo do cliente

if (client) { // Se um novo cliente conecta
Serial.println("New Client."); //Imprime uma mensagem na porta serial
String currentLine = ""; //Faca uma String para armazenar dados recebidos do cliente 
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeoutTime) { //Laco de repeticao while enquanto o cliente estiver conectado 
currentTime = millis(); 
if (client.available()) { //Se há bytes para ser lido do cliente
char c = client.read(); // Le o byte, entao
Serial.write(c); // Imprime uma mensagem na porta serial 
header += c;
if (c == '\n') { //Se byte for um caracter de nova linha
//se a currentLine está em branco, voce tem dois caracters de nova linha em sequencia.
//esse é o fim da requisicao HTTP, entao envia a resposta:
if (currentLine.length() == 0) {
//O cabecalho HTTP sempre incia com o codigo resposta (e.g. HTTP/1.1 200 OK)
//E um content-type entao o cliente sabe o que está vindo, entao a linha em branco:
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

//Liga e desliga as portas GPIO
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




//Exibe a pagina web HTML
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<meta charset=\"UTF-8\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
client.println("<title> CromoIT </title>");
//Estilo da pagina (CSS) 
client.println("<style>@import url('https://fonts.googleapis.com/css2?family=Lato&family=Nunito:wght@400;700&display=swap');");
client.println(" *{margin: 0; padding: 0;}html{ scroll-behavior: smooth;}");
client.println("body { font-family: 'Lato', sans-serif; max-width: 1220px; margin: auto; background-color: #F8F8FF; color: #101010; font-size: 16px; line-height: 24px;}");
client.println("header {display: flex; justify-content: space-between; align-items: center; padding: 25px 0;}");
client.println("header .logo{ display: flex; align-items: center;}");
client.println(".logo img{ width: 100px;}");
client.println(".logo img { width: 100px;}.logo h1 .C {color: #0049AF;}.logo h1 .I {color: #FF9737;}.logo h1 .T {color: #FF0E0E;}");
client.println("h1{ font-family: 'Nunito', sans-serif;}header nav {display: flex; align-items: center;}");
client.println("header nav ul{ display: flex; gap: 15px; flex-wrap: wrap; list-style: none;}nav ul li a { display: flex; flex-shrink: 1; flex-grow: 1; box-shadow: 0px 2px 2px #dcdcdc;}");
client.println("nav a{text-decoration: none; padding: 4px 8px; border: 2px #BFBBBB solid; border-radius: 3px; font-family: 'Nunito', sans-serif; font-weight: 700; color: #101010;}");
client.println("main{ margin: 25px 0;}section{ margin-bottom: 15px;}section h2 {margin-bottom: 1px;font-family: 'Nunito', sans-serif;}");
client.println("#testar{margin-bottom: 15px} #botoes{display: grid; grid-template: 1fr 1fr 1fr 1fr 1fr / 1fr 1fr;} #botoes div{margin-bottom: 5px;}");

//botao vermelho
client.println(".buttonRed { background-color: #ff0000; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao verde
client.println(".buttonGreen { background-color: #008000; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao azul
client.println(".buttonBlue { background-color: #0055ff; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao laranja
client.println(".buttonOrange { background-color: #ff8c00; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao magenta
client.println(".buttonMagenta { background-color: #a020f0; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao amarelo
client.println(".buttonYellow { background-color: #feeb36; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao branco
client.println(".buttonWhite { background-color: #ffffff; border: 2px #BFBBBB solid; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao turquesa
client.println(".buttonTurquesa { background-color: #00cece; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

//botao enquanto esta desligado
client.println(".buttonOff { background-color: #77878A; border: none; color: white; padding: 16px 40px; border-radius: 4px;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; box-shadow: 0px 2px 2px #dcdcdc}");

client.println("@media screen and (max-width: 1220px){body{margin:0 50px; max-width: 720px; font-size: 0.9em;}}");
client.println("@media screen and (max-width: 770px) {header{flex-direction: column; align-items: center; gap: 10px;} #botoes {display: flex; flex-direction: column; align-items: center; justify-content: center;} #botoes div {display: flex; flex-direction: column; align-items: center; justify-content: center;}}");
client.println("@media screen and (max-width: 570px){header nav ul{ flex-direction: column; align-items: center;}}</style></head>");

// Web Page Heading (cabecalho da pagina)
client.println("<body><header><div class=\"logo\"><img src=\"https://raw.githubusercontent.com/LucasFrancoBN/CromoIT/8581d268c88dc32a13cef0227bd83f17710ca77d/Images/CromoIT.svg\"> <h1><span class=\"C\">C</span>romo<span class=\"I\">I</span><span class=\"T\">T</span></h1></div><nav><ul><li><a href=\"#proposito\">""PROPÓSITO</a></li><li><a href=\"#testar\">""VAMOS TESTAR? :D</a></li><li><a href=\"#sobre\">""SOBRE NÓS</a></li><li><a href=\"#integrantes\">""INTEGRANTES</a></li></ul></nav></header>");

//MAIN (conteudo principal)
client.println("<main><section><h2 id=\"proposito\">"" Nosso Propósito</h2><p>Este projeto propõe soluções para melhoria da qualidade do sono. Tendo em vista que, nos dias atuais, temos cada vez mais dificuldade em ter uma boa noite de sono, o que pode alterar nosso humor e prejudicar nossa produtividade. Apresentamos o CromoIT, ferramenta que vai deixar seu ambiente aconchegante e prepará-lo para a rotina noturna.</p></section>");

client.println("<h2 id=\"testar\">""Vamos testar? :D</h2><section id=\"botoes\">"); 
//Exibe o estado atual dos botoes (liga/desliga) o botao vermelho
client.println("<div><p>O Led vermelho está " + vermelho_estado + "</p>");
//Se o vermelho_estado está off, exibe o botao off 
if (vermelho_estado=="off") {
client.println("<p><a href=\"/2/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/2/off\"><button class=\"button buttonRed\">ON</button></a></p></div>");
} 

//Exibe o estado atual dos botoes (liga/desliga) o botao verde
client.println("<div><p>O Led verde está " + verde_estado + "</p>");
//Se o verde_estado está off, exibe o botao off 
if (verde_estado =="off") {
client.println("<p><a href=\"/4/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/4/off\"><button class=\"button buttonGreen\">ON</button></a></p></div>");
}

//Exibe o estado atual dos botoes (liga/desliga) o botao azul
client.println("<div><p>O Led azul está " + azul_estado + "</p>");
//Se o azul_estado está off, exibe o botao off
if (azul_estado =="off") {
client.println("<p><a href=\"/5/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/5/off\"><button class=\"button buttonBlue\">ON</button></a></p></div>");
}

//Exibe o estado atual dos botoes (liga/desliga) o botao laranja
client.println("<div><p>O Led laranja está " + laranja_estado + "</p>");
//Se o laranja_estado está off, exibe o botao off 
if (laranja_estado =="off") {
client.println("<p><a href=\"/6/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/6/off\"><button class=\"button buttonOrange\">ON</button></a></p></div>");
}

//Exibe o estado atual dos botoes (liga/desliga) o botao magenta
client.println("<div><p>O Led Magenta está " + magenta_estado + "</p>");
//Se o magenta_estado está off, exibe o botao off  
if (magenta_estado =="off") {
client.println("<p><a href=\"/7/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/7/off\"><button class=\"button buttonMagenta\">ON</button></a></p></div>");
}

//Exibe o estado atual dos botoes (liga/desliga) o botao amarelo
client.println("<div><p>O Led Amarelo está " + amarelo_estado + "</p>");
//Se o amarelo_estado está off, exibe o botao off 
if (amarelo_estado =="off") {
client.println("<p><a href=\"/8/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/8/off\"><button class=\"button buttonYellow\">ON</button></a></p></div>");
}

//Exibe o estado atual dos botoes (liga/desliga) o botao branco
client.println("<div><p>O Led Branco está " + branco_estado + "</p>");
//Se o branco_estado está off, exibe o botao off 
if (branco_estado =="off") {
client.println("<p><a href=\"/9/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/9/off\"><button class=\"button buttonWhite\">ON</button></a></p></div>");
}

//Exibe o estado atual dos botoes (liga/desliga) o botao turquesa
client.println("<div><p>O Led Turquesa está " + turquesa_estado + "</p>");
//Se o turquesa_estado está off, exibe o botao off 
if (turquesa_estado =="off") {
client.println("<p><a href=\"/1/on\"><button class=\"button buttonOff\">OFF</button></a></p></div>");
} else {
client.println("<p><a href=\"/1/off\"><button class=\"button buttonTurquesa\">ON</button></a></p></div>");
}

//COLOR PICKER
client.println("<div><p>Esolha a cor que você deseja :D</p>");
client.println("<a href=\"ColorPick\" id=\"change_color\"><button class=\"button buttonOff\">Change Color</button></a> ");

//Input com o id "rgb" da classe "jscolor"
client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\">");

//Pega o elemento html que está com o id "rgb" e imprime a cor selecionada dentro do input
client.println("<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");

//Pega a tag "<a>" e altera o atributo ".href" com os valores selecionados
client.println("document.getElementById(\"change_color\").href=\"ColorPick?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}</script>");
client.println("</div>");

//condicao para conferir a URL (header)
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
  
  //define a posicao
  pos1 = header.indexOf('r');
  pos2 = header.indexOf('g');
  pos3 = header.indexOf('b');
  pos4 = header.indexOf('&');
  
  //Encontra a posicao da intensidade do led
  vermelhoCP = header.substring(pos1+7, pos2);
  verdeCP = header.substring(pos2+1, pos3);
  azulCP = header.substring(pos3+1, pos4);

  //Liga os Leds de acordo com a intensidade selecionada
  analogWrite(vermelhoLED, vermelhoCP.toInt());
  analogWrite(verdeLED, verdeCP.toInt());
  analogWrite(azulLED, azulCP.toInt());
  
  //Imprime os valores da intensidade no monitor serial
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
client.println("<section><h2 id=\"sobre\">""Sobre nós</h2><p>Somos estudantes de Análise e Desenvolvimento de Sistemas do Centro Universitário FECAP. No total, somos 5 integrantes que juntaram as ideias e realizaram este projeto para a disciplina de Projeto Interdisciplinar, e o resultado é esse que vocês estão vendo :D</p></section>");

//INTEGRANTES
client.println("<section><h2 id=\"integrantes\">""Integrantes</h2><p>Lucas Franco - 22023179</p><p>Glauber Vinicius - 22023260</p><p> Esdras Mendes - 22023038</p><p>Beatriz Candido - 22023095</p><p>Julya Joplin - 22023461</p></section>");

//FIM DO HTML
client.println("</main></body></html>");

//A resposta HTTP termina com outra linha em branco
client.println();
//Quebra do laco while 
break;
} else { //Se tem uma nova linha, entao limpa a currentLine
currentLine = "";
}
} else if (c != '\r') { //Se voce tem qualquer coisa além de um caracter de retorno
currentLine += c; //Adiciona "c" no fim da currentLine
}
}
}
//Limpa a variavel header
header = "";
//Fecha a conexao
client.stop();
//Imprime cliente desconectado
Serial.println("Client disconnected.");
Serial.println("");
}
}

//funcao para o led RGB
void RGB_cor (int vermelho_valor, int verde_valor, int azul_valor) {
  analogWrite (vermelhoLED, vermelho_valor);
  analogWrite (verdeLED, verde_valor);
  analogWrite (azulLED, azul_valor);
}

//funcao de estado desligado
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
