library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity calculator is
    port (
        CLK : in std_logic;
        SW : in  STD_LOGIC_VECTOR (15 downto 0); -- Switches
        LED : out  STD_LOGIC_VECTOR (15 downto 0); -- leds
        BTNC, BTNU, BTNL, BTNR, BTND : in std_logic; -- Buttons
        CA, CB, CC, CD, CE, CF, CG, DP : out std_logic; -- 7-segment display
        AN : out std_logic_vector( 3 downto 0 )
    );
end calculator;

architecture rtl of calculator is
    signal pulseC, pulseU, pulseL, pulseR, pulseD : std_logic;
    signal acc_in, acc_out : signed (15 downto 0);
    signal acc_enable : std_logic;
    signal display_value : std_logic_vector(15 downto 0);
begin
    -- Initialize buttons
    center_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => SW(15),
        bouncy  => BTNC,
        pulse   => pulseC
    );
    left_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => SW(15),
        bouncy  => BTNL,
        pulse   => pulseL
    );
    right_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => SW(15),
        bouncy  => BTNR,
        pulse   => pulseR
    );
    up_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => SW(15),
        bouncy  => BTNU,
        pulse   => pulseU
    );
    down_detect : entity work.debouncer(rtl) 
    port map (
        clk => clk,
        reset   => SW(15),
        bouncy  => BTND,
        pulse   => pulseD
    );

    -- Initialize seven segment display driver
    display_driver : entity work.seven_segment_driver( Behavioral ) 
    generic map ( 
        size => 21 
    )
    port map (
        clock => clk,
        reset => SW(15),
        digit0 => display_value( 3 downto 0 ),
        digit1 => display_value( 7 downto 4 ),
        digit2 => display_value( 11 downto 8 ),
        digit3 => display_value( 15 downto 12 ),
        ca     => CA,
        cb     => CB,
        cc     => CC,
        cd     => CD,
        ce     => CE,
        cf     => CF,
        cg     => CG,
        dp     => DP,
        an     => AN
    );

    -- Connect LEDs to switches
    LED <= SW;
    
    -- Initialize ALU
    alu_0 : entity work.alu(rtl) 
    port map (
        in1 => acc_out,
        in2 => signed (SW),
        sum => pulseU,
        sub => pulseL,
        mul => pulseR,
        result => acc_in
    );

    -- Initialize accumulator
    accumulator_0 : entity work.accumulator(rtl)
    port map (
        clk => clk,
        reset => SW(15),
        acc_init => pulseC,
        acc_enable => acc_enable,
        acc_in => acc_in,
        acc_out => acc_out
    );

    display_value <= std_logic_vector( acc_out );
    acc_enable <= btnU or btnL or btnL or btnD or btnR;
end architecture;