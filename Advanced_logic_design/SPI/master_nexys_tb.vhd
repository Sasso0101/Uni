library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity master_nexys_tb is
end entity;

architecture rtl of master_nexys_tb is
  signal CLK100MHZ : std_logic := '0';
  signal SW : std_logic_vector(15 downto 0) := (others => '0') ; -- Switches
  signal LED : std_logic_vector(15 downto 0) := (others => '0') ; -- Leds
  signal CPU_RESETN : std_logic := '0'; -- Reset
  signal BTNC, BTNL, BTND, BTNR : std_logic := '0'; -- Buttons
  signal MOSI, MISO, SCK, SS : std_logic := '0';
begin
  spi_master_0: entity work.master_nexys
  port map (
    CLK100MHZ => CLK100MHZ,
    SW => SW,
    LED => LED,
    CPU_RESETN => CPU_RESETN,
    BTNC => BTNC,
    BTNL => BTNL,
    BTND => BTND,
    BTNR => BTNR,
    JA1 => MOSI,
    JA2 => SCK,
    JA8 => SS,
    JA7 => MISO,
    JA3 => MOSI,
    JA4 => SCK,
    JA10 => SS,
    JA9 => MISO
  );

  CLK100MHZ <= not(CLK100MHZ) after 100 ns;
  process begin
    CPU_RESETN <= '0';
    wait for 10 us;
    CPU_RESETN <= '1';
    SW <= "0000000000000011";
    BTNR <= '1';
    wait for 1 ms;
    BTNR <= '0';
    BTNC <= '1';
    wait for 1 ms;
    BTNC <= '0';
    wait for 1 ms;
    BTNC <= '1';
    wait for 1 ms;
    BTNC <= '0';
    wait;
  end process;
end architecture;