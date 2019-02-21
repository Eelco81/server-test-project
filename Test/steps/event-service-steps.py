
import _thread as thread
import json
import requests
import sseclient
import time
from websocket import WebSocketApp
from behave import given, when, then, step
from assertpy import assert_that

class WebSockService():

    def __init__( self, url ):
        
        def onMessage( ws, message ):
            # print ("ws:  " + message)
            self._wsevents.append( json.loads( message ) )
        
        def runWS( service ):
            service._wsapp.run_forever()
        
        self._wsevents = []
        self._wsapp = WebSocketApp( url, on_message=onMessage )
        thread.start_new_thread( runWS, ( self, ) )
    
    def getEvents( self ):
        return self._wsevents
    
    def stop( self ):
        self._wsapp.close()

class EventSourceService():
    
    def __init__( self, url ):
        
        def runSSE( service ):
            for event in self._sseclient.events():
                # print ("sse: " + event.data)
                self._sseevents.append( json.loads( event.data ) )
        
        response = requests.get( url, stream=True, headers={ "Accept": "text/event-stream" } )
        assert_that( response.status_code ).is_equal_to( 200 )
        
        self._sseevents = []
        self._sseclient = sseclient.SSEClient( response )
        thread.start_new_thread( runSSE, ( self, ) )
        
    def getEvents( self ):
        return self._sseevents
        
    def stop( self ):
        # \todo: 
        # self._sseclient.close()
        pass

@given( 'the event service is connected' )
def step_impl( context ):
    context.wsservice = WebSockService( context.websockurl )
    context.sseservice = EventSourceService( context.sseurl )

@then( 'the event service can be closed' )
def step_impl( context ):
    context.wsservice.stop()
    context.sseservice.stop()

@then( 'event "{id}" is received' )
def step_impl( context, id ):
    assert_that( context.wsservice.getEvents() ).extracting( "event-id" ).contains( id )
    assert_that( context.sseservice.getEvents() ).extracting( "event-id" ).contains( id )
