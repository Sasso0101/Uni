library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_graphic is
    port (
        CLK100MHZ : in std_logic;
        CPU_RESETN : in std_logic; -- Reset
        VGA_R, VGA_G, VGA_B : out std_logic_vector(3 downto 0);
        VGA_HS, VGA_VS : out std_logic -- Active negative
    );
end vga_graphic;

architecture rtl of vga_graphic is
    signal sx, sy : unsigned(9 downto 0); -- Screen x and y positions
    signal de : std_logic;

    constant FRAME_NUM : natural := 3;

    signal new_frame : std_logic;
    signal frame_cnt : unsigned(4 downto 0);
    signal color_level : unsigned(3 downto 0);
    signal pxl_clk : std_logic;
begin
    vga_driver_0 : entity work.vga_driver(rtl)
    port map (
        clk => CLK100MHZ,
        res => CPU_RESETN,
        VGA_HS => VGA_HS,
        VGA_VS => VGA_VS,
        VGA_X => sx,
        VGA_Y => sy,
        de => de,
        pxl_clk => pxl_clk
    );

    new_frame <= '1' when sy = 480 and sx = 0 else '0';
    
    process (pxl_clk) begin
        if rising_edge(pxl_clk) then
            if new_frame then
                if frame_cnt = FRAME_NUM - 1 then
                    frame_cnt <= ( others => '0' );
                    color_level <= color_level + 1;
                else
                    frame_cnt <= frame_cnt + 1;
                end if;
            end if;
        end if;
    end process;

    process (all) begin
        if not(de) then
            VGA_R <= x"0";
            VGA_G <= x"0";
            VGA_B <= x"0";
        --elsif sx = 0 or sy = 0 or sx = 639 or sy = 479 then
        --    VGA_R <= x"F";
        --    VGA_G <= x"0";
        --    VGA_B <= x"0";
        else
            VGA_R <= std_logic_vector(sx(3 downto 0));
            VGA_G <= std_logic_vector(sy(3 downto 0));
            VGA_B <= std_logic_vector(color_level);
        end if;
    end process;
end architecture;