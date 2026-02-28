import socket
import time
import select

class EngineClient:
    def __init__(self, host='127.0.0.1', port=8080):
        self.host = host
        self.port = port
        self.sock = None
        self.remnants = ""
        
    def connect(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        for _ in range(10):
            try:
                self.sock.connect((self.host, self.port))
                self.sock.setblocking(False)
                return True
            except ConnectionRefusedError:
                time.sleep(0.5)
        return False
        
    def disconnect(self):
        if self.sock:
            self.sock.close()
            
    def send(self, msg):
        if not msg.endswith('\n'):
            msg += '\n'
        self.sock.sendall(msg.encode())
        
    def read_all(self, timeout=0.1):
        ready = select.select([self.sock], [], [], timeout)
        lines = []
        if ready[0]:
            try:
                data = self.sock.recv(65536).decode()
                full_data = self.remnants + data
                while '\n' in full_data:
                    line, full_data = full_data.split('\n', 1)
                    if line:
                        lines.append(line)
                self.remnants = full_data
            except BlockingIOError:
                pass
            except Exception as e:
                pass
        return lines
