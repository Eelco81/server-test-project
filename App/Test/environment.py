
from subprocess import Popen, PIPE
import time

def before_feature(context, feature):
    context.port = 1703
    context.server = Popen(['../Make/serveraplication.exe', '-port', str(context.port), '-loglevel', 'TRACE'], stdin=PIPE) # stderr=PIPE, stdout=PIPE)
    time.sleep(3)

def after_feature(context, feature):
    context.server.communicate(input=b'quit\n')
    time.sleep(3)
    context.server.kill()