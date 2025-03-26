require 'tk'
require 'twilio-ruby'

class WhatsAppSender
  attr_accessor :transmitter_number, :receiver_numbers, :message

  def initialize
    @transmitter_number = ''
    @receiver_numbers = []
    @message = 'Escreva aqui a mensagem do coração'
    create_transmitter_screen
  end

  def create_transmitter_screen
    @root = TkRoot.new { title "Enviar Mensagem via WhatsApp" }

    TkLabel.new(@root) do
      text "Digite o seu número de telefone:"
      pack
    end

    @transmitter_entry = TkEntry.new(@root) do
      width 20
      pack
    end

    TkButton.new(@root) do
      text "Usar esse número"
      command { set_transmitter_number }
      pack
    end

    Tk.mainloop
  end

  def set_transmitter_number
    @transmitter_number = @transmitter_entry.get
    if valid_phone_number?(@transmitter_number)
      @root.destroy
      create_receiver_screen
    else
      Tk.messageBox('message' => "Número de telefone inválido!", 'type' => 'ok')
    end
  end

  def create_receiver_screen
    @root = TkRoot.new { title "Informe os números de telefone" }

    TkLabel.new(@root) do
      text "Por favor, informe os números de telefone:"
      pack
    end

    @receiver_text = TkText.new(@root) do
      width 30
      height 10
      wrap 'word'
      pack
    end

    TkButton.new(@root) do
      text "Pronto"
      command { set_receiver_numbers }
      pack
    end

    Tk.mainloop
  end

  def set_receiver_numbers
    @receiver_numbers = @receiver_text.get("1.0", "end").strip.split("\n")
    if @receiver_numbers.empty? || @receiver_numbers.any? { |num| !valid_phone_number?(num) }
      Tk.messageBox('message' => "Por favor, insira números válidos!", 'type' => 'ok')
    else
      @root.destroy
      create_message_screen
    end
  end

  def create_message_screen
    @root = TkRoot.new { title "Escreva a mensagem" }

    TkLabel.new(@root) do
      text "Escreva aqui a mensagem do coração:"
      pack
    end

    @message_entry = TkText.new(@root) do
      width 50
      height 10
      wrap 'word'
      pack
    end

    TkButton.new(@root) do
      text "Enviar a todos"
      command { send_messages }
      pack
    end

    Tk.mainloop
  end

  def send_messages
    @message = @message_entry.get("1.0", "end").strip
    if @message.empty?
      Tk.messageBox('message' => "A mensagem não pode ser vazia!", 'type' => 'ok')
    else
      @root.destroy
      create_progress_screen
    end
  end

  def create_progress_screen
    @root = TkRoot.new { title "Enviando mensagens..." }

    @progress_label = TkLabel.new(@root) do
      text "Enviando"
      foreground 'yellow'
      pack
    end

    @progress_bar = TkProgressbar.new(@root) do
      maximum @receiver_numbers.size
      pack
    end

    @receiver_numbers.each_with_index do |number, index|
      Tk.after(index * 1500) { send_message_to(number, index) }
    end

    Tk.mainloop
  end

  def send_message_to(number, index)
    begin
      client = Twilio::REST::Client.new('ACXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX', 'your_auth_token')
      message = client.messages.create(
        from: "whatsapp:+14155238886", # Número de WhatsApp da Twilio
        to: "whatsapp:#{number}",
        body: @message
      )

      @progress_bar.value = index + 1
      @progress_label.text = "Pronto - #{number}"
      @progress_label.foreground = 'green'
    rescue => e
      @progress_label.text = "Erro - #{number}"
      @progress_label.foreground = 'red'
    end
  end

  def valid_phone_number?(number)
    number =~ /\A\(\d{2}\) \d{5}-\d{4}\z/
  end
end

WhatsAppSender.new
