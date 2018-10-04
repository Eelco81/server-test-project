
Feature: Basic routing

  Scenario: Calling non existing endpoints
    When sending a GET request on /i/do/not/exist
    Then the received response has code 404
     And the received response contains a header with key 'Last-Modified'
     And the received response contains header 'User-Agent: HttpServer/0.0.1'
     And the received response contains header 'Content-Length: 0'
     And the received response contains all standard headers
    
  Scenario Outline: Calling the echo endpoint
    When sending an echo request of <byte count> bytes
    Then the received response has code <response>
     And the received response contains all standard headers
     And the received response has a body of size <byte count>
    
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
    
  Scenario: Loading the home page
    When sending a GET request on /home
    Then the received response has code 200
     And the received response contains header 'Content-Type: text/html'
     And the received response contains all standard headers
    
  Scenario: Retrieving the version number
    When sending a GET request on /api/version
    Then the received response has code 200
     And the received response contains header 'Content-Type: application/json'
     And the received response contains all standard headers
     And the received response body has json element { version : 0.0.1 }
     And the received response body has json element { application : HttpServer }
     
  Scenario: Retrieving the server time
    When sending a GET request on /api/time
    Then the received response has code 200 
     And the received response contains header 'Content-Type: application/json'
     And the received response contains all standard headers
     And the received response body has json element { unit : ms }
     And the received response body has json key "value"
     
  