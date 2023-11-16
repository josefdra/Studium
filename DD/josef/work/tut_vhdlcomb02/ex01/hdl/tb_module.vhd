library ieee;
use ieee.std_logic_1164.all;

entity TB_MODULE is
end TB_MODULE;

architecture TESTBENCH of TB_MODULE is
  constant tbase: time:=10 ns;
  constant tcheck: time:=1 ns;
  component MODULE
    port(a, b: in STD_LOGIC_VECTOR(31 downto 0); 
      ALUControl: in STD_LOGIC_VECTOR(1 downto 0); 
      Result:  out STD_LOGIC_VECTOR(31 downto 0); 
      ALUFlags: out STD_LOGIC_VECTOR(2 downto 0)
    ); 
  end component;
  signal TB_a, TB_b: std_logic_vector (31 downto 0);
  signal TB_ALUControl: std_logic_vector (1 downto 0); --00 -> A+B, 01->A-B, 10-> AND, 11->OR
  signal TB_Result: std_logic_vector (31 downto 0);
  signal TB_ALUFlags: std_logic_vector (2 downto 0); --(neg & zero & carry)
  signal expTB_Result: std_logic_vector (31 downto 0);
  signal expTB_ALUFlags: std_logic_vector (2 downto 0);
begin
  DUT: MODULE port map(TB_a, TB_b, TB_ALUControl, TB_Result, TB_ALUFlags);

  TB_a<=x"00000001", x"7fffffff" after 1*tbase, x"ffffffff" after 2*tbase,
        x"00000003" after 3*tbase, x"00000004" after 4*tbase, x"00000005" after 5*tbase,
        x"ffffffff" after 6*tbase, x"fffffffe" after 7*tbase, 
        x"33333333" after 8*tbase, x"55555555" after 9*tbase, 
        x"22222222" after 10*tbase, x"55555555" after 11*tbase; 
  TB_b<=x"00000001", x"00000002" after 1*tbase, x"00000003" after 2*tbase,
        x"00000002" after 3*tbase, x"00000004" after 4*tbase,  x"00000006" after 5*tbase,
        x"fffffffe" after 6*tbase, x"ffffffff" after 7*tbase, 
        x"11111111" after 8*tbase, x"aaaaaaaa" after 9*tbase, 
        x"11111111" after 10*tbase, x"aaaaaaaa" after 11*tbase;
  TB_ALUControl<="00", "00" after 1*tbase, "00" after 2*tbase,
              "01" after 3*tbase, "01" after 4*tbase, "01" after 5*tbase,
              "01" after 6*tbase, "01" after 7*tbase,
              "10" after 8*tbase, "10" after 9*tbase, 
              "11" after 10*tbase,"11" after 11*tbase;

  expTB_Result<=x"00000002", x"80000001" after 1*tbase, x"00000002" after 2*tbase,
              x"00000001" after 3*tbase, x"00000000" after 4*tbase, x"ffffffff" after 5*tbase,
              x"00000001" after 6*tbase, x"ffffffff" after 7*tbase, 
              x"11111111" after 8*tbase, x"00000000" after 9*tbase,
              x"33333333" after 10*tbase,x"ffffffff" after 11*tbase; 
  expTB_ALUFlags<="000", "100" after 1*tbase, "001" after 2*tbase,
              "001" after 3*tbase, "011" after 4*tbase, "100" after 5*tbase,
              "001" after 6*tbase, "100" after 7*tbase, 
              "000" after 8*tbase, "010" after 9*tbase, 
              "000" after 10*tbase,"100" after 11*tbase; 

end TESTBENCH;
