library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter_tb is
end entity;

architecture rtl of counter_tb is
    component counter is
        port (
            clk, res, count_up, count_down: in std_logic;
            min, max: out std_logic;
            ct: out unsigned(7 downto 0)
        );
    end component;
    signal clk, res, count_up, count_down: std_logic := '0';
    signal min, max: std_logic;
    signal ct: unsigned(7 downto 0);
begin
    counter_0: counter port map (
        clk => clk,
        res => res,
        count_up => count_up,
        count_down => count_down,
        min => min,
        max => max,
        ct => ct
    );

    clk <= not(clk) after 5 ns;

    process begin
        res <= '1';
        wait for 10 ns;
        res <= '0';
        wait for 10 ns;
        count_up <= '1';
        wait for 2.7 us;
        count_up <= '0';
        count_down <= '1';
        wait for 2 us;
        count_down <= '0';
        res <= '1';
        wait;
    end process;
end architecture;