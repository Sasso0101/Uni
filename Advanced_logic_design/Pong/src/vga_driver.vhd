library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.utilities.all;

entity vga_driver is
    port (
        pxl_clk : in std_logic;
        res : in std_logic; -- Reset
        VGA_HS, VGA_VS : out std_logic; -- Active negative
        VGA_POS : out position; -- Screen position
        de : out std_logic; -- Data enable (true when in active zone)
        frame_end : out std_logic -- true for one CLK when a whole frame has been drawn
    );
end vga_driver;

architecture rtl of vga_driver is
    signal s : position; -- Screen position
begin
    VGA_POS <= s;
    VGA_HS <= '0' when s.x >= HS_STA and s.x < HS_END else '1';
    VGA_VS <= '0' when s.y >= VS_STA and s.y < VS_END else '1';
    de <= '1' when s.x < DISP_WIDTH and s.y < DISP_HEIGHT else '0';
    frame_end <= '1' when s.y = DISP_HEIGHT and s.x = 0 else '0';
    
    process (pxl_clk, res) begin
        if res = '0' then
            s.x <= (others => '0');
            s.y <= (others => '0');
        elsif rising_edge(pxl_clk) then
            if s.x = LINE_END - 1 then
                s.x <= (others => '0');
                s.y <= (others => '0')  when s.y = SCREEN_END - 1 else s.y + 1;
            else
                s.x <= s.x + 1;
            end if;
        end if;
    end process;
end architecture;