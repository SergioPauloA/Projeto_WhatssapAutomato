# WhatsApp Sender

Este repositório contém duas versões de uma aplicação para envio de mensagens via WhatsApp usando a API Twilio. A aplicação permite que o usuário informe um número de telefone do transmissor, forneça uma lista de destinatários e escreva uma mensagem para enviar a todos os destinatários fornecidos.

## Tecnologias Utilizadas

1. **Ruby (Tk)**
   - A versão Ruby usa o framework Tk para criar a interface gráfica.
   - A API Twilio é utilizada para enviar as mensagens via WhatsApp.

2. **C++ (Qt)**
   - A versão C++ usa o framework Qt para criar a interface gráfica.
   - A API Twilio é utilizada para enviar as mensagens via WhatsApp.

## Funcionalidades

1. **Configuração do número do transmissor**
   - O usuário pode inserir seu número de telefone para ser o transmissor das mensagens.

2. **Inserção de números dos destinatários**
   - O usuário pode inserir uma lista de números de telefone para os destinatários, um por linha.

3. **Escrita da mensagem**
   - O usuário pode escrever uma mensagem que será enviada para todos os destinatários.

4. **Envio das mensagens**
   - A aplicação envia a mensagem para todos os números fornecidos via WhatsApp utilizando a API Twilio.

5. **Barra de progresso**
   - Durante o envio das mensagens, uma barra de progresso é exibida para informar o status do envio.

## Como Rodar

### Ruby (Tk)

1. Instale o Ruby em seu sistema se ainda não o tiver: [Instruções de instalação do Ruby](https://www.ruby-lang.org/en/documentation/installation/).
2. Instale a gem `twilio-ruby`:
   ```bash
   gem install twilio-ruby
