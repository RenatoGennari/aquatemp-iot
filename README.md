O projeto de trabalho possui o código de um sistema de monitoramento de temperatura e umidade usando um sensor DHT11 e um sensor de temperatura Dallas (OneWire). Ele envia dados via MQTT para um broker, além de permitir controle de um LED através de mensagens MQTT. Aqui está um resumo das principais funcionalidades:

Sensores: DHT11 (para medir temperatura e umidade), e um sensor Dallas para medir a temperatura da água.
Wi-Fi: Conecta a uma rede Wi-Fi definida pelas credenciais (SSID e senha).
MQTT: Utiliza o broker público HiveMQ para enviar leituras e receber comandos de controle do LED.
JSON: Serializa as leituras dos sensores em formato JSON e as publica em um tópico MQTT.
