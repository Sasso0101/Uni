library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity recognizer is
    port (
        command: in std_logic;
        start_ser: buffer std_logic;
        -- start_avg: out std_logic;
        busy: in std_logic;
        clk: in std_logic;
        reset: in std_logic
    );
end entity;

architecture rtl of recognizer is
    signal reg: unsigned( 2 downto 0 ) := "000";
begin
    process (clk, reset)
    begin
        if reset = '1' then
            start_ser <= '0';
            reg <= "000";
        elsif rising_edge(clk) then
            if start_ser = '1' then
                start_ser <= '0';
            elsif reg = "110" and command = '1' and busy = '0' then
                start_ser <= '1';
                reg <= "000";
            else
                reg(2) <= reg(1);
                reg(1) <= reg(0);
                reg(0) <= command;
            end if;
        end if;
    end process;
end architecture;