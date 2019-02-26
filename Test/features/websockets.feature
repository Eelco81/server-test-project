
@fixture.test-app
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

  Scenario Outline: Broadcasts
   Given the websocket service is listening
    When running a <type> data stream at <step> ms of <count> packets of <size> bytes
    Then the stream received <count> packets (<type>) of size <size> are received at interval <step> ms 
    Then the websocket service can be closed
    
  Examples:
    | step    | size | count | type   |
    | 20000   | 50   | 100   | string |
    | 20000   | 100  | 100   | string |
    | 20000   | 200  | 100   | string |
    | 50000   | 500  | 100   | string |
    | 100000  | 5000 | 100   | string |
