
import time
import _thread as thread
from websocket import WebSocketApp

class WebSocketService():

    def __init__( self, url, onMessage ):
        
        def runWS( service ):
            service._wsapp.run_forever()
        
        self._wsevents = []
        self._wsapp = WebSocketApp( url, on_message=onMessage )
        thread.start_new_thread( runWS, ( self, ) )
    
    def stop( self ):
        self._wsapp.close()
