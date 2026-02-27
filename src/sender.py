import serial

PORT = 'COM6' # set as needed

arduino = serial.Serial(port=PORT, baudrate=9600)

file1 = open('./drawing_to_send.txt', 'r')
Lines = file1.readlines()


for i in range(3):
    print(arduino.readline().decode('utf-8').rstrip())


def write(coord):
    arduino.write(bytes(coord, 'utf-8'))

count = 0
for line in Lines:
    count += 1
    if "X" in line or "Y" in line or "M" in line:
        write(line)
        print(line)
        if "X" in line or "Y" in line:
            while arduino.readline().decode('utf-8').rstrip() != "ok":
                print(arduino.readline().decode('utf-8').rstrip())
    