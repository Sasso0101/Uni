library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity seven_segment_driver_tb is
end entity;

architecture rtl of seven_segment_driver_tb is
    signal clk, res : std_logic := '0';
    signal digit1, digit2 : std_logic_vector( 6 downto 0 ); 
    signal CA, CB, CC, CD, CE, CF, CG, DP : std_logic;
    signal AN : std_logic_vector( 7 downto 0 );
begin
    seven_segment : entity work.seven_segment_driver
    port map (
        clk => clk,
        res => res,
        digit1 => digit1, digit2 => digit2,
        CA => CA, CB => CB, CC => CC, CD => CD, CE => CE, CF => CF, CG => CG, DP => DP,
        AN => AN
    );

  clk <= not(clk) after 5 ns;
  process begin
    res <= '0';
    wait for 10 ns;
    res <= '1';
    wait for 10 us;
    digit1 <= "0001011";
    digit2 <= "0001101";
    wait for 20 ms;
    wait;
  end process;
end architecture;