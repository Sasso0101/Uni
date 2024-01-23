library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity debouncer_tb is
end entity;

architecture rtl of debouncer_tb is
    component debouncer is
        generic (
            counter_size : integer := 3
        );
        port (
            clk   : in std_logic;
            reset : in std_logic;
            bouncy : in std_logic;
            pulse : out std_logic
        );
    end component;

    signal clk : std_logic := '0';
    signal reset : std_logic := '0';
    signal bouncy : std_logic := '0';
    signal pulse : std_logic := '0';
begin
    debouncer_0: debouncer port map (
        clk   => clk,
        reset => reset,
        bouncy => bouncy,
        pulse => pulse
    );

    clk <= not(clk) after 1 us;
    process begin
        reset <= '0';
        wait for 10 ns;
        reset <= '1';
        wait for 10 ns;
        bouncy <= '0';
        wait for 10 ns;
        bouncy <= '1';
        wait for 10 ns;
        bouncy <= '0';
        wait for 10 ns;
        bouncy <= '1';
        wait for 40 ns;
        bouncy <= '0';
        wait for 10 ns;
        bouncy <= '1';
        wait for 100 ns;
        bouncy <= '0';
        wait;
    end process;
    
end architecture;