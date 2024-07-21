library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity debouncer_tb is
end entity;

architecture rtl of debouncer_tb is
  signal clk, res, bouncy, pulse : std_logic := '0';
begin
  debouncer_0 : entity work.debouncer
    port map
    (
      clk    => clk,
      res    => res,
      bouncy => bouncy,
      pulse  => pulse
    );

  clk <= not(clk) after 5 ns;
  process begin
    res <= '0';
    wait for 10 ns;
    res <= '1';
    wait for 10 us;
    bouncy <= '1';
    wait for 20 ms;
    bouncy <= '0';
    wait;
  end process;
end architecture;