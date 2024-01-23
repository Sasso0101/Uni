library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity alu is
    port (
        in1: in signed (15 downto 0);
        in2: in signed (15 downto 0);
        sum, sub, mul: in std_logic;
        result: out signed (15 downto 0)
    );
end entity;

architecture rtl of alu is
    
begin
    process (in1, in2, sum, sub, mul)
        variable mul_result : signed (31 downto 0);
    begin
        -- Default initialization to avoid memories
        result <= in1;
        if sum = '1' then
            result <= in1 + in2;
        elsif sub = '1' then
            result <= in1 - in2;
        elsif mul = '1' then
            mul_result := in1 * in2;
            result <= mul_result(15 downto 0);
        end if;
    end process;
end architecture;