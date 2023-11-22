library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(a: in  STD_LOGIC_VECTOR(1 downto 0);
         y: out STD_LOGIC_VECTOR(3 downto 0)
  );
  end component;
  signal TB_a : std_logic_vector (1 downto 0);
  signal TB_y, expTB_y: std_logic_vector (3 downto 0);
begin
  DUT: MODULE port map(a=>TB_a, y=>TB_y);

  TB_a<="00", "01" after 1*tbase, "10" after 2*tbase, "11" after 3*tbase; 

  expTB_Y<="0001", "0010" after 1*tbase, "0100" after 2*tbase, "1000" after 3*tbase;         

end TESTBENCH;
