/*#include <DHT.h>

#define DHTPIN 2      // Pin collegato al DHT11
#define DHTTYPE DHT11 // Tipo di sensore DHT
#define RELAY_PIN 10  // Pin collegato al relè

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);
    dht.begin();
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH); // Il LED rosso è acceso di default (relè NC)
}

void loop() {
    float temp = dht.readTemperature();  // Legge la temperatura in °C
    float hum = dht.readHumidity();      // Legge l'umidità in %

    if (isnan(temp) || isnan(hum)) {
        Serial.println("❌ Errore nella lettura del DHT11!");
        delay(2000); // Aspetta 2 secondi prima di riprovare
        return;
    }

    // Stampa i valori letti
    Serial.println("📊 Lettura Sensore DHT11:");
    Serial.print("🌡️ Temperatura: ");
    Serial.print(temp);
    Serial.println("°C");

    Serial.print("💧 Umidità: ");
    Serial.print(hum);
    Serial.println("%");

    // Controllo del relè in base alla temperatura
    if (temp >= 10 && temp <= 20) {
        digitalWrite(RELAY_PIN, HIGH); // Mantiene il LED rosso acceso (NC)
        Serial.println("🔴 LED ROSSO ACCESO");
    } 
    else if (temp >= 22 && temp <= 30) {
        digitalWrite(RELAY_PIN, LOW); // Accende il LED verde (NO)
        Serial.println("🟢 LED VERDE ACCESO");
    } 
    else {
        digitalWrite(RELAY_PIN, HIGH); // Spegne entrambi i LED
        Serial.println("⚪ Nessun LED acceso");
    }

    Serial.println("------------------------"); // Separatore per migliorare la lettura
    delay(2000); // Aspetta 2 secondi prima della prossima lettura
}*/

#include <DHT.h>

#define DHTPIN 2   
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int LED_VERDE = 4;
const int LED_ROSSO = 3;
float soglia_superiore = 30.0; 
float soglia_inferiore = 20.0; 

void setup() {
    Serial.begin(9600);
    dht.begin();
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_ROSSO, OUTPUT);
}

void loop() {
    float umidita = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(umidita) || isnan(temperatura)) {
        Serial.println("Errore nella lettura del sensore!");
        return;
    }

    String stato_led = "Spenti";

    if (temperatura >= soglia_superiore) {
        digitalWrite(LED_ROSSO, HIGH);
        digitalWrite(LED_VERDE, LOW);
        stato_led = "Rosso acceso";
    } else if (temperatura <= soglia_inferiore) {
        digitalWrite(LED_ROSSO, LOW);
        digitalWrite(LED_VERDE, LOW);
        stato_led = "Spenti";
    } else {
        digitalWrite(LED_ROSSO, LOW);
        digitalWrite(LED_VERDE, HIGH);
        stato_led = "Verde acceso";
    }

    // Invia dati a Python (temperatura, umidità, stato LED)
    Serial.print(temperatura);
    Serial.print(",");
    Serial.print(umidita);
    Serial.print(",");
    Serial.println(stato_led);

    // Controlla se ci sono nuovi dati dalla porta seriale
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n'); // Legge la stringa fino a fine riga
        int commaIndex = input.indexOf(',');
        if (commaIndex != -1) {
            soglia_superiore = input.substring(0, commaIndex).toFloat();
            soglia_inferiore = input.substring(commaIndex + 1).toFloat();
        }
    }

    delay(2000); // Campionamento ogni 2 secondi
}




