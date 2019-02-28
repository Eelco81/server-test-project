
from selenium import webdriver
import chromedriver_binary  # Adds chromedriver binary to path

import time
from behave import given, when, then, step
from assertpy import assert_that, fail

@given( 'the user opened the web client' )
def step_impl( context ):
    context.driver = webdriver.Chrome()
    context.driver.get( context.url + "/home" )
    time.sleep( 3 )

@when( 'the user starts the simulation' )
def step_impl( context ):
    start_button = context.driver.find_element_by_id( 'button-start' )
    start_button.click()
    time.sleep( 30 )

@when( 'the user stops the simulation' )
def step_impl( context ):
    stop_button = context.driver.find_element_by_id( 'button-stop' )
    stop_button.click()
    time.sleep( 3 )

@when( 'the user closes the web client' )
def step_impl( context ):
    context.driver.close()
    
@then( 'there are {ammount:d} live plots visible' )
def step_impl( context, ammount ) :
    for i in range( 0, ammount ):
        id = 'sim-sampler-' + str( i )
        try:
            context.driver.find_element_by_id( id )
        except: 
            fail( 'live plot ' + id + ' is not found' )
        