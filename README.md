**AquaTemp**
**Controle de temperatura de aquários e função para acender e apagar iluminação**

O projeto de trabalho do curso de pós graduação do IFSP - Campus Catanduva turma 2024, foi elaborado pelos alunos Ezequiel Pereira e Renato Gennari, possui o código de um sistema de monitoramento de temperatura e umidade usando um sensor DHT11 e um sensor de temperatura Dallas (OneWire). Ele envia dados via MQTT para um broker, além de permitir controle de um LED através de mensagens MQTT. Aqui está um resumo das principais funcionalidades:

Sensores: DHT11 (para medir temperatura e umidade), e um sensor Dallas para medir a temperatura da água.
Wi-Fi: Conecta a uma rede Wi-Fi definida pelas credenciais (SSID e senha).
MQTT: Utiliza o broker público HiveMQ para enviar leituras e receber comandos de controle do LED.
JSON: Serializa as leituras dos sensores em formato JSON e as publica em um tópico MQTT.

Montagem do sensor DHT11 na Placa ESP32 NODE32S

![image](https://github.com/user-attachments/assets/43e63528-9bb8-4724-85b9-f71daad6a7c0)

Montagem do sensor DS18B20 na Placa ESP32 NODE32S

![image](https://github.com/user-attachments/assets/12690609-3dec-481c-a778-7e676bccac86)

Montagem do led na Placa ESP32 NODE32S

![image](https://github.com/user-attachments/assets/ec1a92b1-ed3f-4662-a77e-4286a953af25)

Acesso ao DashBoard - NODE-RED, possui a verificação dos dados coletados e acionamento remoto de liga e desliga do led.

http://3.134.113.134:1880/ui



