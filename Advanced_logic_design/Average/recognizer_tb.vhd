library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity recognizer_tb is
end entity;

architecture rtl of recognizer_tb is
    component recognizer is
        port (
            command: in std_logic;
            start_ser: buffer std_logic;
            -- start_avg: out std_logic;
            busy: in std_logic;
            clk: in std_logic;
            reset: in std_logic
        );
    end component;
    signal command, busy, start_ser, clk, reset : std_logic := '0';
begin
    recognizer_0: recognizer port map (
        command => command, start_ser => start_ser, busy => busy, clk => clk, reset => reset
    );

    clk <= not(clk) after 5 ns;

    process begin
        reset <= '1';
        wait for 10 ns;
        reset <= '0';
        command <= '1';
        wait for 10 ns;
        command <= '1';
        wait for 10 ns;
        command <= '0';
        wait for 10 ns;
        command <= '1';
        wait for 20 ns;
        busy <= '1';
        wait;
    end process;
end architecture;