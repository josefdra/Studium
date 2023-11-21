library IEEE; 
use IEEE.STD_LOGIC_1164.all; 
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all; 

entity module is  
  port(a, b: in STD_LOGIC_VECTOR(31 downto 0); 
    ALUControl: in STD_LOGIC_VECTOR(1 downto 0); --00 -> A+B, 01->A-B, 10-> AND, 11->OR
    Result:  out STD_LOGIC_VECTOR(31 downto 0); 
    ALUFlags: out STD_LOGIC_VECTOR(2 downto 0)  --(neg zero carry)
  ); 
end; 

architecture behave of module is 
  signal lResult: STD_LOGIC_VECTOR(32 downto 0);
--##INSERT YOUR CODE HERE
  signal Carry: STD_LOGIC;


begin 
	DoSmth: process(ALUControl, a, b, lResult) 
	begin
	case ALUControl is
			when "00" => lResult <= ('0' & a) + ('0' & b);
			when "01" => lResult <= ('0' & a) + ('0' & not b) + 1;
			when "10" => lResult <= '0' & (a and b);
			when "11" => lResult <= '0' & (a or b);
			when others => lResult <= (others=>'0');			
		end case;
		
            if lResult(31)='1' then ALUFlags(2)<='1';
              else ALUFlags(2)<='0';
              end if;
            if lResult(31 downto 0)= 0 then ALUFlags(1)<='1';
              else ALUFlags(1) <= '0';
              end if;
            if lResult(32)='1' then ALUFlags(0)<='1';
                Carry <= '1';
              else ALUFlags(0)<='0';
              Carry <= '0';
              end if;
              
    end process DoSmth;
    Result <= lResult(31 downto 0);
end behave;
--##INSERT YOUR CODE HERE END 




