
import _thread as thread
import json
import requests
import sseclient
import time
from websocket import WebSocketApp
from behave import given, when, then, step
from assertpy import assert_that

from eventsource import EventSource
from websocketservice import WebSocketService

@given( 'the event service is connected' )
def step_impl( context ):
    
    context.wsevents = []
    def onWsMessage( ws, message ):
        event = json.loads( message )
        context.wsevents.append( event )
    
    context.wsservice = WebSocketService( context.websockurl, onWsMessage )
   
    context.sseevents = []
    def onSseMessage( id, message ):
        event = json.loads( message )
        context.sseevents.append( event )
    
    context.sseservice = EventSource( context.sseurl, onSseMessage )
    
    time.sleep( 0.1 )
    
@then( 'the event service can be closed' )
def step_impl( context ):
    context.wsservice.stop()
    context.sseservice.stop()

@then( 'event "{id}" is received' )
def step_impl( context, id ):
    assert_that( context.wsevents ).extracting( "event-id" ).contains( id )
    assert_that( context.sseevents ).extracting( "event-id" ).contains( id )
