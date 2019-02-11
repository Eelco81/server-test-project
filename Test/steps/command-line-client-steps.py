
from subprocess import run, PIPE
from behave import given, when, then, step
from assertpy import assert_that

@when( 'sending a {method} request on {route} using the command line client' )
def step_impl( context, method, route ):
    
    completedProcess = run(
        [
            '../Install/client.exe', 
            '-ip', context.ip, 
            '-port', context.port, 
            '-method', method, 
            '-path', route,
            '-loglevel', 'INFO'
        ],
        stdin=PIPE, stderr=PIPE, stdout=PIPE,
        cwd = "../Install" 
    )
    
    assert_that (completedProcess.returncode).is_equal_to (0)