library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity master_nexys_tb is
end entity;

architecture rtl of master_nexys_tb is
  signal CLK100MHZ : std_logic := '0';
  signal CPU_RESETN : std_logic := '0'; -- Reset
  signal VGA_R, VGA_G, VGA_B : std_logic_vector(3 downto 0) := (others => '0');
  signal VGA_HS, VGA_VS : std_logic := '0';
begin
  vga_nexys_0: entity work.vga_nexys
  port map (
    CLK100MHZ => CLK100MHZ,
    CPU_RESETN => CPU_RESETN,
    VGA_R => VGA_R,
    VGA_G => VGA_G,
    VGA_B => VGA_B,
    VGA_HS => VGA_HS,
    VGA_VS => VGA_VS
  );

  CLK100MHZ <= not(CLK100MHZ) after 5 ns;
  process begin
    CPU_RESETN <= '0';
    wait for 10 us;
    CPU_RESETN <= '1';
    wait;
  end process;
end architecture;