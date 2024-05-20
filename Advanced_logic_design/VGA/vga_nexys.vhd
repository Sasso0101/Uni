library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_nexys is
    port (
        CLK100MHZ : in std_logic;
        CPU_RESETN : in std_logic; -- Reset
        VGA_R, VGA_G, VGA_B : out std_logic_vector(3 downto 0);
        VGA_HS, VGA_VS : out std_logic
    );
end vga_nexys;

architecture rtl of vga_nexys is
    signal pxl_res, pxl_tc : std_logic := '0';
    signal ver_res, hor_res : std_logic := '0';
    signal ver_en, hor_en : std_logic := '0';
    signal ver_cnt, hor_cnt : unsigned (9 downto 0);
    signal res: std_logic := '1';
begin
    pxl_clk : entity work.counter(rtl)
    generic map (
        counter_size => 2
    )
    port map (
        clk => CLK100MHZ,
        reset   => pxl_res,
        count_enable => '1',
        tc => pxl_tc
    );
    hor_counter : entity work.counter(rtl)
    generic map (
        counter_size => 10
    )
    port map (
        clk => CLK100MHZ,
        reset   => hor_res,
        count_enable => hor_en,
        count => hor_cnt,
        tc => open
    );
    ver_counter : entity work.counter(rtl) 
    generic map (
        counter_size => 10
    )
    port map (
        clk => CLK100MHZ,
        reset   => ver_res,
        count_enable => ver_en,
        count => ver_cnt,
        tc => open
    );
    
    process (CLK100MHZ, CPU_RESETN)
    begin
        if CPU_RESETN = '0' then
            hor_res <= '0';
            ver_res <= '0';
            hor_en <= '0';
            ver_en <= '0';
            pxl_res <= '0';
            VGA_R <= (others => '0');
            VGA_G <= (others => '0');
            VGA_B <= (others => '0');
            VGA_HS <= '0';
            VGA_VS <= '0';
        elsif rising_edge(CLK100MHZ) then
            pxl_res <= '1';
            ver_en <= '0';
            if pxl_tc = '1' then
                hor_en <= '1';
                if ver_cnt = 0 then
                    VGA_VS <= '1';
                    ver_res <= '1';
                elsif ver_cnt = 2 then
                    VGA_VS <= '0';
                elsif ver_cnt = 525 then
                    ver_res <= '0';
                end if;
                if hor_cnt = 0 then
                    VGA_HS <= '1';
                    hor_res <= '1';
                elsif hor_cnt >= 97 and hor_cnt < 145 then
                    VGA_HS <= '0';
                elsif hor_cnt >= 145 and hor_cnt < 785 then
                    VGA_HS <= '0';
                    VGA_R <= (others => '1') ;
                elsif hor_cnt < 800 then
                    VGA_R <= (others => '0') ;
                elsif hor_cnt = 800 then
                    hor_res <= '0';
                    ver_en <= '1';
                end if;
            else
                hor_en <= '0';
            end if;
        end if;
    end process;
end architecture;