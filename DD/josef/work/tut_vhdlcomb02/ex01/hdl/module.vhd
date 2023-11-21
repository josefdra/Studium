library IEEE; 
use IEEE.STD_LOGIC_1164.all; 
use ieee.std_logic_unsigned.all;

entity module is  
  port(a, b: in STD_LOGIC_VECTOR(31 downto 0); 
    ALUControl: in STD_LOGIC_VECTOR(1 downto 0); --00 -> A+B, 01->A-B, 10-> AND, 11->OR
    Result:  out STD_LOGIC_VECTOR(31 downto 0); 
    ALUFlags: out STD_LOGIC_VECTOR(2 downto 0)  --(neg zero carry)
  ); 
end; 

architecture behave of module is 
  signal lResult: STD_LOGIC_VECTOR(31 downto 0);  
  begin 
  CALC: process(a, b, ALUControl)
    begin
    case ALUControl is
      when "00" => lResult <= a + b;
      when "01" => lResult <= a + not b + 1;
      when "10" => lResult <= a and b;
      when "11" => lResult <= a or b;
      when others => lResult <= "0";
    end case;
    if (a + b) < a or (a + b) < b then
      ALUFlags (0) <= '1';
    end if;
    if a + b = 0 and not ((a + b) < a or (a + b) < b) then
      ALUFlags (1) <= '1';
    else
      ALUFlags (1) <= '0';
    end if;
    if a < b and ALUControl = "01" then
      ALUFlags (2) <= '1';
    end if;
    Result <= lResult;
  end process CALC;
end behave;



