library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package utilities is
    constant DISP_WIDTH : natural := 1024; -- Display width
    constant DISP_HEIGHT : natural := 768; -- Display height
    constant POSITION_BITS : natural := 11; -- Number of bits needed to store a position on screen; given by ceil(log2(DISP_WIDTH))

    -- constant HA_END : natural := 1024; -- End of active pixels
    constant HS_STA : natural := DISP_WIDTH + 24; -- Duration of front porch
    constant HS_END : natural := HS_STA + 136; -- Duration of sync
    constant LINE_END : natural := 1344; -- Last pixel in line

    -- constant VA_END : natural := 768; -- End of active pixels
    constant VS_STA : natural := DISP_HEIGHT + 3; -- Start of sync after front porch
    constant VS_END : natural := VS_STA + 6; -- End of sync
    constant SCREEN_END : natural := 806; -- Last pixel in screen

    constant BAR_WIDTH : natural := 10; -- Width of bar
    constant BAR_HEIGHT : natural := 120; -- Height of bar
    constant BAR_DISTANCE : natural := 30; -- Bar distance from wall
    constant BALL_LENGTH : natural := 10; -- Length of ball side

    constant CENTER_X : signed := to_signed(DISP_WIDTH / 2, POSITION_BITS+1);
    constant CENTER_Y : signed := to_signed(DISP_HEIGHT / 2, POSITION_BITS+1);
    
    constant SPEED : natural := 5; -- Speed of ball (1 = 1px movement per frame)
    constant HOLD_COUNTER_SIZE : natural := 16; -- Number of clock cycles needed to register new button press (2^HOLD_COUNTER_SIZE).
    
    type position is record
        x : unsigned(POSITION_BITS downto 0);
        y : unsigned(POSITION_BITS downto 0);
    end record;
    type position_signed is record
        x : signed(POSITION_BITS downto 0);
        y : signed(POSITION_BITS downto 0);
    end record;

    constant BAR_INIT : signed := CENTER_Y-(BAR_HEIGHT/2);
    constant BALL_INIT : position_signed := (x => CENTER_X-(BALL_LENGTH/2), y => CENTER_Y-(BALL_LENGTH/2));
end package;