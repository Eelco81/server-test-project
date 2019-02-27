
import requests
import json
import time
from behave import given, when, then, step
from assertpy import assert_that

@when( 'running a {type} data stream at {step:d} ms of {count:d} packets of {size:d} bytes' )
def step_impl( context, type, step, count, size ):

    payload = json.dumps( { "step": step, "type": type, "count": count, "size": size } )
    response = requests.put( context.url + "/stream", data = payload )
    assert_that( response.status_code ).is_equal_to( 200 )

    # Wait for all events to be send
    time.sleep( 2 + step * count / 1000 / 1000 )

@then( 'the stream received {count:d} packets ({type}) of size {size:d} are received at interval {step:d} ms' )
def step_impl( context, count, type, size, step ):
    
    assert_that( context.events ).is_length( count )
   
    rstamps = [ float( e['recv']  ) for e in context.events ]
    sizes   = [ float( e['size']  ) for e in context.events ]
    sstamps = [ float( e['time']  ) for e in context.events ]
    ticks   = [ float( e['tick']  ) for e in context.events ]
    counts  = [ float( e['count'] ) for e in context.events ]
    ids     = [ float( e['id']    ) for e in context.events ]
    
    rframes = [ x-y for (x,y) in zip( rstamps[1:], rstamps[:-1] ) ]
    sframes = [ x-y for (x,y) in zip( sstamps[1:], sstamps[:-1] ) ]
    drifts  = [ y-x for (x,y) in zip( sstamps, rstamps ) ]
    incrmts = [ x-y for (x,y) in zip( ticks[1:], ticks[:-1] ) ]
    incrids = [ x-y for (x,y) in zip( ids[1:], ids[:-1] ) ]
    
    assert_that( sizes ).contains_only( size )
    assert_that( counts ).contains_only( count )
    assert_that( incrmts ).contains_only( 1 )
    assert_that( incrids ).contains_only( 1 )
    
    assert_that( sum( sframes ) / len( sframes ) ).is_close_to( step, 2001 )
    assert_that( min( sframes ) ).is_close_to( step, 5001 )
    assert_that( max( sframes ) ).is_close_to( step, 5002 )
    
    assert_that( sum( rframes ) / len( rframes ) ).is_close_to( step, 2002 )
    #assert_that( min( rframes ) ).is_close_to( step, 10001 )
    #assert_that( max( rframes ) ).is_close_to( step, 10002 )
    
    meandrift = sum( drifts ) / len( drifts )
    assert_that( meandrift ).is_less_than( 100003 )
    assert_that( min( drifts ) ).is_close_to( meandrift, 20001 )
    assert_that( max( drifts ) ).is_close_to( meandrift, 20002 )
    