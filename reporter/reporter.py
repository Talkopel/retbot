import socket
import time
import struct
import cffi

LOGSLOCATION = "/proc/retbot_logs"
CONNECTPORT = 3332
CONNECTIP = "127.0.0.1"

ffi = cffi.FFI()

def get_logs(location):
	with open(location, "r") as f:
		data = f.read()
	return data

def create_message(report):
	m_len = len(report)
	data = struct.pack(">I",m_len) + report
	print "Sending: %d" % m_len
	return data

def send_message(ip, port, message):
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((ip, port))
	s.sendall(message)
	s.close()

if __name__ == "__main__":

	# Create tool 
	lib = ffi.dlopen("logreader")

	# while True:
	# 	time.sleep(3)
	# 	logs = get_logs(LOGSLOCATION)
	# 	to_send = create_message(logs)
	# 	send_message(CONNECTIP, CONNECTPORT, to_send)
