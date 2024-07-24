library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_nexys_tb is
end entity;

architecture rtl of vga_nexys_tb is
  signal CLK100MHZ : std_logic := '0';
  signal CPU_RESETN : std_logic := '0'; -- Reset
  signal VGA_R, VGA_G, VGA_B : std_logic_vector(3 downto 0) := (others => '0');
  signal VGA_HS, VGA_VS : std_logic := '0';
  signal BTNC, BTNU, BTNL, BTNR, BTND : std_logic := '0'; -- Buttons
begin
  vga_graphic_0: entity work.vga_graphic
  port map (
    CLK100MHZ => CLK100MHZ,
    CPU_RESETN => CPU_RESETN,
    VGA_R => VGA_R,
    VGA_G => VGA_G,
    VGA_B => VGA_B,
    VGA_HS => VGA_HS,
    VGA_VS => VGA_VS,
    BTNC => BTNC, BTNU => BTNU, BTNL => BTNL, BTNR => BTNR, BTND => BTND
  );

  CLK100MHZ <= not(CLK100MHZ) after 5 ns;
  process begin
    CPU_RESETN <= '0';
    wait for 10 ns;
    CPU_RESETN <= '1';
    wait for 10 us;
    BTND <= '1';
    BTNC <= '1';
    wait for 200 us;
    BTNC <= '0';
    BTND <= '0';
    wait;
  end process;
end architecture;