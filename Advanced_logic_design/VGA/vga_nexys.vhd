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
    signal pxl_init, pxl_tc : std_logic := '0';
    signal ver_init, hor_init : std_logic := '0';
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
        reset   => CPU_RESETN,
        count_init => pxl_init,
        count_enable => '1',
        tc => pxl_tc
    );
    hor_counter : entity work.counter(rtl)
    generic map (
        counter_size => 10
    )
    port map (
        clk => CLK100MHZ,
        reset   => CPU_RESETN,
        count_init => hor_init,
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
        reset   => CPU_RESETN,
        count_init => ver_init,
        count_enable => ver_en,
        count => ver_cnt,
        tc => open
    );
    
    process (CLK100MHZ, CPU_RESETN)
        variable displayable : std_logic := '0';
        variable color : std_logic_vector(3 downto 0) := (others => '0') ;
    begin
        if CPU_RESETN = '0' then
            hor_en <= '0';
            ver_en <= '0';
            pxl_init <= '1';
            VGA_R <= (others => '0');
            VGA_G <= (others => '0');
            VGA_B <= (others => '0');
            VGA_HS <= '0';
            VGA_VS <= '0';
        elsif rising_edge(CLK100MHZ) then
            pxl_init <= '0';
            if pxl_tc = '1' then
                hor_en <= '1';
                -- Vertical sync pulse
                if ver_cnt = 0 then
                    VGA_VS <= '1';
                    ver_init <= '0';
                -- Vertical front porch
                elsif ver_cnt >= 2 and ver_cnt < 35 then
                    VGA_VS <= '0';
                -- Vertical display time
                elsif ver_cnt >= 35 and ver_cnt < 515 then
                    displayable := '1';
                -- Vertical back porch
                elsif ver_cnt >= 515 and ver_cnt < 525 then
                    displayable := '0';
                end if;
                -- Horizontal sync pulse
                if hor_cnt = 0 then
                    VGA_HS <= '1';
                    hor_init <= '0';
                -- Horizontal front porch
                elsif hor_cnt >= 96 and hor_cnt < 144 then
                    VGA_HS <= '0';
                -- Horizontal display time
                elsif hor_cnt >= 144 and hor_cnt < 784 and displayable = '1' then
                    VGA_R <= (others => '0');
                    VGA_G <= (others => '0');
                    VGA_B <= (others => '0');
                    if ver_cnt < 102 then
                        VGA_R <= "1101";
                        VGA_G <= "0011";
                        VGA_B <= "0010";
                    elsif ver_cnt < 168 then
                        VGA_R <= "1111";
                        VGA_G <= "1010";
                        VGA_B <= "0100";
                    elsif ver_cnt < 235 then
                        VGA_R <= "1111";
                        VGA_G <= "1111";
                        VGA_B <= "0101";
                    elsif ver_cnt < 302 then
                        VGA_R <= "0100";
                        VGA_G <= "1000";
                        VGA_B <= "0011";
                    elsif ver_cnt < 368 then
                        VGA_R <= "0010";
                        VGA_G <= "0101";
                        VGA_B <= "1111";
                    else
                        VGA_R <= "0111";
                        VGA_G <= "0010";
                        VGA_B <= "1000";
                    end if;
                -- Horizontal back porch
                elsif hor_cnt < 800 then
                    VGA_R <= (others => '0');
                    VGA_G <= (others => '0');
                    VGA_B <= (others => '0');
                elsif hor_cnt = 800 then
                    hor_init <= '1';
                    ver_en <= '1';
                    if ver_cnt = 525 then
                        ver_init <= '1';
                    end if;
                end if;
            else
                ver_en <= '0';
                hor_en <= '0';
            end if;
        end if;
    end process;
end architecture;