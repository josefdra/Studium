library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(a: in  STD_LOGIC_VECTOR(3 downto 0);
         y: out STD_LOGIC
    );
  end component;
  signal TB_a: std_logic_vector (3 downto 0);
  signal TB_y: std_logic;
begin
  instance_module: MODULE port map(a=>TB_a, y=>TB_y);
  TB_a<="0000", 
        "0001" after 1*tbase,  
        "0010" after 2*tbase,  
        "0011" after 3*tbase,  
        "0100" after 4*tbase,  
        "0101" after 5*tbase,  
        "0110" after 6*tbase,  
        "0111" after 7*tbase,  
        "1000" after 8*tbase,  
        "1001" after 9*tbase,  
        "1010" after 10*tbase,  
        "1011" after 11*tbase,  
        "1100" after 12*tbase,  
        "1101" after 13*tbase,  
        "1110" after 14*tbase,  
        "1111" after 15*tbase;
    TB_y<='0',
          '1' after 1*tbase,
          '0' after 3*tbase,
          '1' after 4*tbase,
          '0' after 5*tbase,
          '1' after 8*tbase,
          '0' after 9*tbase;
end TESTBENCH;
