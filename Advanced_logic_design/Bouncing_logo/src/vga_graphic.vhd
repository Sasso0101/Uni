library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_graphic is
    port (
        CLK100MHZ : in std_logic;
        CPU_RESETN : in std_logic; -- Reset
        VGA_R, VGA_G, VGA_B : out std_logic_vector(3 downto 0);
        VGA_HS, VGA_VS : out std_logic; -- Active negative
        BTNC, BTNU, BTNL, BTNR, BTND : in std_logic; -- Buttons
        LED : out  STD_LOGIC_VECTOR (15 downto 0) -- LEDs
    );
end vga_graphic;

architecture rtl of vga_graphic is
    signal sx, sy : unsigned(10 downto 0); -- Screen x and y positions
    signal de : std_logic;
    signal pxl_clk : std_logic;

    constant WIDTH : natural := 1024;
    constant HEIGHT : natural := 768;
    constant IM_WIDTH : natural := 150;
    constant IM_HEIGHT : natural := 100;

    signal imx, imy : unsigned(10 downto 0);
    signal is_square, new_frame : std_logic := '0';
    signal offset : unsigned(5 downto 0) := (others => '0') ;

    signal rom_addr : std_logic_vector(12 downto 0) := (others => '0');
    signal rom_out : std_logic_vector(35 downto 0);

    COMPONENT blk_mem_gen_0
    PORT (
        clka : IN STD_LOGIC;
        addra : IN STD_LOGIC_VECTOR(12 DOWNTO 0);
        douta : OUT STD_LOGIC_VECTOR(35 DOWNTO 0) 
    );
    END COMPONENT;
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
    vga_driver_0 : entity work.vga_driver
    port map (
        pxl_clk => pxl_clk,
        res => CPU_RESETN,
        VGA_HS => VGA_HS,
        VGA_VS => VGA_VS,
        VGA_X => sx,
        VGA_Y => sy,
        de => de
    );
    rom : blk_mem_gen_0
    PORT MAP (
        clka => pxl_clk,
        addra => rom_addr,
        douta => rom_out
    );
    im_controller_0 : entity work.im_controller
    generic map (
        IM_WIDTH => IM_WIDTH,
        IM_HEIGHT => IM_HEIGHT,
        DISP_WIDTH => WIDTH,
        DISP_HEIGHT => HEIGHT
    )
    port map (
        clk => pxl_clk,
        res => CPU_RESETN,
        imx => imx, imy => imy,
        frame_end => new_frame,
        BTNC => BTNC, BTNU => BTNU, BTNL => BTNL, BTNR => BTNR, BTND => BTND,
        LED => LED
    );
    -- 25.2MHz clock
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

    is_square <= '1' when sx >= imx and sx < imx + IM_WIDTH and sy >= imy and sy < imy + IM_HEIGHT else '0';
    new_frame <= '1' when sy = HEIGHT and sx = 0 else '0';

    process (pxl_clk, CPU_RESETN) begin
        if CPU_RESETN = '0' then
            rom_addr <= (others => '0');
            offset <= (others => '0');
        elsif rising_edge(pxl_clk) then
            if new_frame then
                rom_addr <= (others => '0');
            end if;
            if is_square then
                offset <= offset + 12;
                -- ROM has 2 clock cycle delay, so increment address 2 clock cycles early
                if offset = 0 then
                    rom_addr <= std_logic_vector(unsigned(rom_addr) + 1);
                end if;
                if offset = 24 then
                    offset <= (others => '0') ;
                end if;
            end if;
        end if;
    end process;

    process (all) begin
        if not(de) then
            VGA_R <= x"0";
            VGA_G <= x"0";
            VGA_B <= x"0";
        elsif is_square then
            -- Read from ROM data
            VGA_R <= rom_out(to_integer(unsigned(11 + offset)) downto to_integer(unsigned(8 + offset)));
            VGA_G <= rom_out(to_integer(unsigned(7 + offset)) downto to_integer(unsigned(4 + offset)));
            VGA_B <= rom_out(to_integer(unsigned(3 + offset)) downto to_integer(unsigned(0 + offset)));
        else
            VGA_R <= x"2";
            VGA_G <= x"2";
            VGA_B <= x"C";
        end if;
    end process;
end architecture;