require 'tk'

class WhatsAppSender
  attr_accessor :root, :transmitter_number, :receiver_numbers, :message

  def initialize
    @transmitter_number = ""
    @receiver_numbers = []
    @message = "Escreva aqui a mensagem do coração"
    create_transmitter_screen
  end

  def create_transmitter_screen
    @root = TkRoot.new { title "Configurar Número Transmissor" }
    
    TkLabel.new(@root, text: "Digite seu número de telefone:", font: "Arial 12").pack(pady: 10)
    entry = TkEntry.new(@root, font: "Arial 12")
    entry.pack(pady: 5)

    TkButton.new(@root, text: "Usar este número", command: proc {
      @transmitter_number = entry.value.strip
      if @transmitter_number.match?(/^\d{11}$/)
        @root.destroy
        create_receiver_screen
      else
        Tk.messageBox(message: "Digite um número válido com 11 dígitos!")
      end
    }).pack(pady: 10)

    Tk.mainloop
  end

  def create_receiver_screen
    @root = TkRoot.new { title "Inserir Números de Destino" }

    TkLabel.new(@root, text: "Informe os números de telefone:", font: "Arial 12").pack(pady: 10)
    
    text_area = TkText.new(@root, height: 10, width: 30, font: "Arial 12")
    text_area.pack(pady: 5)

    TkButton.new(@root, text: "Pronto", command: proc {
      input_numbers = text_area.get("1.0", "end").split("\n").map(&:strip)
      valid_numbers = input_numbers.select { |n| n.match?(/^\(\d{2}\) \d{5}-\d{4}$/) }
      
      if valid_numbers.empty?
        Tk.messageBox(message: "Digite pelo menos um número válido!")
      else
        @receiver_numbers = valid_numbers
        @root.destroy
        create_message_screen
      end
    }).pack(pady: 10)

    Tk.mainloop
  end

  def create_message_screen
    @root = TkRoot.new { title "Escrever Mensagem" }

    TkLabel.new(@root, text: "Digite sua mensagem:", font: "Arial 12").pack(pady: 10)
    text_area = TkText.new(@root, height: 5, width: 40, font: "Arial 12")
    text_area.insert("1.0", @message)
    text_area.pack(pady: 5)

    TkButton.new(@root, text: "Enviar a todos", command: proc {
      @message = text_area.get("1.0", "end").strip
      if @message.empty?
        Tk.messageBox(message: "A mensagem não pode estar vazia!")
      else
        @root.destroy
        create_progress_screen
      end
    }).pack(pady: 10)

    Tk.mainloop
  end

  def create_progress_screen
    @root = TkRoot.new { title "Enviando Mensagens..." }

    progress_bar = TkCanvas.new(@root, width: 400, height: 30, bg: "white").pack(pady: 10)
    label_status = TkLabel.new(@root, text: "", font: "Arial 12").pack(pady: 5)

    @receiver_numbers.each_with_index do |number, index|
      label_status.text = "#{number} - Enviando..."
      Tk.update
      sleep(1)  # Simula o envio real, aqui chamaremos a API Twilio
      progress_bar.create_rectangle(0, 0, (400 * ((index + 1).to_f / @receiver_numbers.length)), 30, fill: "green")
      label_status.text = "#{number} - Pronto"
      Tk.update
      sleep(0.5)
    end

    Tk.mainloop
  end
end

WhatsAppSender.new
