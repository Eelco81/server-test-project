
import _thread as thread
import requests
import sseclient
from assertpy import assert_that

class EventSource():
    
    def __init__( self, url, onMessage ):
        
        def runSSE( service ):
            for event in self._sseclient.events():
                onMessage (event.data)
        
        response = requests.get( url, stream=True, headers={ "Accept": "text/event-stream" } )
        assert_that( response.status_code ).is_equal_to( 200 )
        
        self._sseclient = sseclient.SSEClient( response )
        thread.start_new_thread( runSSE, ( self, ) )
        
    def stop( self ):
        self._sseclient.close()
