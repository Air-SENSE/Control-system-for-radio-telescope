import socket

#dien dia chi IP cua server muon ket noi
# Vi DU: ServerIP = "192.168.1.8"
ServerIP = "192.168.1.33"
ServerPort = 1999 

TCPClientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

TCPClientSocket.connect((ServerIP, ServerPort))

print("TCP client connected to server and ready to perform read write operations")
print(f"Connected by server {ServerIP}")

receivedData = ""
bufferSize = 1024

print("client send first")
print("waiting to receive data from the keyboard")

switchState = 1

# def sendData():
    

while True:
    if switchState:
        msg = input()

        if len(msg) > 0:
            dataToSend = (msg + '$').encode('utf-8')
            # print(dataToSend)

            try:
                TCPClientSocket.sendall(dataToSend)
                
            except:
                print("something's wrong, client can't send data to server")
                break

            if msg == "close":
                print("client disconnected")
                TCPClientSocket.close()
                break
        
        switchState = not switchState
        print("client is wating for incoming data")
    
    if not switchState:
        try:
            receivedData = TCPClientSocket.recv(bufferSize).decode('utf-8')
        except:
            print("something's wrong, client can't receive data from server")

        if not receivedData: 
            break
        else:
            print("[FromServer]: " + receivedData)

        switchState = not switchState
        print("waiting to receive data from the keyboard")

        

   

