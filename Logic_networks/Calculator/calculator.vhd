library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity calculator is
    port (
        SW : in  STD_LOGIC_VECTOR (15 downto 0); --! Switches
        BTN : in  STD_LOGIC_VECTOR (4 downto 0); --! Buttons (0:UP,1:center,2:down)
        CLK : in  STD_LOGIC; --! clock
        LED : out  STD_LOGIC_VECTOR (15 downto 0) --! LEDs
    );
end calculator;

architecture rtl of calculator is
begin
end architecture;