library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity counter is
    port (
        clk, res, count_up, count_down: in std_logic;
        min, max: out std_logic;
        ct: out unsigned(7 downto 0)
    );
end entity;

architecture rtl of counter is
begin
    process (res, clk)
    begin
        if res = '1' then
            ct <= (others => '0');
        elsif rising_edge(clk) then
            if count_down = '1' and min = '0' then
                ct <= ct - 1;
            elsif count_up = '1' and max = '0' then
                ct <= ct + 1;
            end if;
        end if;
    end process;
    min <= '1' when ct = 0 else
        '0';
    max <= '1' when ct = 255 else
        '0';
end architecture;