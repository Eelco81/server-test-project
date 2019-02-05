
from websocket import create_connection
from behave import given, when, then, step
from assertpy import assert_that

@given( 'the websocket connection is open' )
def step_impl( context ):
    context.ws = create_connection( context.websockurl )

@when( 'sending a ping with payload size {size:d}' )
def step_impl( context, size ):
    context.ws.ping( bytearray( [0xFE]*size ) )
    
@then( 'a pong is received with payload size {size:d}' )
def step_impl( context, size ):
    ( opcode, payload ) = context.ws.recv_data( True )
    assert_that( opcode ).is_equal_to ( 0xA )
    assert_that( payload ).is_length( size )
    if size > 0:
        assert_that( payload ).contains_only( 0xFE )

@then( 'the websocket connection can be closed' )
def step_impl( context ):
    context.ws.close()
    