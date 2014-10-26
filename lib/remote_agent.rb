require 'digiusb'
require 'colored'
require 'io/console'

class RemoteAgent
  def run
    sleep 0.5 until DigiUSB.sparks(false).length > 0
    spark = DigiUSB.sparks(false).last
    if !spark
      raise "Unable to connect to device"
    end
    
    $stderr.puts "Attached to #{spark.inspect.green}".blue 
    $stderr.puts "Type control + c to exit".blue 
    begin
      loop do
        char = spark.getc
        if char != ""
          puts char.ord.to_s.red
        else
          sleep 1.0 / 30.0
        end
        
        begin
          line = IO.console.read_nonblock(100)
          if match = line.match(/^([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)\n$/)
            led = match[1].to_i
            red = match[2].to_i
            green = match[3].to_i
            blue = match[4].to_i
            puts "Parsed '#{led}', #{red}, #{green}, #{blue}"
            spark.putc led
            spark.putc red
            spark.putc green
            spark.putc blue
          else
            puts "Unable to parse <led> <red> <green> <blue>"
          end
          # spark.putc char
        rescue Errno::EAGAIN
        end
      end
    rescue LIBUSB::ERROR_PIPE, LIBUSB::ERROR_NO_DEVICE
      $stderr.puts "" # newline
      $stderr.puts "Digispark disconnected".white_on_red 
      exit 2
    rescue DigiUSB::ErrorCrashed
      $stderr.puts "" # newline
      $stderr.puts "Digispark has crashed".white_on_red
      $stderr.puts " >> Probably need to call DigiUSB.refresh() more frequently in Digispark Program".blue
      exit 3
    rescue Interrupt
      $stderr.puts "" 
    end
  end
end