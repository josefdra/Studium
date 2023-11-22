library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(x: in  STD_LOGIC_VECTOR(3 downto 0);
         y: in  STD_LOGIC_VECTOR(3 downto 0);         
         sum: out STD_LOGIC_VECTOR(3 downto 0);
         cout: out STD_LOGIC
    );
  end component;
  signal TB_x, TB_y, TB_sum, expTB_sum : std_logic_vector (3 downto 0);
  signal TB_cout, expTB_cout: std_logic;
begin
  DUT: MODULE port map(x=>TB_x, y=>TB_y, sum=>TB_sum, cout=>TB_cout);

  TB_x<="0000", "0001" after 1*tbase, "1011" after 2*tbase, "0010" after 3*tbase;

  TB_y<="0000", "0001" after 1*tbase, "0110" after 2*tbase, "0011" after 3*tbase;

  expTB_sum<="0000", "0010" after 1*tbase, "0001" after 2*tbase, "0101" after 3*tbase;

  expTB_cout<='0', '0' after 1*tbase, '1' after 2*tbase, '0' after 3*tbase;

end TESTBENCH;
