
Feature: Simulation API

  Scenario: Starting and stopping simulations
   Given the event service is connected
    When simulation "basic" is started
    Then the simulation is running
    Then event "sim-started" is received
    Then event "sim-sampler-0" is received
    Then event "sim-sampler-1" is received
    When the simulation is stopped
    Then the simulation is not running
    Then event "sim-stopped" is received
    Then the event service can be closed

  Scenario: Starting a running simulation
    When simulation "basic" is started
    When simulation "basic" is attempted to be started
    When simulation "heat" is attempted to be started
    When simulation "wave" is attempted to be started
    Then the simulation is running
    When the simulation is stopped
    Then the simulation is not running
  
  Scenario: Stopping a stopped simulation
    When the simulation is attempted to be stopped
    When the simulation is attempted to be stopped
    Then the simulation is not running
    
  Scenario Outline: Retrieving all ports
    When simulation "basic" is started
    Then the simulation is running
    Then there are <ammount> ports of type "<type>" open
    When the simulation is stopped
    Then the simulation is not running
    
  Examples: Types
    | type       | ammount |
    | in         | 4       |
    | out        | 5       |
    | par        | 12      |
  
  Scenario Outline: Parameter manipulation
    When simulation "basic" is started
    Then the simulation is running
    Then port <path> has value <startval>
    When port <path> is set to value <endval>
    Then port <path> has value <endval>
    When the simulation is stopped
    Then the simulation is not running
    
  Examples: Parameters
    | path            | startval | endval |
    | source.par.type | 3.0      | 0.0    |
    | pid.par.enable  | 1.0      | 0.0    |
    | pid.par.kp      | 1.0      | 2.0    |
  