
import _thread as thread
import sseclient

class EventSource():
    
    def __init__( self, url, onMessage ):
        
        self._sseclient = sseclient.SSEClient( url, chunk_size=1, headers={ "Accept": "text/event-stream" }, timeout=10 )
        
        def runSSE( service ):
            try:
                for event in service._sseclient:
                    onMessage( event.id, event.data )
                
            except Exception as e: print( e )
        
        thread.start_new_thread( runSSE, ( self, ) )
        
    def stop( self ):
        self._sseclient.resp.close()
    
