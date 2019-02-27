
import time
import json
from websocket import create_connection
from behave import given, when, then, step
from assertpy import assert_that
from websocketservice import WebSocketService

@given( 'the websocket connection is open' )
def step_impl( context ):
    context.ws = create_connection( context.websockurl )

@given( 'the websocket service is listening' )
def step_impl( context ):
    
    context.events = []
    def onMessage( ws, message ):
        event = json.loads( message )
        event['size'] = len( message )
        event['recv'] = time.time() * 1000 * 1000
        event['id'] = event['tick']
        context.events.append( event )
    
    context.service = WebSocketService( context.websockurl, onMessage )
    
    time.sleep( 0.1 )

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

@then( 'the websocket service can be closed' )
def step_impl( context ):
    context.service.stop()

