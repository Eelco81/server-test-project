
import time
import json
from behave import given, when, then, step
from assertpy import assert_that
from eventsource import EventSource

@given( 'the event source is listening' )
def step_impl( context ):
    
    context.events = []
    def onMessage( id, message ):
        event = json.loads( message )
        event['size'] = len( message )
        event['recv'] = time.time() * 1000 * 1000
        event['id'] = id
        context.events.append( event )
        
    context.service = EventSource( context.sseurl, onMessage )
    
    time.sleep( 0.1 )

@then( 'the event source can be closed' )
def step_impl( context ):
    context.service.stop()


