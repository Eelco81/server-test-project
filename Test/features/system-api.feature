
@fixture.server
Feature: System API

  Scenario: Retrieving the version number
    When sending a GET request on /api/version
    Then the received response has code 200
    Then the received response contains header 'Content-Type: application/json'
    Then the received response contains all standard headers
    Then the received response body has json element { version : 0.0.1 }
    Then the received response body has json element { application : Server }
     
  Scenario: Retrieving the server time
    When sending a GET request on /api/time
    Then the received response has code 200 
    Then the received response contains header 'Content-Type: application/json'
    Then the received response contains all standard headers
    Then the received response body has json element { unit : ms }
    Then the received response body has json key "value"
    