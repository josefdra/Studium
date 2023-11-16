library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(a: in STD_LOGIC_VECTOR(3 downto 0);
         shamt: in STD_LOGIC_VECTOR(1 downto 0);
         right_rotated: out STD_LOGIC_VECTOR(3 downto 0); 
         left_rotated: out STD_LOGIC_VECTOR(3 downto 0)       
    );
  end component;
  signal TB_a: std_logic_vector (3 downto 0);
  signal TB_shamt: std_logic_vector (1 downto 0); 
  signal TB_right_rotated: std_logic_vector (3 downto 0);
  signal TB_left_rotated: std_logic_vector (3 downto 0);
  signal expTB_right_rotated: std_logic_vector (3 downto 0);
  signal expTB_left_rotated: std_logic_vector (3 downto 0);
begin
  DUT: MODULE port map(TB_a, TB_shamt, TB_right_rotated, TB_left_rotated);

  TB_a<="0011", "0011" after 1*tbase, "0011" after 2*tbase,
        "0011" after 3*tbase; 
 
  TB_shamt<="00", "01" after 1*tbase, "10" after 2*tbase,
        "11" after 3*tbase; 

  expTB_right_rotated<="0011", "1001" after 1*tbase, "1100" after 2*tbase,
              "0110" after 3*tbase; 
  expTB_left_rotated<="0011", "0110" after 1*tbase, "1100" after 2*tbase,
              "1001" after 3*tbase; 

end TESTBENCH;
