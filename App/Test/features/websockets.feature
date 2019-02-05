
Feature: Websockets (RFC6455)

  Scenario Outline: Heartbeats
   Given the websocket connection is open
    When sending a ping with payload size <byte count>
    Then a pong is received with payload size <byte count>
    Then the websocket connection can be closed
    
  Examples: Binary payloads
    | byte count |
    | 0          |
    | 125        |
    | 250        |
    | 500        |
    | 65000      |
    | 100000     |
    | 1000000    |
