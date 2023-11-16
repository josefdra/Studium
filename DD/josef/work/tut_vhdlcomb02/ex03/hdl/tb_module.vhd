library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(btn: in std_logic_vector(1 downto 0);
         segments: out std_logic_vector(6 downto 0)     
    );
  end component;
  signal TB_btn: std_logic_vector (1 downto 0);
  signal TB_segments: std_logic_vector(6 downto 0);
  signal expTB_segments: std_logic_vector(6 downto 0);
begin
  DUT: MODULE port map(TB_btn, TB_segments);

  TB_btn<="00", "01" after 1*tbase, "10" after 2*tbase,
        "11" after 3*tbase; 

  expTB_segments<="1111110", "0110000" after 1*tbase, "1101101" after 2*tbase,
              "1111001" after 3*tbase; 
  

end TESTBENCH;
