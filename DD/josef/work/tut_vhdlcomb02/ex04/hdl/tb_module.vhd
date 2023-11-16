library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(a: in  STD_LOGIC_VECTOR(3 downto 0);
       y: out STD_LOGIC_VECTOR(7 downto 0)
    );
  end component;
  signal TB_a: std_logic_vector (3 downto 0); 
  signal TB_y: std_logic_vector (7 downto 0);
  signal expTB_y: std_logic_vector (7 downto 0);
begin
  DUT: MODULE port map(TB_a, TB_y);

  TB_a<="0000", "0111" after 1*tbase, "1000" after 2*tbase;
  
  expTB_y<="00000000", "00000111" after 1*tbase, "11111000" after 2*tbase; 

end TESTBENCH;
