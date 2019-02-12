
Feature: Command Line Client

  Scenario Outline: Calling the echo endpoint
    When sending a GET request on <path> using the command line client
    Then the received response body has json element <body>
    
  Examples: Binary messages
    | path            | body                      |
    | /api/version    | { version : 0.0.1 }       |
    | /api/version    | { application : Server }  |
    | /api/time       | { unit : ms }             |