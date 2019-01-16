
Feature: Simulator

  Scenario: Configuration management
    When sending a GET request on /api/simulation/config
    Then the received response has code 200
    Then the received response contains all standard headers
    Then the received response contains simulation "basic"
    Then the received response contains simulation "heat"
    Then the received response contains simulation "wave"

  Scenario: Starting and stopping simulations
   Given the event service is connected
    When simulation "basic" is started
    Then the simulation is running
    Then event "sim-started" is received
    Then event "sim-sampler-0" is received
    Then event "sim-sampler-1" is received
    When the simulation is stopped
    Then the simulation is not running
    Then event "sim-stopped" is received
    Then the event service can be closed
