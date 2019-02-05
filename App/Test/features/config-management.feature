
Feature: Configuration management

  Scenario: Get all configurations
    When sending a GET request on /api/simulation/config
    Then the received response has code 200
    Then the received response contains all standard headers
    Then the received response body has json key "basic"
    Then the received response body has json key "heat"
    Then the received response body has json key "wave"