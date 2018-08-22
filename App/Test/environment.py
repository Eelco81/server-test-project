
from subprocess import Popen, PIPE
import time

def before_feature(context, feature):
    context.port = "1703"
    context.ip = "127.0.0.1"
    context.url = "http://" + context.ip + ":" + context.port
    context.server = Popen(['../Make/serveraplication.exe', '-ip', context.ip, '-port', context.port, '-loglevel', 'INFO'], 
                            stdin=PIPE, # stderr=PIPE, stdout=PIPE,
                            cwd = "../..")
    time.sleep(3)

def after_feature(context, feature):
    context.server.communicate(input=b'quit\n')
    time.sleep(3)
    context.server.kill()
    time.sleep(2)