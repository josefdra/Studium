library ieee;
use ieee.std_logic_1164.all;

entity HALFADD is
  port( E0, E1: in std_logic;
        SUM, COUT: out std_logic);
end HALFADD;

architecture BEHAV of HALFADD is
begin
  SUM <= E0 xor E1;
  COUT <= E0 and E1;
end BEHAV;
