
from subprocess import Popen, PIPE
import time

def before_feature( context, feature ):
    context.port = "1704"
    context.websockport = "1706"
    context.sseport = "1708" 
    context.ip = "127.0.0.1"
    context.url = "http://" + context.ip + ":" + context.port
    context.websockurl = "ws://" + context.ip + ":" + context.websockport + "/web"
    context.sseurl = "http://"+ context.ip + ":" + context.sseport + "/sse"
    context.server = Popen(
        [
            '../Install/server.exe', 
            '-ip', context.ip, 
            '-port', context.port, 
            '-websockport', context.websockport, 
            '-sseport', context.sseport,
            '-loglevel', 'NONE'
        ], 
        stdin=PIPE, stderr=PIPE, stdout=PIPE,
        cwd = "../Install"
    )
    time.sleep( 3 )

def after_feature( context, feature ):
    context.server.communicate( input=b'quit\n' )
    time.sleep( 3 )
    context.server.kill()
    time.sleep( 2 )
