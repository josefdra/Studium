library IEEE; 
use IEEE.STD_LOGIC_1164.all;

entity module is
  port(btn: in std_logic_vector(1 downto 0);
       segments: out std_logic_vector(6 downto 0)     
  );
end;

architecture arch of module is
component sevenseg is
port(bin: in std_logic_vector(3 downto 0); --complev1
     segments: out std_logic_vector(6 downto 0));
end component sevenseg;
signal tmpBin: std_logic_vector(3 downto 0);
begin
with btn select
    tmpBin <= "0000" when "00",
              "0001" when "01",
              "0010" when "10",
              "0011" when "11",
              "ZZZZ" when others;
sevenseg_inst: sevenseg port map(bin => tmpBin, segments => segments);
end;
