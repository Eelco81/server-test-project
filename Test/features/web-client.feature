
@fixture.server
Feature: Web Client

  @wip
  Scenario: Starting and stopping simulations
   Given the user opened the web client
    When the user starts the simulation
    Then the simulation is running
    Then there are 3 live plots visible
    When the user stops the simulation
    Then the simulation is not running
    When the user closes the web client
