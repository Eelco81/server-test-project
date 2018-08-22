
import requests

from behave import given, when, then, step

@when('sending a {method} request on {route}')
def step_impl(context, method, route):
    if method == "GET" :
        context.response = requests.get(context.url + route)
    elif method == "PUT":
        context.response = requests.put(context.url + route, data = "")
    else :
        assert false

@when ('sending an echo request of {bytes:d} bytes')
def step_impl(context, bytes) :
    context.response = requests.put(context.url + "/api/echo", data = bytearray([0xFE]*bytes))

@then('the received response has code {code:d}')
def step_impl(context, code):
    assert context.response.status_code == code

@then('the received response contains a header with key \'{key}\'')
def step_impl(context, key):
    assert context.response.headers.get(key) != None

@then('the received response contains header \'{key}: {value}\'')
def step_impl(context, key, value):
    assert context.response.headers.get(key) == value

@then('the received response contains all standard headers')
def step_impl(context):
    assert context.response.headers.get('Last-Modified') != None
    assert context.response.headers.get('User-Agent') == 'HttpServer/0.0.1'
    print (context.response.headers.get('Host'))
    assert context.response.headers.get('Host') == context.ip + ":" + context.port

@then('the received response has a body of size {bytes:d}')
def step_impl(context, bytes):
    assert len(context.response.content) == bytes
    for byte in context.response.content:
        assert byte == 0xFE

@then('the received response body has json element {{ {key} : {value} }}')
def step_imp(context, key, value):
    body = context.response.json()
    assert body[key] == value

@then('the received response body has json key "{key}"')
def step_imp(context, key):
    body = context.response.json()
    assert body[key] is not None
