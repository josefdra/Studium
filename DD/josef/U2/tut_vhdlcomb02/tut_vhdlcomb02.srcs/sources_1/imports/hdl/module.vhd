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
--##INSERT YOUR CODE HERE 

begin 
 
--##INSERT YOUR CODE HERE END 
end;
