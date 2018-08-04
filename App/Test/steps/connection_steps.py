
import time
import sys
import socket

from behave import given, when, then, step


@given('a client is connected to the server')
def step_impl(context):
    context.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    context.client.connect(("127.0.0.1", context.port))
    time.sleep(1)

@when('sending {length:d} bytes to the server')
def step_impl(context, length):
    data = bytearray(length)
    size = context.client.send(data)
    assert size == length

@then('an echo command of {length:d} bytes will be received')
def step_impl(context, length):
    data = bytearray(context.client.recv(length))
    assert len(data) == length
    
@then('the client can be closed succesfully')
def step_impl(context):
    context.client.close()
    time.sleep(5)
