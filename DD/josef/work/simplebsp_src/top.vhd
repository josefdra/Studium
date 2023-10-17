library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity top is
  port( BTN0: in std_logic;
        BTN1: in std_logic;          
        LED0: out std_logic;
        LED1: out std_logic);        
end top;

architecture Behavioral of top is

begin
LED0 <=BTN0;
LED1 <=BTN1;

end Behavioral;

