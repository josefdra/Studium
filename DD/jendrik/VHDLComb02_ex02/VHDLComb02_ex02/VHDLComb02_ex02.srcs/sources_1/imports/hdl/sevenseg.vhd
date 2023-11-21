library IEEE; 
use IEEE.STD_LOGIC_1164.all;
entity sevenseg is
  port(bin: in  STD_LOGIC_VECTOR(3 downto 0);
    segments: out STD_LOGIC_VECTOR(6 downto 0) -- SA SB SC SD SE SF SG
  );
end;

architecture arch of sevenseg is
begin
DECODE: process(bin)
    begin
    case bin is
        when "0000" => segments <= "1111110";
        when "0001" => segments <= "0110000";
        when "0010" => segments <= "1101101";
        when "0011" => segments <= "1111001";
        when "0100" => segments <= "0110011";
        when "0101" => segments <= "1011011";
        when "0110" => segments <= "1011111";
        when "0111" => segments <= "1110000";
        when "1000" => segments <= "1111111";
        when "1001" => segments <= "1110011";
        when "1010" => segments <= "1110111"; -- a
        when "1011" => segments <= "0011111"; -- b
        when "1100" => segments <= "0001101"; -- c
        when "1101" => segments <= "0111101"; -- d
        when "1110" => segments <= "1001111"; -- e
        when "1111" => segments <= "1000111"; -- f 
        when others => segments <= "0111110"; -- u (undefined)
        end case;
    end process DECODE;
end arch;