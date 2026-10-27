// Professor Rafael Oliveira - Tio Rafa 2025 www.tiorafa.tec.br
//código desenvolvido para o carrinho Formula RPS e atividades da Robótica Paula Souza

#include <Servo.h>
#include <PS2X_lib.h>  // Biblioteca para controle do joystick PS2

// Define os pinos dos motores4
int in1 = 3;
int in2 = 5;
int in3 = 6;
int in4 = 9;

// Define o pino do servo motor
const int servoPin = 2;

// Define a velocidade máxima (0 a 255)
int maxSpeed = 255;

// Cria um objeto Servo
Servo steeringServo;

// Posições do servo (ajuste conforme necessário)
const int centerPos = 90;   // Posição central
const int leftPos = 60;      // Máximo para esquerda
const int rightPos = 120;    // Máximo para direita

// Configuração do joystick PS2 sem fio
#define PS2_DAT        13 // Pino DATA LARANJA
#define PS2_CMD        12  // Pino COMMAND AMARELO
#define PS2_SEL        10  // Pino SELECT VERDE
#define PS2_CLK        11  // Pino CLOCK AZUL

#define pressures   false
#define rumble      false

PS2X ps2x; // Cria objeto do controle PS2

int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
    Serial.begin(57600);
    
    // Configura os pinos dos motores como saída
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    
    // Anexa o servo ao pino correspondente
    steeringServo.attach(servoPin);
    steeringServo.write(centerPos); // Centraliza o servo
    
    // Configuração do controle PS2 sem fio
    delay(300); // Delay para inicialização do receptor
    
    // Tenta configurar a comunicação com o controle
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    
    if(error == 0){
        Serial.println("Controle PS2 sem fio encontrado e configurado com sucesso");
    } else {
        Serial.print("Erro na configuração do controle PS2. Codigo: ");
        Serial.println(error);
    }
}

void loop() {
    if(error == 0) { // Só executa se o controle estiver conectado
        ps2x.read_gamepad(false, vibrate); // Lê o estado do controle
        
        // Controle da direção com o servo (usando direcional esquerdo)
        if(ps2x.Analog(PSS_LX) < 100) { // Analógico esquerdo para esquerda
            steeringServo.write(leftPos);
        } 
        else if(ps2x.Analog(PSS_LX) > 150) { // Analógico esquerdo para direita
            steeringServo.write(rightPos);
        } 
        else {
            steeringServo.write(centerPos);
        }

        // Controle de movimento - Frente (botão X)
        if(ps2x.Button(PSB_CROSS)) {
            analogWrite(in1, 0);
            analogWrite(in2, maxSpeed);
            analogWrite(in3, maxSpeed);
            analogWrite(in4, 0);
        }
        // Controle de movimento - Trás (botão Círculo)
        else if(ps2x.Button(PSB_CIRCLE)) {
            analogWrite(in1, maxSpeed);
            analogWrite(in2, 0);
            analogWrite(in3, 0);
            analogWrite(in4, maxSpeed);
        }
        // Parar os motores se nenhum botão de movimento estiver pressionado
        else {
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
        }
    } else {
        // Caso haja erro na conexão, para os motores
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        steeringServo.write(centerPos);
        
        // Tenta reconectar a cada 2 segundos
        delay(2000);
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    }

    delay(10);
}
