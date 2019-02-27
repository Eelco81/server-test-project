
@fixture.test-app
Feature: Event Source (SSE)

  Scenario Outline: Broadcasts
   Given the event source is listening
    When running a <type> data stream at <step> ms of <count> packets of <size> bytes
    Then the stream received <count> packets (<type>) of size <size> are received at interval <step> ms 
    Then the event source can be closed
    
  Examples:
    | step    | size | count | type   |
    | 20000   | 50   | 100   | string |
    | 20000   | 100  | 100   | string |
    | 20000   | 200  | 100   | string |
    | 50000   | 500  | 100   | string |
    | 100000  | 2000 | 100   | string |