
Feature: Communicating with the server

  Scenario: Echo data
    Given a client is connected to the server
     When sending 1000 bytes to the server
     Then an echo command of 1000 bytes will be received
      And the client can be closed succesfully