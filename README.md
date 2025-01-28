# Projeto: Controle de LEDs com Interrupções e Debouncing

Este projeto utiliza o microcontrolador **RP2040** na placa **BitDogLab** para controlar uma matriz de LEDs WS2812, além de LEDs RGB e botões com interrupções e debouncing. 

## 🎯 Objetivo

- Compreender o uso de interrupções em microcontroladores.
- Implementar debouncing via software para eliminar o efeito de bouncing nos botões.
- Controlar LEDs RGB e LEDs endereçáveis (WS2812).
- Desenvolver um projeto funcional combinando hardware e software.

## 🛠️ Componentes Utilizados

1. **Matriz de LEDs 5x5 WS2812** (GPIO 7)
2. **LED RGB**:
   - Vermelho: GPIO 13
   - Verde: GPIO 11
   - Azul: GPIO 12
3. **Botões**:
   - Botão A: GPIO 5
   - Botão B: GPIO 6

## 📋 Funcionalidades

1. **LED RGB:**
   - O LED vermelho pisca continuamente 5 vezes por segundo.
   
2. **Matriz de LEDs:**
   - Exibe números de **0 a 9** em formato fixo.
   - O botão **A** incrementa o número exibido na matriz.
   - O botão **B** decrementa o número exibido na matriz.

3. **Interrupções e Debouncing:**
   - Uso de interrupções para detectar os botões A e B.
   - Implementação de debouncing via software para evitar leituras falsas.

## 📹 Demonstração
Veja o vídeo da demonstração no YouTube através do link abaixo:

[Vídeo de Demonstração do Projeto](https://youtu.be/HsksHqiZUsE)

Ou assista diretamente no repositório:

[Demonstração do Projeto](./Video-Demostração-Do-Codigo-Na-Placa-BitDogLab.mp4)

---

## 📂 Arquivos do Projeto

- **`tarefa-interrupcoes.c`**: Código-fonte do projeto.
- **`pio_matrix.pio`**: Programa PIO para controle da matriz WS2812.
- **`Video-Demostração-Do-Codigo-Na-Placa-BitDogLab.mp4`**: Vídeo de demonstração do projeto.
