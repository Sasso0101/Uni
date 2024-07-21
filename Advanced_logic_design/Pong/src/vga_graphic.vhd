library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.utilities.all;

entity vga_graphic is
    port (
        CLK100MHZ : in std_logic;
        CPU_RESETN : in std_logic; -- Reset
        VGA_R, VGA_G, VGA_B : out std_logic_vector(3 downto 0);
        VGA_HS, VGA_VS : out std_logic; -- Active negative
        BTNU, BTNL, BTNR, BTND : in std_logic; -- Buttons
        CA, CB, CC, CD, CE, CF, CG, DP : out std_logic;
        AN : out std_logic_vector( 7 downto 0 )
    );
end vga_graphic;

architecture rtl of vga_graphic is
    signal s : position; -- Screen x and y positions
    signal pxl_clk : std_logic;
    signal frame_end, de : std_logic;

    signal is_bar, is_ball : std_logic;

    signal ball : position;
    signal p1, p2 : unsigned(POSITION_BITS downto 0);

    component clk_wiz_0
        port (
            -- Clock out ports
            clk_out1 : out std_logic;
            -- Status and control signals
            resetn : in std_logic;
            locked : out std_logic;
            clk_in1 : in std_logic
        );
    end component;
begin
    vga_driver_0 : entity work.vga_driver
    port map (
        pxl_clk => pxl_clk,
        res => CPU_RESETN,
        VGA_HS => VGA_HS,
        VGA_VS => VGA_VS,
        VGA_POS => s,
        de => de,
        frame_end => frame_end
    );
    game_0 : entity work.game
    port map (
        clk => pxl_clk,
        res => CPU_RESETN,
        frame_end => frame_end,
        ball => ball,
        p1 => p1, p2 => p2,
        CA => CA, CB => CB, CC => CC, CD => CD, CE => CE, CF => CF, CG => CG, DP => DP,
        AN => AN,
        BTNU => BTNU, BTNL => BTNL, BTNR => BTNR, BTND => BTND
    );
    -- Pixel clock
   pxl_clk_wiz : clk_wiz_0
   port map ( 
      -- Clock out ports  
       clk_out1 => pxl_clk,
      -- Status and control signals                
       resetn => CPU_RESETN,
       locked => open,
       -- Clock in ports
       clk_in1 => CLK100MHZ
    );

    is_bar <= '1' when (s.x >= BAR_DISTANCE-1 and s.x < BAR_DISTANCE+BAR_WIDTH and s.y >= p1 and s.y <= p1+BAR_HEIGHT) or (s.x >= DISP_WIDTH-BAR_DISTANCE-BAR_WIDTH-1 and s.x < DISP_WIDTH-BAR_DISTANCE and s.y >= p2 and s.y <= p2+BAR_HEIGHT) else '0';
    is_ball <= '1' when s.x >= ball.x and s.x <= ball.x+BALL_LENGTH and s.y >= ball.y and s.y <= ball.y+BALL_LENGTH else '0';

    process (all) begin
        if not(de) then
            VGA_R <= x"0";
            VGA_G <= x"0";
            VGA_B <= x"0";
        elsif is_bar then
            VGA_R <= x"F";
            VGA_G <= x"0";
            VGA_B <= x"0";
        elsif is_ball then
            VGA_R <= x"F";
            VGA_G <= x"F";
            VGA_B <= x"F";
        else
            VGA_R <= x"0";
            VGA_G <= x"0";
            VGA_B <= x"0";
        end if;
    end process;
end architecture;