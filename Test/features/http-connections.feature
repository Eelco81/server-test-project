
@fixture.test-app
Feature: HTTP Connections

  Scenario: Calling non existing endpoints
    When sending a GET request on /i/do/not/exist
    Then the received response has code 404
    Then the received response contains a header with key 'Last-Modified'
    Then the received response contains header 'User-Agent: test-app/0.0.1'
    Then the received response contains header 'Content-Length: 0'
    Then the received response contains all standard headers
    
  Scenario Outline: Calling the echo endpoint
    When sending an echo request of <byte count> bytes
    Then the received response has code <response>
    Then the received response contains all standard headers
    Then the received response has a body of size <byte count>
    
  Examples: Binary messages
    | byte count | response |
    | 0          | 200      |
    | 1          | 200      |
    | 10         | 200      |
    | 100        | 200      |
    | 1000       | 200      |
    | 10000      | 200      |
    | 100000     | 200      |
    | 1000000    | 200      |
