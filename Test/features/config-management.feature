
@fixture.server
Feature: Configuration management

  Scenario: Get all configurations
    When sending a GET request on /api/simulation/config
    Then the received response has code 200
    Then the received response contains all standard headers
    Then the received response body has json keys
        | key   |
        | basic |
        | heat  |
        | wave  |
    
  Scenario Outline: Get a single configuration
    When sending a GET request on /api/simulation/config/<item>
    Then the received response has code 200
    Then the received response contains all standard headers
  
  Examples: Items
    | item       |
    | basic      |
    | heat       |
    | wave       |
