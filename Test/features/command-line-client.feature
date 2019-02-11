
Feature: Command Line Client

  Scenario Outline: Calling the echo endpoint
    When sending a GET request on <path> using the command line client
    
  Examples: Binary messages
    | path            |
    | /api/version    |
    | /api/time       |
    | /api/simulation |