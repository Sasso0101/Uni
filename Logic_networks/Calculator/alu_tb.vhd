library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity alu_tb is
end entity;

architecture rtl of alu_tb is
    component alu is
        port (
            in1: in signed (31 downto 0);
            in2: in signed (31 downto 0);
            sum, sub, mul: in std_logic;
            result: out signed (31 downto 0)
        );
    end component;
    signal in1, in2 : signed (31 downto 0);
    signal sum, sub, mul: std_logic;
    signal result: signed (31 downto 0);
begin
    alu_0: alu port map (
        in1 => in1, in2 => in2, sum => sum, sub => sub, mul => mul, result => result
    );
    process begin
        in1 <= to_signed(15, 32);
        in2 <= to_signed(3, 32);
        sum <= '1';
        wait for 10 ns;
        sum <= '0';
        in1 <= to_signed(12, 32);
        in2 <= to_signed(3, 32);
        sub <= '1';
        wait for 10 ns;
        sub <= '0';
        in1 <= to_signed(9, 32);
        in2 <= to_signed(3, 32);
        mul <= '1';
        wait for 10 ns;
        wait;
    end process;
end architecture;