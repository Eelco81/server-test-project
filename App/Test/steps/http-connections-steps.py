
import requests
from behave import given, when, then, step
from assertpy import assert_that

@when( 'sending a {method} request on {route}' )
def step_impl( context, method, route ):
    if method == "GET" :
        context.response = requests.get( context.url + route )
    elif method == "PUT":
        context.response = requests.put( context.url + route, data = "" )
    else :
        assert false

@when( 'sending an echo request of {bytes:d} bytes' )
def step_impl( context, bytes ) :
    context.response = requests.put( context.url + "/api/echo", data = bytearray( [0xFE]*bytes ) )

@then( 'the received response has code {code:d}' )
def step_impl( context, code ):
    assert_that( context.response.status_code ).is_equal_to( code )

@then( 'the received response contains a header with key \'{key}\'' )
def step_impl( context, key ):
    assert_that( context.response.headers.get( key ) ).is_not_none()

@then( 'the received response contains header \'{key}: {value}\'' )
def step_impl( context, key, value ):
    assert_that( context.response.headers.get( key ) ).is_equal_to( value )

@then( 'the received response contains all standard headers' )
def step_impl( context ):
    context.execute_steps(u'''
        Then the received response contains a header with key 'Last-Modified'
        Then the received response contains header 'User-Agent: HttpServer/0.0.1'
    ''')

@then( 'the received response has a body of size {bytes:d}' )
def step_impl( context, bytes ):
    assert_that( context.response.content ).is_length( bytes )
    if bytes > 0:
        assert_that( context.response.content ).contains_only( 0xFE )

@then( 'the received response body has json element {{ {key} : {value} }}' )
def step_imp( context, key, value ):
    assert_that( context.response.json() ).contains_entry( { key : value } )

@then( 'the received response body has json key "{key}"' )
def step_imp( context, key ):
    assert_that( context.response.json() ).contains( key )

@then( 'the received response body has json keys' )
def step_imp( context ):
    for row in context.table:
        assert_that( context.response.json() ).contains( row['key'] )
