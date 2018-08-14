
Feature: Basic routing

  Scenario: Calling non existing endpoints
    When sending a GET request on /i/do/not/exist
    Then the received response has code 404
     And the received response contains a header with key 'Host'
     And the received response contains a header with key 'Last-Modified'
     And the received response contains header 'User-Agent: HttpServer/0.0.1'
     And the received response contains header 'Content-Length: 0'
     And the received response contains all standard headers

  Scenario: Calling the echo endpoint
    When sending a PUT request on /system/echo
    Then the received response has code 404
     And the received response contains all standard headers

