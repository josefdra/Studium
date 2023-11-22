library ieee;
use ieee.std_logic_1164.all;

entity FULLADD is
  port( E0, E1, CIN: in std_logic;
        SUM, COUT: out std_logic);
end FULLADD;

architecture STRUCT of FULLADD is
signal SUM1, SUM2, COUT1, COUT2: std_logic;
component HALFADD
  port( E0, E1: in std_logic;
        SUM, COUT: out std_logic);
end component;
begin
  U1: HALFADD port map(E0, E1, SUM1, COUT1);
  U2: HALFADD port map(SUM1, CIN, SUM2, COUT2);
  COUT <= COUT1 or COUT2;
  SUM <= SUM2;
end STRUCT;
