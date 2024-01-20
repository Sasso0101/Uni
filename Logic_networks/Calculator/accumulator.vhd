library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity accumulator is
    port (
        clk, reset: in std_logic;
        acc_init, acc_enable: in std_logic;
        acc_in: in signed (31 downto 0);
        acc_out: out signed (31 downto 0)
    );
end entity;

architecture rtl of accumulator is
begin
    process (clk, reset)
    begin
        if reset = '0' then
            acc_out <= ( others => '0');
        elsif rising_edge(clk) then
            if acc_init = '1' then
                acc_out <= ( others => '0');
            else
                acc_out <= acc_in;
            end if;
        end if;
    end process;
end architecture;