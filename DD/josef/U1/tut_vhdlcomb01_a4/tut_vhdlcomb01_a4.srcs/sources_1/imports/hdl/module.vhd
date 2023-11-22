library ieee;
use ieee.std_logic_1164.all;

entity MODULE is
  port(a: in  STD_LOGIC_VECTOR(1 downto 0);
       y: out STD_LOGIC_VECTOR(3 downto 0)
  );
end MODULE;

architecture BEHAV of MODULE is
begin
  DECODER: process(a)
  begin
    case a is
      when "00" => y <= "0001";
      when "01" => y <= "0010";
      when "10" => y <= "0100";
      when "11" => y <= "1000";
      when others => y <= "0000";
    end case;
  end process DECODER;
end BEHAV;