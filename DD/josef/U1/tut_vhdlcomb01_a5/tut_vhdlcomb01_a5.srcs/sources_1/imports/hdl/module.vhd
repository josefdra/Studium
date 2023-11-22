library ieee;
use ieee.std_logic_1164.all;

entity MODULE is
    port(a: in  STD_LOGIC_VECTOR(3 downto 0);
         y: out STD_LOGIC
    );
end MODULE;

architecture BEHAV of MODULE is
begin
   y <= a(3) xor a(1) xor a(1) xor a(0); 
end BEHAV;

