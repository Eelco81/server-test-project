
from websocket import create_connection
from behave import given, when, then, step

@given('the streaming connection is open')
def step_impl(context):
    context.ws = create_connection(context.websockurl)

@when('sending a ping with payload size {size:d}')
def step_impl(context, size):
    context.ws.ping(bytearray([0xFE]*size))
    
@then('a pong is received with payload size {size:d}')
def step_impl(context, size):
    (opcode, payload) = context.ws.recv_data (True)
    assert opcode == 0xA
    print (len(payload))
    assert len(payload) == size
    for byte in payload:
        assert byte == 0xFE

@then('the streaming connection can be closed')
def step_impl(context):
    context.ws.close ()
    