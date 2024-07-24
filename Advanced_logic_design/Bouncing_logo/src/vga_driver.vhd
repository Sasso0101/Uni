library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_driver is
    port (
        pxl_clk : in std_logic;
        res : in std_logic; -- Reset
        VGA_HS, VGA_VS : out std_logic; -- Active negative
        VGA_X, VGA_Y : out unsigned(10 downto 0); -- Screen x and y positions
        de : out std_logic -- Data enable (low during blanking interval)
    );
end vga_driver;

architecture rtl of vga_driver is
    constant HA_END : natural := 1024; -- End of active pixels
    constant HS_STA : natural := HA_END + 24; -- Duration of front porch
    constant HS_END : natural := HS_STA + 136; -- Duration of sync
    constant LINE_END : natural := 1344; -- Last pixel in line

    constant VA_END : natural := 768; -- End of active pixels
    constant VS_STA : natural := VA_END + 3; -- Start of sync after front porch
    constant VS_END : natural := VS_STA + 6; -- End of sync
    constant SCREEN_END : natural := 806; -- Last pixel in screen

    signal sx, sy : unsigned(10 downto 0) := (others => '0'); -- Screen x and y positions
begin
    VGA_X <= sx;
    VGA_Y <= sy;
    VGA_HS <= '0' when sx >= HS_STA and sx < HS_END else '1';
    VGA_VS <= '0' when sy >= VS_STA and sy < VS_END else '1';
    de <= '1' when sx < HA_END and sy < VA_END else '0';
    
    process (pxl_clk, res) begin
        if res = '0' then
            sx <= (others => '0');
            sy <= (others => '0');
        elsif rising_edge(pxl_clk) then
            if sx = LINE_END - 1 then
                sx <= (others => '0');
                sy <= (others => '0')  when sy = SCREEN_END - 1 else sy + 1;
            else
                sx <= sx + 1;
            end if;
        end if;
    end process;
end architecture;