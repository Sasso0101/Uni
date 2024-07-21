library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_driver is
    port (
        clk : in std_logic;
        res : in std_logic; -- Reset
        VGA_HS, VGA_VS : out std_logic; -- Active negative
        VGA_X, VGA_Y : out unsigned(9 downto 0); -- Screen x and y positions
        de : out std_logic; -- Data enable (low during blanking interval)
        pxl_clk : out std_logic
    );
end vga_driver;

architecture rtl of vga_driver is
    constant HA_END : natural := 640; -- End of active pixels
    constant HS_STA : natural := HA_END + 18; -- Start of sync after front porch 16
    constant HS_END : natural := HS_STA + 92; -- End of sync 96
    constant LINE_END : natural := 799; -- Last pixel in line

    constant VA_END : natural := 480; -- End of active pixels
    constant VS_STA : natural := VA_END + 10; -- Start of sync after front porch
    constant VS_END : natural := VS_STA + 2; -- End of sync
    constant SCREEN_END : natural := 524; -- Last pixel in screen

    signal sx, sy : unsigned(9 downto 0) := (others => '0'); -- Screen x and y positions
    
    component clk_wiz_0
    port
     (-- Clock in ports
      -- Clock out ports
      clk_out1          : out    std_logic;
      -- Status and control signals
      resetn             : in     std_logic;
      locked            : out    std_logic;
      clk_in1           : in     std_logic
     );
    end component;
begin
    -- 25.2MHz clock
   pxl_clk_wiz : clk_wiz_0
   port map ( 
      -- Clock out ports  
       clk_out1 => pxl_clk,
      -- Status and control signals                
       resetn => res,
       locked => open,
       -- Clock in ports
       clk_in1 => clk
    );

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
            if sx = LINE_END then
                sx <= (others => '0');
                sy <= (others => '0')  when sy = SCREEN_END else sy + 1;
            else
                sx <= sx + 1;
            end if;
        end if;
    end process;
end architecture;