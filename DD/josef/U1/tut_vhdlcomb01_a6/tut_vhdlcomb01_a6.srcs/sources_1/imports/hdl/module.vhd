library ieee;
use ieee.std_logic_1164.all;

entity MODULE is
    port(x: in  STD_LOGIC_VECTOR(3 downto 0);
         y: in  STD_LOGIC_VECTOR(3 downto 0);         
         sum: out STD_LOGIC_VECTOR(3 downto 0);
         cout: out STD_LOGIC
    );
end MODULE;

architecture ARCH of MODULE is
signal c0, c1, c2, c3: std_logic;
component FULLADD
  port(E0, E1, CIN: in std_logic;
       SUM, COUT: out std_logic);
end component;
begin
  A1: FULLADD port map(x(0), y(0), '0', sum(0), c0);
  A2: FULLADD port map(x(1), y(1), c0, sum(1), c1);
  A3: FULLADD port map(x(2), y(2), c1, sum(2), c2);
  A4: FULLADD port map(x(3), y(3), c2, sum(3), c3);
  cout<=c3;
end ARCH;

