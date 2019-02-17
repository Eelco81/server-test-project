
Feature: Command Line Client

  Scenario Outline: Calling the echo endpoint
    When sending a GET request on <path> using the command line client
    Then the received response body has json element <body>
    
  Examples: Binary messages
    | path            | body                      |
    | /api/version    | { version : 0.0.1 }       |
    | /api/version    | { application : Server }  |
    | /api/time       | { unit : ms }             |
  
  Scenario:
    When calling help on the command line client
    Then the received response body equals
      """
      -----------------------------------
       name    :    Client
       version :    0.0.1
      -----------------------------------
      -loglevel -l 
          default : NONE
          type    : optional
      -help -h 
          default : 
          type    : optional
      -version -v 
          default : 
          type    : optional
      -ip -i 
          default : 127.0.0.1
          type    : optional
      -port -p 
          default : 1703
          type    : optional
      -method 
          default : GET
          type    : mandatory
      -path 
          default : /
          type    : mandatory
      -----------------------------------
      
      
      """
      
  Scenario:
    When calling version on the command line client
    Then the received response body equals
      """
      Client : 0.0.1
      
      """