
import _thread as thread
import json
from websocket import WebSocketApp
from behave import given, when, then, step
from assertpy import assert_that

class EventService():
    
    def __init__( self, url ):
    
        def onMessage( ws, message ):
            self._events.append( json.loads( message ) )
        
        def run( service ):
            service._app.run_forever()
        
        self._events = []
        self._app = WebSocketApp( url, on_message = onMessage )
        thread.start_new_thread( run, ( self, ) )
    
    def getEvents( self ):
        return self._events
        
    def stop( self ):
        self._app.close()

@given( 'the event service is connected' )
def step_impl( context ):
    context.service = EventService( context.websockurl )

@then( 'the event service can be closed' )
def step_impl( context ):
    context.service.stop()

@then( 'event "{id}" is received' )
def step_impl( context, id ):
    assert_that( context.service.getEvents() ).extracting( "event-id" ).contains( id )

