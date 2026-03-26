🌿 Datalogger Ambiental ATmega 328P

Sistema de monitoramento ambiental de baixo consumo baseado no microcontrolador ATmega 328P (compatível com Arduino Uno).

O dispositivo monitora temperatura, umidade e luminosidade em tempo real, fornecendo alertas visuais e sonoros quando as condições saem das faixas ideais.

🚀 Funcionalidades
🖥️ Interface de Usuário
Menu interativo em Display LCD 16x2 com I²C
🔋 Gestão de Energia
Modo de desligamento lógico
Animação de startup
Trava de segurança de 5 segundos
📊 Monitoramento em Tempo Real
Temperatura e Umidade (Sensor DHT22 ou DHT11)
Luminosidade (LDR em divisor de tensão)
Data e Hora (RTC DS1307 ou DS3231)
🚨 Sistema de Alertas
🟢 LED Verde → Condições normais
🔴 LED Vermelho + 🔊 Buzzer → Condições fora da faixa (gatilhos configuráveis)
🎮 Navegação
Botões com Pull-Down externo
Detecção de borda (ação ao soltar o botão)
🛠️ Hardware Necessário
1x Microcontrolador ATmega 328P (ou Arduino Uno)
1x Display LCD 16x2 com módulo I²C
1x Módulo RTC (DS1307 ou DS3231)
1x Sensor de Temperatura e Umidade (DHT22)
1x LDR (resistor dependente de luz)
1x Resistor 10kΩ (divisor de tensão do LDR)
2x Resistores 10kΩ (Pull-Down dos botões)
2x Push-buttons
1x Buzzer (ativo ou passivo)
2x LEDs (vermelho e verde)
2x Resistores 220Ω (para os LEDs)
📐 Pinagem do Projeto
Componente	Pino Arduino	Observação
DHT22 (Data)	D2	Protocolo Single-Bus
Buzzer	D3	Alerta sonoro
LED Verde	D4	Status OK
LED Vermelho	D5	Status de alerta
Botão 1	D6	Próximo / Ligar (Pull-Down)
Botão 2	D7	Confirmar (Pull-Down)
LDR	A0	Leitura analógica
SDA (LCD/RTC)	A4	Barramento I²C
SCL (LCD/RTC)	A5	Barramento I²C
⚙️ Configuração de Alertas (Triggers)

O sistema entra em estado de alerta quando:

Temperatura: < 15°C ou > 25°C
Umidade: < 30% ou > 50%
Luminosidade: > 30% (escala de 0 a 100%)
📌 Observações
Compatível com Arduino Uno e projetos standalone com ATmega328P
Ideal para aplicações de monitoramento ambiental, estufas e projetos acadêmicos
