
import _thread as thread
import requests
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

@then( 'the received response contains simulation "{sim}"' )
def step_impl( context, sim ):
    body = context.response.json()
    assert_that( body ).contains( sim )

@when( 'simulation "{sim}" is started' )
def step_impl( context, sim ):
    payload = json.dumps( { "id": sim } )
    response = requests.put( context.url + "/api/simulation", data = payload )
    assert_that( response.status_code ).is_equal_to( 202 )

@when( 'the simulation is stopped' )
def step_impl( context ):
    response = requests.delete( context.url + "/api/simulation" )
    assert_that( response.status_code ).is_equal_to( 202 )

@then( 'the simulation is {status}' )
def step_impl( context, status ):
    response = requests.get( context.url + "/api/simulation" )
    assert_that( response.status_code ).is_equal_to( 200 )
    assert_that( response.json() ).contains_entry( { 'running' : status == "running" } )

@then( 'event "{id}" is received' )
def step_impl( context, id ):
    assert_that( context.service.getEvents() ).extracting( "event-id" ).contains( id )
