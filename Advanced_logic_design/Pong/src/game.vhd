library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.utilities.all;

entity game is
    port (
        clk : in std_logic;
        res : in std_logic; -- Reset
        BTNU, BTND : in std_logic; -- Player 1
        BTNL, BTNR : in std_logic; -- Player 2
        frame_end : in std_logic;
        CA, CB, CC, CD, CE, CF, CG, DP : out std_logic;
        AN : out std_logic_vector( 7 downto 0 );
        ball : out position;
        p1, p2 : out unsigned(POSITION_BITS downto 0)
    );
end game;

architecture rtl of game is
    signal pulseU, pulseL, pulseR, pulseD : std_logic;
    signal in_field : std_logic;
    signal p1_wins, p2_wins : unsigned(6 downto 0);
begin
    btn_u : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTNU,
        pulse => pulseU
    );
    btn_d : entity work.debouncer
    port map (
        clk => clk,
        res => res,
        bouncy => BTND,
        pulse => pulseD
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
    seven_segment : entity work.seven_segment_driver
    port map (
        clk => clk,
        res => res,
        digit1 => std_logic_vector(p1_wins),
        digit2 => std_logic_vector(p2_wins),
        CA => CA, CB => CB, CC => CC, CD => CD, CE => CE, CF => CF, CG => CG, DP => DP,
        AN => AN
    );

    in_field <= '1' when (ball.x >= BAR_DISTANCE + BAR_WIDTH) and (ball.x + BALL_LENGTH < DISP_WIDTH - BAR_DISTANCE - BAR_WIDTH) else '0';

    process (clk, res) is
        variable temp_p1, temp_p2: signed(POSITION_BITS downto 0);
        variable dirx, diry: std_logic;
        variable temp_ball : position_signed;
    begin
        if res = '0' then
            p1_wins <= (others => '0'); 
            p2_wins <= (others => '0');
            temp_p1 := BAR_INIT;
            temp_p2 := BAR_INIT;
            p1 <= unsigned(temp_p1);
            p2 <= unsigned(temp_p2);
            dirx := '1';
            diry := '1';
            temp_ball := BALL_INIT;
            ball.x <= unsigned(temp_ball.x);
            ball.y <= unsigned(temp_ball.y);
        elsif rising_edge(clk) then
            temp_p1 := temp_p1 - 1 when pulseU = '1' and temp_p1 > 0 else temp_p1;
            temp_p1 := temp_p1 + 1 when pulseD = '1' and temp_p1+BAR_HEIGHT < DISP_HEIGHT-1 else temp_p1;
            temp_p2 := temp_p2 - 1 when pulseL = '1' and temp_p2 > 0 else temp_p2;
            temp_p2 := temp_p2 + 1 when pulseR = '1' and temp_p2+BAR_HEIGHT < DISP_HEIGHT-1 else temp_p2;

            if frame_end then
                temp_ball.x := signed(ball.x)+SPEED when dirx else signed(ball.x)-SPEED;
                temp_ball.y := signed(ball.y)+SPEED when diry else signed(ball.y)-SPEED;

                if (in_field = '1') and (temp_ball.x <= integer(BAR_DISTANCE + BAR_WIDTH)) and (temp_ball.y >= signed(p1)) and (temp_ball.y <= signed(p1 + BAR_HEIGHT)) then
                    -- Collision with left bar
                    dirx := not dirx;
                    temp_ball.x := to_signed(BAR_DISTANCE + BAR_WIDTH, temp_ball.x'length);
                elsif (in_field = '1') and (temp_ball.x + BALL_LENGTH >= integer(DISP_WIDTH - BAR_DISTANCE - BAR_WIDTH)) and (temp_ball.y >= signed(p2)) and (temp_ball.y <= signed(p2 + BAR_HEIGHT)) then
                    -- Collision with right bar
                    dirx := not dirx;
                    temp_ball.x := to_signed(DISP_WIDTH - BAR_DISTANCE - BAR_WIDTH - BALL_LENGTH, temp_ball.x'length);
                elsif (temp_ball.x <= 0) then
                    -- Collision with left wall
                    p2_wins <= p2_wins + 1;
                    temp_ball := BALL_INIT;
                    dirx := '0';
                    diry := '1';
                elsif (temp_ball.x + BALL_LENGTH >= DISP_WIDTH) then
                    -- Collision with right wall
                    p1_wins <= p1_wins + 1;
                    temp_ball := BALL_INIT;
                    dirx := '1';
                    diry := '1';
                elsif temp_ball.y <= 0 then
                    -- Collision with top wall
                    diry := not diry;
                    temp_ball.y := (others => '0') ;
                elsif temp_ball.y >= DISP_HEIGHT-BALL_LENGTH-1 then
                    -- Collision with bottom wall
                    diry := not diry;
                    temp_ball.y := to_signed(DISP_HEIGHT-BALL_LENGTH-1, temp_ball.y'length);
                end if;
                ball.x <= unsigned(temp_ball.x);
                ball.y <= unsigned(temp_ball.y);
                p1 <= unsigned(temp_p1);
                p2 <= unsigned(temp_p2);
            end if;
        end if;
    end process;
end architecture;