#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <DHT_ESP8266.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define PIN_PIXELS_1 13 //D7
#define PIN_PIXELS_2 15 //D8
#define PIN_SENSOR_HUMEDAD 12 //D6
#define RADIO_TERRESTRE 6371

//Este codigo fue extraido de un ejemplo de la biblioteca
//LiquidCrystal_I2C, sirve para poder imprimir caracteres especiales
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

struct
{
  double latitud;
  double longitud;
} typedef Punto;

Adafruit_NeoPixel  *anillo_1;
Adafruit_NeoPixel  *anillo_2;
LiquidCrystal_I2C  *pantalla;
PietteTech_DHT     *humedad;
NTPClient          *cliente_tiempo;

WiFiClient         cliente;
WiFiUDP            udp;


int             color_temperatura_H_inicio = 281;
float           color_temperatura_S_inicio = 1;
float           color_temperatura_V_inicio = 1;
int             color_temperatura_H_fin = 292;
float           color_temperatura_S_fin = 1;
float           color_temperatura_V_fin = 1;
float           umbral_temperatura_maximo=50;
float           umbral_temperatura_minimo=50;

int             color_humedad_H_inicio = 57;
float           color_humedad_S_inicio = 1;
float           color_humedad_V_inicio = 1;
int             color_humedad_H_fin = 3;
float           color_humedad_S_fin = 1;
float           color_humedad_V_fin = 1;

int             color_hora_H_inicio = 288;
float           color_hora_S_inicio = 1;
float           color_hora_V_inicio = 1;
int             color_hora_H_fin = 17;
float           color_hora_S_fin = 1;
float           color_hora_V_fin = 1;

int             color_proximidad_H_inicio = 288;
float           color_proximidad_S_inicio = 1;
float           color_proximidad_V_inicio = 1;
int             color_proximidad_H_fin = 17;
float           color_proximidad_S_fin = 1;
float           color_proximidad_V_fin = 1;
int             umbral_proximidad_maximo = 10;


int             color_distancia_H_inicio = 120;
float           color_distancia_S_inicio = 100;
float           color_distancia_V_inicio = 100;
int             color_distancia_H_fin = 357;
float           color_distancia_S_fin = 100;
float           color_distancia_V_fin = 100;


uint8_t         tilde[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t         cruz [8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};

int             opcion   = 0;

Punto           punto_inicial;
Punto           punto_final;
bool            primera_muestra = true;

int             valor_proximidad = 0;


float lectura_humedad_g;
float lectura_temperatura_g;
float lectura_temperatura_anterior;



//La biblioteca DHT_ESP8266 (PietteTech_DHT) Necesita esta funcion para el correcto funcionamiento.
void dht_wrapper()
{
  humedad->isrCallback();
}


//SETUP
void setup()
{
  Serial.begin(115200);
  humedad  = new PietteTech_DHT(PIN_SENSOR_HUMEDAD, DHT22, dht_wrapper);
  anillo_1 = new Adafruit_NeoPixel(16, PIN_PIXELS_1, NEO_GRB + NEO_KHZ800);
  anillo_2 = new Adafruit_NeoPixel(16, PIN_PIXELS_2, NEO_GRB + NEO_KHZ800);
  pantalla = new LiquidCrystal_I2C(0x3f, 20, 4);
  anillo_1->begin();
  anillo_2->begin();
  anillo_1->show();
  anillo_2->show();
  Wire.begin(2, 0); //GPIO2 y GPIO0
  pantalla->init();
  pantalla->setBacklight(HIGH);
  pantalla->setCursor(0, 0);
  pantalla->print("[CONECTANDO] SOa-IoT");
  Conectar("", "", "www.dweet.io");
  cliente_tiempo = new NTPClient(udp, "europe.pool.ntp.org", -10800, 60000);
  cliente_tiempo->update();
  cliente_tiempo->begin();
  humedad->acquire();
  pantalla->setCursor(10, 1);
  pantalla->print("|");
  pantalla->setCursor(10, 2);
  pantalla->print("|");
  pantalla->setCursor(10, 3);
  pantalla->print("|");
  pantalla->createChar(0, tilde);
  pantalla->createChar(1, cruz);
  pantalla->setCursor(0, 1);
  pantalla->print("Tem: ");
  pantalla->print("?");
  pantalla->setCursor(0, 2);
  pantalla->print("Hum: ");
  pantalla->print("?");
  lectura_temperatura_anterior = analogRead(A0) * 0.32226562;
  delay(2500);
}


//MAIN
void loop()
{
  int   lecturas = 0;
  float lectura_temperatura_actual;
  
  pantalla->setCursor(13, 1);
  pantalla->print("Modo");

  while (true)
  {
    MostrarHora();
    
    pantalla->setCursor(0, 3);
    pantalla->print("Conexion:");
    pantalla->printByte(0);

    lectura_humedad_g = humedad->getHumidity();
    lectura_temperatura_actual = analogRead(A0) * 0.32226562;
    lectura_temperatura_g = (lectura_temperatura_actual + lectura_temperatura_anterior) / 2;

    if (lectura_temperatura_g < 0)
      lectura_temperatura_g = 0;

    lectura_temperatura_anterior = lectura_temperatura_g;

    pantalla->setCursor(0, 1);
    pantalla->print("Tem: ");
    pantalla->print(lectura_temperatura_g);
    pantalla->setCursor(0, 2);
    pantalla->print("Hum: ");
    pantalla->print(lectura_humedad_g);

    EnvioDatos(lectura_temperatura_g, lectura_humedad_g, "www.dweet.io", "/get/latest/dweet/for/soa-ici-sensor");
    ConsultarConfiguracion("www.dweet.io", "/get/latest/dweet/for/soa-ici-colors");
    RecibirDatos("www.dweet.io", "/get/latest/dweet/for/soa-ici-android");


    switch (opcion)
    {
      //Humedad y Temperatura
      case 0:
        punto_inicial.latitud = punto_inicial.longitud = 0;
        punto_final.latitud = punto_final.longitud = 0;
        primera_muestra=true;
        LimpiarCarateres(3, 11, 19);
        pantalla->setCursor(13, 2);
        pantalla->print("H & T");
        HumedadTemperatura();
        break;
      //Hora
      case 1:
        punto_inicial.latitud = punto_inicial.longitud = 0;
        punto_final.latitud = punto_final.longitud = 0;
        primera_muestra=true;
        LimpiarCarateres(3, 11, 19);
        pantalla->setCursor(13, 2);
        pantalla->print("Hora ");
        Hora();
        break;
      //GPS
      case 2:
        LimpiarCarateres(3, 11, 19);
        pantalla->setCursor(13, 2);
        pantalla->print("GPS  ");
        Distancia();
        break;
      //Proximidad
      case 3:
        punto_inicial.latitud = punto_inicial.longitud = 0;
        punto_final.latitud = punto_final.longitud = 0;
        primera_muestra=true;
        LimpiarCarateres(3, 11, 19);
        pantalla->setCursor(13, 2);
        pantalla->print("Prox ");

        for (int i = 0; i < valor_proximidad; i++)
        {
          pantalla->setCursor(12 + i, 3);
          pantalla->print("|");
        }

        Proximidad();

        break;
    }
    delay(2500);
  }
}


void MostrarHora()
{
    LimpiarLinea(0);
    pantalla->setCursor(8, 0);
    if (cliente_tiempo->getHours() < 10)
    {
      pantalla->print("0");
      pantalla->setCursor(9, 0);
    }
    pantalla->print(cliente_tiempo->getHours());
    pantalla->setCursor(10, 0);
    pantalla->print(": ");
    pantalla->setCursor(11, 0);
    if (cliente_tiempo->getMinutes() < 10)
    {
      pantalla->print("0");
      pantalla->setCursor(12, 0);
    }
    pantalla->print(cliente_tiempo->getMinutes());
}

void HumedadTemperatura()
{
  uint8_t rojo;
  uint8_t verde;
  uint8_t azul;

  float lectura_humedad = lectura_humedad_g / 100;
  float lectura_temperatura = (((lectura_temperatura_g) * 100) / 50) / 100;

  HSVaRGB(color_humedad_H_inicio + (int) (lectura_humedad * (color_humedad_H_fin - color_humedad_H_inicio)),
          color_humedad_S_inicio + (int) (lectura_humedad * (color_humedad_S_fin - color_humedad_S_inicio)),
          color_humedad_V_inicio + (int) (lectura_humedad * (color_humedad_V_fin - color_humedad_V_inicio)),
          &rojo,
          &verde,
          &azul
         );

  for (int i = 0; i < 16; i++)
    anillo_1->setPixelColor(i, rojo, verde, azul);

  HSVaRGB(color_temperatura_H_inicio + (int) (lectura_temperatura * (color_temperatura_H_fin - color_temperatura_H_inicio)),
          color_temperatura_S_inicio + (int) (lectura_temperatura * (color_temperatura_S_fin - color_temperatura_S_inicio)),
          color_temperatura_V_inicio + (int) (lectura_temperatura * (color_temperatura_V_fin - color_temperatura_V_inicio)),
          &rojo,
          &verde,
          &azul
         );

  for (int i = 0; i < 16; i++)
    anillo_2->setPixelColor(i, rojo, verde, azul);

  anillo_1->show();
  anillo_2->show();

}

void Hora()
{
  float lectura_hora = 0;
  uint8_t rojo;
  uint8_t verde;
  uint8_t azul;


  if (cliente_tiempo->getHours() >= 0 && cliente_tiempo->getHours() < 13)
  {
    if (cliente_tiempo->getHours() == 0)
      lectura_hora = ((((cliente_tiempo->getMinutes())) * 100.0) / 780) / 100;
    else
      lectura_hora = ((((cliente_tiempo->getHours() * 60 + cliente_tiempo->getMinutes())) * 100.0) / 780) / 100;


    HSVaRGB(color_hora_H_inicio + (int) (lectura_hora * (color_hora_H_fin - color_hora_H_inicio)),
            color_hora_S_inicio + (int) (lectura_hora * (color_hora_S_fin - color_hora_S_inicio)),
            color_hora_V_inicio + (int) (lectura_hora * (color_hora_V_fin - color_hora_V_inicio)),
            &rojo,
            &verde,
            &azul
           );


    for (int i = 0; i < 16; i++)
    {
      anillo_1->setPixelColor(i, rojo, verde, azul);
      anillo_2->setPixelColor(i, rojo, verde, azul);
      anillo_1->show();
      anillo_2->show();
      delay(5);
    }
  }
  else
  {

    lectura_hora = (((1440 - ((cliente_tiempo->getHours() * 60 + cliente_tiempo->getMinutes()))) * 100.0) / 660) / 100;


    HSVaRGB(color_hora_H_inicio + (int) (lectura_hora * (color_hora_H_fin - color_hora_H_inicio)),
            color_hora_S_inicio + (int) (lectura_hora * (color_hora_S_fin - color_hora_S_inicio)),
            color_hora_V_inicio + (int) (lectura_hora * (color_hora_V_fin - color_hora_V_inicio)),
            &rojo,
            &verde,
            &azul
           );

    for (int i = 0; i < 16; i++)
    {
      anillo_1->setPixelColor(i, rojo, verde, azul);
      anillo_2->setPixelColor(i, rojo, verde, azul);
      anillo_1->show();
      anillo_2->show();
      delay(5);
    }
  }
}


void Distancia()
{
  //Formula de Haversine
  double h = sqrt(pow(sin((punto_inicial.latitud - punto_final.latitud) / 2), 2) + cos(punto_inicial.latitud) * cos(punto_final.latitud) * pow(sin((punto_inicial.longitud - punto_final.longitud) / 2), 2));
  double ddistancia;
  float  distancia;
  uint8_t rojo;
  uint8_t verde;
  uint8_t azul;

  if (h >= 1)
    h = 0.99;

  ddistancia = 6371000 * 2 * asin(h);

  pantalla->setCursor(12, 3);
  pantalla->print(ddistancia);

  distancia = (float) ((ddistancia * 100) / 20);

  HSVaRGB(   color_distancia_H_inicio + (int) (distancia * (color_distancia_H_fin - color_distancia_H_inicio)),
             color_distancia_S_inicio + (int) (distancia * (color_distancia_S_fin - color_distancia_S_inicio)),
             color_distancia_V_inicio + (int) (distancia * (color_distancia_V_fin - color_distancia_V_inicio)),
             &rojo,
             &verde,
             &azul
         );

  for (int i = 0; i < 16; i++)
  {
    anillo_1->setPixelColor(i, rojo, verde, azul);
    anillo_2->setPixelColor(i, rojo, verde, azul);
    anillo_1->show();
    anillo_2->show();
    delay(5);
  }

}

void Proximidad()
{
  uint8_t rojo;
  uint8_t verde;
  uint8_t azul;

  float lectura_proximidad = ((valor_proximidad * 100) / (float) umbral_proximidad_maximo) / 100;

  Serial.println(lectura_proximidad);

  HSVaRGB(   color_proximidad_H_inicio + (int) (lectura_proximidad * (color_proximidad_H_fin - color_proximidad_H_inicio)),
             color_proximidad_S_inicio + (int) (lectura_proximidad * (color_proximidad_S_fin - color_proximidad_S_inicio)),
             color_proximidad_V_inicio + (int) (lectura_proximidad * (color_proximidad_V_fin - color_proximidad_V_inicio)),
             &rojo,
             &verde,
             &azul
         );

  for (int i = 0; i < 16; i++)
  {
    anillo_1->setPixelColor(i, rojo, verde, azul);
    anillo_2->setPixelColor(i, rojo, verde, azul);
    anillo_1->show();
    anillo_2->show();
    delay(5);
  }
}

//COLORES
void RGBaHSV(uint8_t rojo, uint8_t verde, uint8_t azul, float *H, float *S, float *V)
{
  float   minimo;
  uint8_t maximo;

  //V es el maximo de RGB
  if (rojo == verde && rojo == azul)
  {
    maximo = rojo;
    minimo = rojo;
  }
  else
  {
    if (rojo > verde && rojo > azul)
      maximo = rojo;
    else
    {
      if (verde > rojo && verde > azul)
        maximo = verde;
      else
      {
        if (azul > rojo && azul > verde)
          maximo = azul;
        else
        {
          if ( (rojo > verde  && rojo == azul) || (rojo == verde && rojo > azul))
            maximo = rojo;
          else
          {
            if ( (verde > rojo && verde == azul) || (verde == rojo && verde > azul))
              maximo = verde;
            else
            {
              if ( (azul > rojo && azul == verde) || (azul == rojo && azul > verde))
                maximo = azul;
            }
          }
        }
      }
    }
  }

  //Minimo Valor
  if (rojo < verde && rojo < azul)
    minimo = rojo;
  else
  {
    if (verde < rojo && verde < azul)
      minimo = verde;
    else
    {
      if (azul < rojo && azul < verde)
        minimo = azul;
      else
      {
        if ( (rojo < verde  && rojo == azul) || (rojo == verde && rojo < azul))
          minimo = rojo;
        else
        {
          if ( (verde < rojo && verde == azul) || (verde == rojo && verde < azul))
            minimo = verde;
          else
          {
            if ( (azul < rojo && azul == verde) || (azul == rojo && azul < verde))
              minimo = azul;
          }
        }
      }
    }
  }

  //Valor
  (*V) = maximo / 255.0;
  minimo = minimo / 255.0;

  //Saturacion
  if ((*V) == 0)
    (*S) = 0;
  else
  {
    (*S) = (1 - (minimo / (*V)));
  }

  //Matiz

  if ((*V) == minimo)
    (*H) = 0;
  else
  {
    if (maximo == rojo && verde >= azul)
      (*H) = 60 * ((verde / 255.0 - azul / 255.0) / (*V - minimo));
    else
    {
      if (maximo == rojo && verde < azul)
        (*H) = 60 * ((verde / 255.0 - azul / 255.0) / (*V - minimo)) + 360;
      else
      {
        if (maximo == verde)
          (*H) = 60 * ((azul / 255.0 - rojo / 255.0) / (*V - minimo)) + 120;
        else
        {
          if (maximo == azul)
            (*H) = 60 * (((rojo / 255.0 - verde / 255.0)) / (*V - minimo)) + 240;
        }
      }
    }
  }

}

void HSVaRGB(float H, float S, float V, uint8_t *rojo, uint8_t *verde, uint8_t *azul)
{
  int   Ti = (int)(H / 60) % 6;
  float f = (H / 60) - Ti;
  float l = V * (1 - S);
  float m = V * (1 - f * S);
  float n = V * (1 - (1 - f) * S);

  switch (Ti)
  {
    case 0:
      *rojo = (int) (V * 255);
      *verde = (int) (n * 255);
      *azul = (int) (l * 255);
      break;
    case 1:
      *rojo = (int) (m * 255);
      *verde = (int) (V * 255);
      *azul = (int) (l * 255);
      break;
    case 2:
      *rojo = (int) (l * 255);
      *verde = (int) (V * 255);
      *azul = (int) (n * 255);
      break;
    case 3:
      *rojo = (int) (l * 255);
      *verde = (int) (m * 255);
      *azul = (int) (V * 255);
      break;
    case 4:
      *rojo = (int) (n * 255);
      *verde = (int) (l * 255);
      *azul = (int) (V * 255);
      break;
    case 5:
      *rojo = (int) (V * 255);
      *verde = (int) (l * 255);
      *azul = (int) (m * 255);
      break;
  }

}

//PANTALLA
void LimpiarCarateres(int linea, int posicion_inicial, int posicion_final)
{
  for (int i = posicion_inicial; i <= posicion_final; i++)
  {
    pantalla->setCursor(i, linea);
    pantalla->print(" ");
  }
}

void LimpiarLinea(int linea)
{
  for (int i = 0; i < 20; i++)
  {
    pantalla->setCursor(i, linea);
    pantalla->print(" ");
  }
}

void LimpiarPantalla()
{
  for (int i = 0; i < 4; i++)
    LimpiarLinea(i);
}

//WEB
void Conectar(char *red, char *password, char *host)
{
  bool  conexion_exitosa = false;
  float iteraciones = 0;
  int   led_restante = 8;
  int   led_recorrido = 0;
  bool  direccion = false;
  int   contador_derecha = 0;
  int   contador_izquierda = 0;


  for (int leds_iniciales = 0; leds_iniciales < 8; leds_iniciales++)
  {
    anillo_1->setPixelColor(leds_iniciales, 255, 0, 0);
    anillo_2->setPixelColor(15 - leds_iniciales, 255, 0, 0);
  }

  WiFi.begin(red, password);

  while (conexion_exitosa == false)
  {

    if (WiFi.status() == WL_CONNECTED && cliente.connect(host, 80) == true)
    {
      conexion_exitosa = true;
      cliente.println("Connection: close\r\n\r\n");
      break;
    }

    //Se muestra una animacion en los led, ya que no se conecto a internet
    for (int i = 0; i < 100; i++)
    {
      LimpiarCarateres(2, 6, 13);
      pantalla->setCursor(5, 2);
      pantalla->print("[");
      pantalla->setCursor(14, 2);
      pantalla->print("]");

      if (direccion == false)
      {
        pantalla->setCursor(6 + (i % 6), 2);
        pantalla->print("*");
        contador_derecha++;
      }
      else
      {
        pantalla->setCursor(13 - (i % 6), 2);
        pantalla->print("*");
        contador_izquierda++;
      }

      if (contador_derecha == 6)
      {
        contador_derecha = 0;
        direccion = true;
      }

      if (contador_izquierda == 6)
      {
        contador_izquierda = 0;
        direccion = false;
      }

      anillo_1->setPixelColor(16 - led_restante, 255, 0, 0);
      anillo_1->setPixelColor(led_recorrido, 0, 0, 0);
      anillo_2->setPixelColor(16 - led_recorrido, 0, 0, 0);
      anillo_2->setPixelColor(led_restante, 255, 0, 0);

      led_restante--;
      led_recorrido++;

      if (led_restante == 0)
        led_restante = 16;

      if (led_recorrido == 16)
        led_recorrido = 0;

      anillo_1->show();
      anillo_2->show();

      delay(150);
    }
  }

  pantalla->setCursor(0, 0);
  pantalla->print("[CONECTADO] SOa-IoT");

  //Se muestra una animacion verde cuando se pudo conectar a internet
  for (int j = 0; j < 255; j++)
  {
    for (int i = 0; i < 16; i++)
    {
      anillo_1->setPixelColor(i, 0, j, 0);
      anillo_2->setPixelColor(i, 0, j, 0);
      anillo_1->show();
      anillo_2->show();
    }

    delay(10);
  }

  LimpiarPantalla();
}


void ConsultarConfiguracion(char *host, char *destino)
{
  String datos;
  String humedad;
  String colores;
  String temperatura;
  String hora;
  String proximidad;

  cliente.stop();
  cliente.connect(host, 80);
  cliente.print(String("GET ") + destino + "\r\n Host: " + host + "\r\n Connection: close\r\n\r\n");
  delay(20);

  while (cliente.available())
    datos += cliente.readStringUntil('\r');


  if (datos.length() > 10 && datos.indexOf("\"this\":\"failed\"") == -1)
  {
    datos = datos.substring(datos.indexOf("<?xml"), datos.indexOf("</colorConfiguration>"));
    datos.concat(" </colorConfiguration>");
    datos.replace("\\n", " ");
    datos.replace(",", ".");

    //Temperatura (Bajo)
    temperatura = datos.substring(datos.indexOf("<TemperatureColor>") + 18, datos.indexOf("</TemperatureColor>"));
    colores = temperatura.substring(temperatura.indexOf("<LowColor>") + 10, temperatura.indexOf("</LowColor>"));
    color_temperatura_H_inicio = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_temperatura_S_inicio = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_temperatura_V_inicio = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Temperatura (Alto)
    colores = temperatura.substring(temperatura.indexOf("<HighColor>") + 11, temperatura.indexOf("</HighColor>"));
    color_temperatura_H_fin = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_temperatura_S_fin = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_temperatura_V_fin = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Humedad (Bajo)
    humedad = datos.substring(datos.indexOf("<HumidityColor>") + 15, datos.indexOf("</HumidityColor>"));
    colores = humedad.substring(humedad.indexOf("<LowColor>") + 10, humedad.indexOf("</LowColor>"));
    color_humedad_H_inicio = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_humedad_S_inicio = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_humedad_V_inicio = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Humedad (Alto)
    humedad = datos.substring(datos.indexOf("<HumidityColor>") + 15, datos.indexOf("</HumidityColor>"));
    colores = humedad.substring(humedad.indexOf("<HighColor>") + 11, humedad.indexOf("</HighColor>"));
    color_humedad_H_fin = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_humedad_S_fin = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_humedad_V_fin = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Hora (Bajo)
    hora = datos.substring(datos.indexOf("<HourColor>") + 11, datos.indexOf("</HourColor>"));
    colores = hora.substring(hora.indexOf("<LowColor>") + 10, hora.indexOf("</LowColor>"));
    color_hora_H_inicio = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_hora_S_inicio = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_hora_V_inicio = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Hora (Alto)
    hora = datos.substring(datos.indexOf("<HourColor>") + 11, datos.indexOf("</HourColor>"));
    colores = hora.substring(hora.indexOf("<HighColor>") + 11, hora.indexOf("</HighColor>"));
    color_hora_H_fin = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_hora_S_fin = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_hora_V_fin = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();


    //Proximidad (Bajo)
    proximidad = datos.substring(datos.indexOf("<ProximityColor>") + 16, datos.indexOf("</ProximityColor>"));
    colores = proximidad.substring(proximidad.indexOf("<LowColor>") + 10, proximidad.indexOf("</LowColor>"));
    color_proximidad_H_inicio = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_proximidad_S_inicio = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_proximidad_V_inicio = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Proximidad (Alto)
    proximidad = datos.substring(datos.indexOf("<ProximityColor>") + 16, datos.indexOf("</ProximityColor>"));
    colores = proximidad.substring(proximidad.indexOf("<HighColor>") + 11, proximidad.indexOf("</HighColor>"));
    color_proximidad_H_fin = (colores.substring(colores.indexOf("<H>") + 3, colores.indexOf("</H>"))).toInt();
    color_proximidad_S_fin = (colores.substring(colores.indexOf("<S>") + 3, colores.indexOf("</S>"))).toFloat();
    color_proximidad_V_fin = (colores.substring(colores.indexOf("<V>") + 3, colores.indexOf("</V>"))).toFloat();

    //Proximidad (Umbral)
    proximidad = datos.substring(datos.indexOf("<ProximityColor>") + 16, datos.indexOf("</ProximityColor>"));
    umbral_proximidad_maximo = proximidad.substring(proximidad.indexOf("<Threshold>") + 11, proximidad.indexOf("</Threshold>")).toInt();

  }

}


void EnvioDatos(float temperatura, float humedad, char *host, char *destino)
{
  String datos;
  cliente.stop();
  cliente.connect(host, 80);
  datos = "POST /dweet/for/soa-ici-sensor?type=temperature&value=";
  datos.concat(temperatura);
  datos.concat("|");
  datos.concat(humedad);
  cliente.println(datos);
  cliente.println("Host: www.dweet.io");
  cliente.println("Connection: close\r\n\r\n");
}


void RecibirDatos(char *host, char *destino)
{
  //Cada vez que se van a enviar datos se debe leer el dweet para saber si hay datos de cambio de configuracion
  String datos;

  cliente.stop();
  cliente.connect(host, 80);
  cliente.print(String("GET ") + destino + "\r\n Host: " + host + "\r\n Connection: close\r\n\r\n");
  Serial.println(String("GET ") + destino + "\r\n Host: " + host + "\r\n Connection: close\r\n\r\n");
  delay(20);

  while (cliente.available())
    datos += cliente.readStringUntil('\r');

  datos = datos.substring(datos.indexOf("{\"type"), datos.indexOf("},")); //""\"}}]}"

  if (datos.indexOf("HT") != -1)
    opcion = 0;
  else
  {
    if (datos.indexOf("Hour") != -1)
      opcion = 1;
    else
    {
      if (datos.indexOf("GPS") != -1)
      {
        String valores = datos.substring(datos.indexOf("value\":") + 8, datos.indexOf("}"));

        if (primera_muestra == true)
        {
          punto_inicial.latitud = (valores.substring(0, valores.indexOf('|')).toFloat()) * (PI / 180);
          punto_inicial.longitud = (valores.substring(valores.indexOf('|') + 1, valores.indexOf('"')).toFloat()) * (PI / 180);
          primera_muestra = false;
        }

        punto_final.latitud = (valores.substring(0, valores.indexOf('|')).toFloat()) * (PI / 180);
        punto_final.longitud = (valores.substring(valores.indexOf('|') + 1, valores.indexOf('"')).toFloat()) * (PI / 180);
        opcion = 2;
      }
      else
      {
        if (datos.indexOf("Proximity") != -1)
        {
          valor_proximidad = (datos.substring(datos.indexOf("value\":") + 7, datos.indexOf("}"))).toInt();
          opcion = 3;
        }
      }
    }
  }

}



