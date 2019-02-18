
import json
from subprocess import run, PIPE
from behave import given, when, then, step
from assertpy import assert_that

class Response() :
    def __init__( self, content ):
        self.content = content
        
    def json( self ) :
        return json.loads( self.content )

@when( 'sending a {method} request on {route} using the command line client' )
def step_impl( context, method, route ):
    
    completedProcess = run(
        [
            '../Install/client.exe', 
            '-ip', context.ip, 
            '-port', context.port, 
            '-method', method, 
            '-path', route,
            '-loglevel', 'NONE'
        ],
        stdin=PIPE, stderr=PIPE, stdout=PIPE,
        cwd = "../Install" 
    )
    assert_that (completedProcess.returncode).is_equal_to (0)
    context.response = Response( completedProcess.stdout )

@when( 'calling {call} on the command line client' )
def step_impl( context, call ):

    completedProcess = run(
        [
            '../Install/client.exe', 
            '-' + call,
        ],
        stdin=PIPE, stderr=PIPE, stdout=PIPE,
        cwd = "../Install" 
    )
    assert_that (completedProcess.returncode).is_equal_to (1)
    context.response = Response( completedProcess.stdout )
