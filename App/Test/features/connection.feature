
Feature: Basic routing

  Scenario: Calling non existing endpoints
    When sending a GET request on /i/do/not/exist
    Then the received response has code 404
     And the received response contains a header with key 'Host'
     And the received response contains a header with key 'Last-Modified'
     And the received response contains header 'User-Agent: HttpServer/0.0.1'
     And the received response contains header 'Content-Length: 0'
     And the received response contains all standard headers

  Scenario Outline: Calling the echo endpoint
    When sending an echo request of <byte count in> bytes
    Then the received response has code <response>
     And the received response contains all standard headers
     And the received response has a body of size <byte count out>

  Examples: Amphibians
   | byte count in | byte count out | response |
   | 10            | 10             | 200      |
   | 100           | 100            | 200      |
   | 1000          | 1000           | 200      |