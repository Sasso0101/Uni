library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter_tb is
end entity;

architecture rtl of counter_tb is
    component counter is
        generic (
            counter_size : integer := 3
        );
        port (
            clk   : in std_logic;
            res : in std_logic;
            tc: out std_logic
        );
    end component;

    signal clk : std_logic := '0';
    signal res : std_logic := '0';
    signal tc_1 : std_logic := '0';
    signal tc_2 : std_logic := '0';
begin
    counter_0: counter
    generic map (
        counter_size => 3
    )
    port map (
        clk => clk,
        res => res,
        tc => tc_1
    );
    counter_1: counter
    generic map (
        counter_size => 4
    )
    port map (
        clk => clk,
        res => res,
        tc => tc_2
    );

    clk <= not(clk) after 1 us;
    process begin
        res <= '1';
        wait for 10 ns;
        res <= '0';
        wait for 100 ns;
        res <= '1';
        wait for 10 ns;
        res <= '0';
        wait;
    end process;
end architecture;