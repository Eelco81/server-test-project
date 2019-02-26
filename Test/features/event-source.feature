
@fixture.test-app
Feature: Event Source (SSE)

  Scenario Outline: Broadcasts
   Given the event source is listening
    When running a <type> data stream at <step> ms of <count> packets of <size> bytes
    Then the stream received <count> packets (<type>) of size <size> are received at interval <step> ms 
    Then the event source can be closed
    
  Examples:
    | step    | size | count | type   |
    | 20000   | 50   | 10   | string |