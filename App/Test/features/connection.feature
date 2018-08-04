
Feature: Communicating with the server

  Scenario: Open and close connection
    Given a client is connected to the server
     Then the client can be closed succesfully

  Scenario: Echo data (small size)
    Given a client is connected to the server
     When sending 64 bytes to the server
      And waiting for 0 seconds
     Then an echo command of 64 bytes will be received
      And the client can be closed succesfully
      
  Scenario: Echo data (large size)
    Given a client is connected to the server
     When sending 102400 bytes to the server
      And waiting for 2 seconds
     Then an echo command of 102400 bytes will be received
      And the client can be closed succesfully