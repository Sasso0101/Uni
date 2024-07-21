library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity im_controller is
    generic (
        IM_WIDTH : natural := 100;
        IM_HEIGHT : natural := 100;
        DISP_WIDTH : natural := 640;
        DISP_HEIGHT : natural := 480
    );
    port (
        clk : in std_logic;
        res : in std_logic; -- Reset
        imx, imy: out unsigned(10 downto 0);
        BTNC, BTNU, BTNL, BTNR, BTND : in std_logic; -- Buttons
        LED : out  STD_LOGIC_VECTOR (15 downto 0); -- LEDs
        frame_end : in std_logic
    );
end im_controller;

architecture rtl of im_controller is
    signal pulseC, pulseU, pulseL, pulseR, pulseD : std_logic;
    signal auto : std_logic := '0';
    signal tempx, tempy: signed(10 downto 0) := (others => '0') ;
begin
    btn_c : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTNC,
        pulse => pulseC
    );
    btn_d : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTND,
        pulse => pulseD
    );
    btn_u : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTNU,
        pulse => pulseU
    );
    btn_l : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTNL,
        pulse => pulseL
    );
    btn_r : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTNR,
        pulse => pulseR
    );

    LED(0) <= BTNL;
    LED(1) <= BTNR;
    LED(2) <= BTNU;
    LED(3) <= BTND;
    LED(4) <= BTNC;
    LED(5) <= auto;
    LED(8) <= frame_end;
    LED(9) <= pulseL;
    LED(10) <= pulseR;
    LED(11) <= pulseU;
    LED(12) <= pulseD;
    LED(13) <= pulseC;
    LED(15 downto 14) <= (others => '0');

    process (clk, res) is
        variable dir_x, dir_y : std_logic := '1';
    begin
        LED(6) <= dir_x;
        LED(7) <= dir_y;
        if res = '0' then
            imx <= (others => '0');
            imy <= (others => '0');
            auto <= '0';
            dir_x := '1';
            dir_y := '1';
            tempx <= (others => '0');
            tempy <= (others => '0');
        elsif rising_edge(clk) then
            if pulseC then
                auto <= not auto;
            end if;
            -- Detection of collision with border
            if (imx + IM_WIDTH = DISP_WIDTH and dir_x = '1') or (imx = 0 and dir_x = '0') then
                dir_x := not dir_x;
            end if;
            if (imy + IM_HEIGHT = DISP_HEIGHT and dir_y = '1') or (imy = 0 and dir_y = '0') then
                dir_y := not dir_y;
            end if;
            if auto = '1' then
                if frame_end = '1' then
                    -- Move image
                    imx <= imx + 1 when dir_x else imx - 1;
                    imy <= imy + 1 when dir_y else imy - 1;
                end if;
            else
                if pulseU = '1' and not (imy = 0) then
                    tempy <= tempy - 1;
                elsif pulseD = '1' and not (imy + IM_HEIGHT = DISP_HEIGHT) then
                    tempy <= tempy + 1;
                end if;
                if pulseL = '1' and not (imx = 0) then
                    tempx <= tempx - 1;
                elsif pulseR = '1' and not (imx + IM_WIDTH = DISP_WIDTH) then
                    tempx <= tempx + 1;
                end if;
                if frame_end then
                    imx <= unsigned(signed(imx) + tempx);
                    imy <= unsigned(signed(imy) + tempy);
                    tempx <= (others => '0'); 
                    tempy <= (others => '0'); 
                end if;
            end if;
        end if;
    end process;
end architecture;