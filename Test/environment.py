
from behave import fixture, use_fixture
from subprocess import Popen, PIPE
import time
import sys

sys.path.append ("lib")

@fixture 
def options( context, timeout=30, **kwargs ):
    context.port = "1704"
    context.websockport = "1706"
    context.sseport = "1708" 
    context.ip = "127.0.0.1"
    context.url = "http://" + context.ip + ":" + context.port
    context.websockurl = "ws://" + context.ip + ":" + context.websockport + "/web"
    context.sseurl = "http://"+ context.ip + ":" + context.sseport + "/sse"

@fixture
def server( context, timeout=30, **kwargs ):
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
    context.serverName = "Server"
    time.sleep( 1 )
    
    yield context.server
    
    context.server.communicate( input=b'quit\n' )
    time.sleep( 1 )
    context.server.kill()
    time.sleep( 1 )

@fixture
def testapp( context, timeout=30, **kwargs ):
    context.server = Popen(
        [
            '../Install/testapp.exe', 
            '-ip', context.ip, 
            '-port', context.port, 
            '-websockport', context.websockport, 
            '-sseport', context.sseport,
            '-loglevel', 'INFO'
        ], 
        stdin=PIPE, stderr=PIPE, stdout=PIPE,
        cwd = "../Install"
    )
    context.serverName = "test-app"
    time.sleep( 1 )
    
    yield context.server
    
    context.server.communicate( input=b'quit\n' )
    time.sleep( 1 )
    context.server.kill()
    time.sleep( 1 )
    
def before_tag( context, tag ):
    if tag == "fixture.server":
        use_fixture( options, context, timeout=10 )
        use_fixture( server, context, timeout=10 )
    if tag == "fixture.test-app":
        use_fixture( options, context, timeout=10 )
        use_fixture( testapp, context, timeout=10 )
        
    