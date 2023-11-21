library IEEE; 
use IEEE.STD_LOGIC_1164.all;

entity module is
  port(btn: in std_logic_vector(1 downto 0);
       segments: out std_logic_vector(6 downto 0)     
  );
end;

architecture arch of module is 
component sevenseg
  port(bin: in std_logic_vector(3 downto 0); 
     segments: out std_logic_vector(6 downto 0));
end component;
signal tmpBin: std_logic_vector(3 downto 0);
begin
sevenseg_inst: sevenseg port map(bin => tmpBin, segments => segments);
process(btn)
begin
  case btn is
     when "00" => tmpBin <= "0000";
     when "01" => tmpBin <= "0001";
     when "10" => tmpBin <= "0010";
     when "11" => tmpBin <= "0011";
     when others => tmpBin <= "ZZZZ";
 end case;
end process;
end;
