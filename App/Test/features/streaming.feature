
Feature: Data streaming

  Scenario Outline: Connection heartbeats
   Given the streaming connection is open
    When sending a ping with payload size <byte count>
    Then a pong is received with payload size <byte count>
     And the streaming connection can be closed
    
  Examples: Binary payloads
    | byte count |
    | 0          |
    | 125        |
    | 250        |
    | 500        |
    | 65000      |
    | 100000     |
    | 1000000    |