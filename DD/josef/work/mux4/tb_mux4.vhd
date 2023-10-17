entity TB_MUX4 is
end TB_MUX4;

architecture TESTBENCH of TB_MUX4 is
  component MUX4
    port( S: in bit_vector (1 downto 0);
          E: in bit_vector (3 downto 0);
          Y: out bit);
  end component;
  signal TB_S: bit_vector (1 downto 0);
  signal TB_E: bit_vector (3 downto 0);
  signal TB_Y: bit;

begin
  DUT: MUX4 port map(S=>TB_S, E=>TB_E, Y=>TB_Y);

  TB_E<="0101"; -- static input
  TB_S<="00", "01" after 10 ns, "10" after 20 ns, "11" after 30 ns;
end TESTBENCH;