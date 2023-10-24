-- TestBench Template 

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;

  ENTITY testbench IS
  END testbench;

  ARCHITECTURE behavior OF testbench IS 

  -- Component Declaration
          COMPONENT top is
            port( BTN0: in std_logic;
                  BTN1: in std_logic;          
                  LED0: out std_logic;
                  LED1: out std_logic);        
          END COMPONENT;

          SIGNAL BTN0, BTN1 :  std_logic;
          SIGNAL LED0, LED1: std_logic;
          

  BEGIN

  -- Component Instantiation
          uut: top PORT MAP(
                  BTN0 => BTN0,
                  BTN1 => BTN1,
                  LED0 => LED0,
                  LED1 => LED1
          );


  --  Test Bench Statements
     tb : PROCESS
     BEGIN

        wait for 100 ns; -- wait until global set/reset completes

        -- Add user defined stimulus here
        BTN0<='0', '1' after 20ns, '0' after 40ns;
        BTN1<='0', '1' after 60ns, '0' after 80ns;

        wait; -- will wait forever
     END PROCESS tb;
  --  End Test Bench 

  END;
