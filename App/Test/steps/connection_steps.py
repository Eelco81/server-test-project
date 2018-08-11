
import requests

from behave import given, when, then, step

@when('sending a GET request on {route}')
def step_impl(context, route):
    context.response = requests.get(context.url + route)

@then('the received response has code {code:d}')
def step_impl(context, code):
    assert context.response.status_code == code

@then('the received response contains a header with key \'{key}\'')
def step_impl(context, key):
    assert context.response.headers.get(key) != None
    
@then('the received response contains header \'{key}: {value}\'')
def step_impl(context, key, value):
    assert context.response.headers.get(key) == value