import serial

device_port = "COM7"
baud_rate = 115200

serial_connection = serial.Serial(device_port, baud_rate)  # open serial port
        # Read the "Serial Initialized value sent"

while True:
    result = serial_connection.read()
    if result == b'\xcc' or result == b'\xcd':
        pass
    elif result == b'\xcb':
        print("\n")
    else:
        second_byte = serial_connection.read()
        # Combine first and second byte, then convert into a 16-bit number
        output_stream = result + second_byte
        # Communication is MSB, and the value is signed
        print(int.from_bytes(output_stream, 'little', signed=True), end=",")