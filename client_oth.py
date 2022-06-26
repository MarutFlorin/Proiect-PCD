import socket, os

IP = "127.0.0.1"
PORT = 8080
ADDR = (IP, PORT)
BUFFER_SIZE = 1024

print("Starting ")
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.connect(ADDR)

sent_file_name = input("File path: ")
file_size = os.path.getsize(sent_file_name)

# server.send(file_name.encode())
# client.send(str(file_size).encode())

with open(sent_file_name, "rb") as file:
    c = 0

    while c <= file_size:
        data = file.read(BUFFER_SIZE)
        if not (data):
            break
        server.sendall(data)
        c += len(data)

print("File transfer is complete")

received_file_name = "from_server_oth"

with open(received_file_name, "wb") as file:
    while True:
        bytes_read = server.recv(BUFFER_SIZE)
        if not bytes_read:
            print("No more byes to read")    
            break

        f.write(bytes_read)

print("File was received")