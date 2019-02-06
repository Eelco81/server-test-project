
import requests
import json
from behave import given, when, then, step
from assertpy import assert_that

@when( 'simulation "{sim}" is started' )
def step_impl( context, sim ):
    payload = json.dumps( { "id": sim } )
    response = requests.put( context.url + "/api/simulation", data = payload )
    assert_that( response.status_code ).is_equal_to( 202 )

@when( 'simulation "{sim}" is attempted to be started' )
def step_impl( context, sim ):
    payload = json.dumps( { "id": sim } )
    response = requests.put( context.url + "/api/simulation", data = payload )
    assert_that( response.status_code ).is_equal_to( 403 )

@when( 'the simulation is stopped' )
def step_impl( context ):
    response = requests.delete( context.url + "/api/simulation" )
    assert_that( response.status_code ).is_equal_to( 202 )

@when( 'the simulation is attempted to be stopped' )
def step_impl( context ):
    response = requests.delete( context.url + "/api/simulation" )
    assert_that( response.status_code ).is_equal_to( 304 )

@when( 'port {path} is set to value {value:f}' )
def step_impl( context, path, value ):
    payload = json.dumps( { "value": value } )
    response = requests.put( context.url + "/api/simulation/ports/" + path, data = payload )
    assert_that( response.status_code ).is_equal_to( 204 )

@then( 'the simulation is {status}' )
def step_impl( context, status ):
    response = requests.get( context.url + "/api/simulation" )
    assert_that( response.status_code ).is_equal_to( 200 )
    assert_that( response.json() ).contains_entry( { 'running' : status == "running" } )

@then( 'there are {ammount:d} ports of type "{type}" open' )
def step_impl( context, ammount, type ):
    response = requests.get( context.url + "/api/simulation/ports" )
    assert_that( response.status_code ).is_equal_to( 200 )
    assert_that( response.json() ).contains( "ports" )
    assert_that( response.json()["ports"] ).extracting( 'path', filter=lambda x: ("." + type + ".") in x['path'] ).is_length( ammount )

@then( 'port {path} has value {value:f}' )
def step_impl( context, path, value ):
    response = requests.get( context.url + "/api/simulation/ports/" + path )
    assert_that( response.status_code ).is_equal_to( 200 )
    assert_that( response.json() ).contains( "value" )
    assert_that( response.json()["value"] ).is_equal_to( value )

