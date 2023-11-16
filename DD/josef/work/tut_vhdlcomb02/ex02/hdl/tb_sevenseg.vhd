library ieee;
use ieee.std_logic_1164.all;

entity TB_SEVENSEG is
end TB_SEVENSEG;

architecture TESTBENCH of TB_SEVENSEG is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component sevenseg is
    port(bin: in  STD_LOGIC_VECTOR(3 downto 0);
      segments: out STD_LOGIC_VECTOR(6 downto 0)
    );
  end component;
  signal TB_bin: std_logic_vector (3 downto 0);
  signal TB_segments: std_logic_vector (6 downto 0);
  signal expTB_segments: std_logic_vector (6 downto 0);
begin
  DUT: SEVENSEG port map(TB_bin, TB_segments);

  TB_bin<=x"0", x"1" after 1*tbase, x"2" after 2*tbase, x"3" after 3*tbase, 
     x"4" after  4*tbase, x"5" after  5*tbase, x"6" after  6*tbase, x"7" after  7*tbase,  
     x"8" after  8*tbase, x"9" after  9*tbase, x"A" after 10*tbase, x"B" after 11*tbase,
     x"C" after 12*tbase, x"D" after 13*tbase, x"E" after 14*tbase, x"F" after 15*tbase;
 
  expTB_segments<="1111110", "0110000" after 1*tbase, "1101101" after 2*tbase, 
     "1111001" after  3*tbase, "0110011" after  4*tbase, "1011011" after  5*tbase, 
     "1011111" after  6*tbase, "1110000" after  7*tbase, "1111111" after  8*tbase, 
     "1110011" after  9*tbase, "1110111" after 10*tbase, "0011111" after 11*tbase,
     "0001101" after 12*tbase, "0111101" after 13*tbase, "1001111" after 14*tbase, 
     "1000111" after 15*tbase;

end TESTBENCH;
